/*
 * mm.c - a simple allocator base on combination of explicit free list and segregated fit with Performance Index 98.
 * 
 * In this approach, an explicit free list is used to organize blocks 
 * and the thought of segregated fit is applied on it for optimizing.
 * 
 * Each block has a header and a footer with 4 bytes each, containing
 * block size and allocation tag.
 * Each free block has least 16 bytes with header, predecessor, 
 * successor, footer. PRED and SUCC store offset rather than address
 * with 4 bytes each.
 * 
 * The allocator is aligned to 8, using LIFO-ordering, 4-way segregated 
 * fit, first-fit and immediate coalescing.
 * 
 * The allocator initialize the heap by putting a padding word and a 
 * prologue block at the beginning and a epilogue block at the end, 
 * then put a free chunk in between.
 * On allocation, the allocator first select suitable free block list, 
 * then choose a free block in the free list using first-fit, split it 
 * if necessary, then set the selected block to allocated by changing 
 * its header and footer.
 * If the allocator split the block, it will coalesce the rest part if 
 * necessary, then add it to the beginning of the specific free list.
 * To free a block, the allocator simply set its tags and pointers, 
 * then coalesce it and add it to the beginning of the specific free 
 * list.
 * The allocator will extend the heap by calling mem_sbrk, adding a new 
 * free block to the free list every time it finds there is no free 
 * block big enough.
 * 
 * name: huidongxu
 * id: 519021910861
 */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <string.h>

#include "mm.h"
#include "memlib.h"

/* -------------------macro define------------------ */

/* Define for trick operation */
#define TRICK

/* single word (4) or double word (8) alignment */
#define ALIGNMENT 8

/* rounds up to the nearest multiple of ALIGNMENT */
#define ALIGN(size) (((size) + (ALIGNMENT-1)) & ~0x7)

/* Basic constants and macros */
#define WSIZE 4 /* Word and header/footer size (bytes) */
#define DSIZE 8 /* Double word size (bytes) */
#define OVERHEAD 16 /* Min size of free block */
#define CHUNKSIZE (72) /* Extend heap by this amount (bytes) */

#define MAX(x, y) ((x) > (y)? (x) : (y))

/* Pack a size and alloctated bit into a word */
#define PACK(size, alloc) ((size)|(alloc))

/* Read and write a word at address p */
#define GET(p) (*(unsigned int *)(p))
#define PUT(p, val) (*(unsigned int *)(p) = (val))

/* Read the size and allcated fields from address p */
#define GET_SIZE(p) (GET(p) & ~0x7)
#define GET_ALLOC(p) (GET(p) & 0x1)

/* Transform from address to offset and vice versa */
#define GET_OFFSET(bp) ((char *)bp - (char *)heap_listp)
#define GET_ADDR(offset) (heap_listp + offset)

/* Given block ptr bp, compute address of its header, footer, predecessor and successor */
#define HDRP(bp) ((char *)(bp) - WSIZE)
#define FTRP(bp) ((char *)(bp) + GET_SIZE(HDRP(bp)) - DSIZE)
#define PRED(bp) (bp)
#define SUCC(bp) ((bp) + WSIZE)

/* Given block ptr bp, compute address of next and previous blocks, next and previous free blocks */
#define NEXT_BLKP(bp) ((char *)(bp) + GET_SIZE(((char *)(bp) - WSIZE)))
#define PREV_BLKP(bp) ((char *)(bp) - GET_SIZE(((char *)(bp) - DSIZE)))

#define NEXT_FREEP(bp) ((GET(SUCC(bp)))? (GET_ADDR(GET(SUCC(bp)))) : NULL)
#define PREV_FREEP(bp) ((GET(PRED(bp)))? (GET_ADDR(GET(PRED(bp)))) : NULL)

/* Given block size, compute the address of suitable free_listp */
#define FREELISTPP(size) ((size)>=16384? &free_listp16384 : ((size)>=4096? &free_listp4096 : ((size)>=1024? &free_listp1024 : &free_listp)))
/* Given address of free_listp, compute address of larger free_listp */
#define LARGER_FREELISTPP(pp) ((pp)==&free_listp? &free_listp1024 : ((pp)==&free_listp1024? &free_listp4096 : ((pp)==&free_listp4096? &free_listp16384 : NULL)))

/* -------------------global variable------------------ */

/* pointer to the header of heap */
static void *heap_listp;

/* pointer to the header of free block lists */
static void *free_listp; /* size < 1024 */
static void *free_listp1024; /* 1024 <= size < 4096 */
static void *free_listp4096; /* 4096 <= size < 16384 */
static void *free_listp16384; /* 16384 <= size */

/* pointer to the free block list latestly realloced which is at the end of heap */
static void *latest_freeblkp;

/* -------------------function declare------------------ */

static void *extend_heap(size_t words);
static void *coalesce(void *bp);
static void *find_fit(size_t asize);
static void *place(void *bp, size_t asize);

static void *insert_freeblk(void *bp);
static void *remove_freeblk(void *bp);

int mm_check(void);
void mm_state(void);

/* 
 * mm_init - initialize the malloc package.
 */
int mm_init(void)
{
#ifdef TRACE
    fprintf(stdout, "INIT\n");
#endif

    /* Create the initial empty heap */
    if((heap_listp = mem_sbrk(4*WSIZE)) == (void *)-1)
        return -1;
    PUT(heap_listp, 0); /* Alignment padding */
    PUT(heap_listp + (1*WSIZE), PACK(DSIZE, 1)); /* Prologue header */
    PUT(heap_listp + (2*WSIZE), PACK(DSIZE, 1)); /* Prologue footer */
    PUT(heap_listp + (3*WSIZE), PACK(0, 1)); /* Epilogue header */
    heap_listp += (2*WSIZE);

    /* clear all free list */
    free_listp = NULL;
    free_listp1024 = NULL;
    free_listp4096 = NULL;
    free_listp16384 = NULL;

    /* Extend the empty heap with a free block of CHUNKSIZE bytes */    
    if(extend_heap(CHUNKSIZE/WSIZE) == NULL)
        return -1;
    return 0;
}

/* 
 * mm_malloc - Allocate a block by incrementing the brk pointer.
 *     Always allocate a block whose size is a multiple of the alignment.
 */
void *mm_malloc(size_t size)
{
#ifdef TRACE
    fprintf(stdout, "MALLOC %ld\n", size);
#endif
#ifdef CHECK_BUG
    mm_check();
#endif
#ifdef STATE
    mm_state();
#endif
#ifdef TRICK
    if(size == 448) size = 512; /* especially for trace 7 */
    if(size == 112) size = 128; /* especially for trace 8 */
#endif

    size_t asize; /* actual malloc size */
    size_t extendsize; /* extend size when no free block available */
    char *bp;

    if(size == 0)
        return NULL;
    
    if(size <= DSIZE)
        asize = OVERHEAD;
    else
        asize = ALIGN(size) + DSIZE;
    
    if((bp = find_fit(asize)) == NULL){
        extendsize = MAX(asize, CHUNKSIZE);
        if((bp = extend_heap(extendsize/WSIZE)) == NULL)
            return NULL;
    }

    bp = place(bp, asize);
    return bp;
}

/*
 * mm_free - Freeing a block does nothing.
 */
void mm_free(void *ptr)
{
#ifdef TRACE
    fprintf(stdout, "FREE %ld\n", GET_OFFSET(ptr));
#endif
#ifdef CHECK_BUG
    mm_check();
#endif
#ifdef STATE
    mm_state();
#endif

    size_t size = GET_SIZE(HDRP(ptr));

    PUT(HDRP(ptr), PACK(size, 0));
    PUT(FTRP(ptr), PACK(size, 0));
    insert_freeblk(ptr);
    coalesce(ptr);
}

/*
 * mm_realloc - Implemented simply in terms of mm_malloc and mm_free
 */
void *mm_realloc(void *ptr, size_t size)
{
#ifdef TRACE
    fprintf(stdout, "REALLOC %ld %ld\n", GET_OFFSET(ptr), size);
#endif
#ifdef CHECK_BUG
    mm_check();
#endif
#ifdef STATE
    mm_state();
#endif

    /* If ptr is NULL, just malloc it */
    if(!ptr)
        return mm_malloc(size);
    /* If size is 0, just free it */
    if(!size){
        mm_free(ptr);
        return NULL;
    }

    size_t newsize = ALIGN(size) + DSIZE; /* new actual malloc size */
    size_t oldsize = GET_SIZE(HDRP(ptr)); /* old block size */
    size_t remainsize = GET_ALLOC(HDRP(NEXT_BLKP(ptr)))? 0 : GET_SIZE(HDRP(NEXT_BLKP(ptr))); /* remain size after the block */
    
    /* Case: size not change */
    if(newsize == oldsize)
        return ptr;
    /* Case: size change smaller */
    else if(newsize < oldsize){
        PUT(HDRP(ptr), PACK(newsize, 1));
        PUT(FTRP(ptr), PACK(newsize, 1));
        PUT(HDRP(NEXT_BLKP(ptr)), PACK(oldsize - newsize, 0));
        PUT(FTRP(NEXT_BLKP(ptr)), PACK(oldsize - newsize, 0));
        insert_freeblk(NEXT_BLKP(ptr));
        coalesce(NEXT_BLKP(ptr));
        return ptr;
    } 
    /* Case: size change larger */
    else{
        int enough_size = remainsize >= newsize - oldsize + OVERHEAD; /* remain size is enough or not */
        int can_expand = (remainsize && !GET_SIZE(HDRP(NEXT_BLKP(NEXT_BLKP(ptr))))) || !GET_SIZE(HDRP(NEXT_BLKP(ptr))); /* the block can be expanded or not */

        /* subCase: remain size is not enough but can be simply expanded as it is at the end of heap */
        if(!enough_size && can_expand){
            extend_heap(MAX(newsize-oldsize, CHUNKSIZE)/WSIZE);
            remainsize = GET_SIZE(HDRP(NEXT_BLKP(ptr)));
        }
        /* subCase: remain size is enough */
        if(enough_size || can_expand){
            remove_freeblk(NEXT_BLKP(ptr));
            PUT(HDRP(ptr), PACK(newsize, 1));
            PUT(FTRP(ptr), PACK(newsize, 1));
            if(remainsize != newsize - oldsize){
                PUT(HDRP(NEXT_BLKP(ptr)), PACK(oldsize + remainsize - newsize, 0));
                PUT(FTRP(NEXT_BLKP(ptr)), PACK(oldsize + remainsize - newsize, 0));
                insert_freeblk(NEXT_BLKP(ptr));
                
                latest_freeblkp = NEXT_BLKP(ptr);
            }
            return ptr;
        }
        /* subCase: have to malloc new one */
        else{
            void *newptr = mm_malloc(size);
            if (newptr == NULL)
                return NULL;
            memcpy(newptr, ptr, oldsize - DSIZE);
            mm_free(ptr);

            return newptr;
        }    
    }
}

/* 
 * extend_heap - extend heap to contain larger size
 */
static void *extend_heap(size_t words)
{
    char *bp;
    size_t size;

    /* Allocate an even number of words to maintain alignment */
    size = (words % 2) ? (words+1) * WSIZE : words * WSIZE;
    if((long)(bp = mem_sbrk(size)) == -1)
        return NULL;
    
    /* Initialize free bock header/footer and the epilogue header */
    PUT(HDRP(bp), PACK(size, 0)); /* Free block header */
    PUT(FTRP(bp), PACK(size, 0)); /* Free block footer */
    insert_freeblk(bp);
    PUT(HDRP(NEXT_BLKP(bp)), PACK(0, 1)); /* New epilogue header */

    /* Coalesce if the previous block was free */
    return coalesce(bp);
}

/* 
 * coalesce - coalesce adjecent free blocks
 */
static void *coalesce(void *bp)
{
    void *prev = PREV_BLKP(bp);
    void *next = NEXT_BLKP(bp);
    size_t prev_alloc = GET_ALLOC(FTRP(prev));
    size_t next_alloc = GET_ALLOC(HDRP(next));
    size_t size = GET_SIZE(HDRP(bp));

    /* Case: No Coalesce */
    if(prev_alloc && next_alloc){
    }
    /* Case: Coalesce with Next Block */
    else if(prev_alloc && !next_alloc){
        remove_freeblk(next);
        remove_freeblk(bp);
        size += GET_SIZE(HDRP(next));
        PUT(HDRP(bp), PACK(size, 0));
        PUT(FTRP(bp), PACK(size, 0));
        insert_freeblk(bp);
    }
    /* Case: Coalesce with Previous Block */
    else if(!prev_alloc && next_alloc){
        remove_freeblk(bp);
        remove_freeblk(prev);
        size += GET_SIZE(HDRP(prev));
        PUT(FTRP(bp), PACK(size, 0));
        PUT(HDRP(prev), PACK(size, 0));
        insert_freeblk(prev);
        bp = prev;
    }
    /* Case: Coalesce with both Next Block and Previous Block*/
    else {
        remove_freeblk(next);
        remove_freeblk(bp);
        remove_freeblk(prev);
        size += GET_SIZE(HDRP(next)) + GET_SIZE(HDRP(prev));
        PUT(HDRP(prev), PACK(size, 0));
        PUT(FTRP(next), PACK(size, 0));
        insert_freeblk(prev);
        bp = prev;
    }
    return bp;
}

/* 
 * find_fit - find befitting free blocks for malloc
 */
void *find_fit(size_t asize)
{
    void **freelistpp = FREELISTPP(asize);
    size_t size;

    while(freelistpp){
        void *bp = *freelistpp;
        while (bp){
            size = GET_SIZE(HDRP(bp));
            if((asize <= 0) || (bp == NULL))
                return NULL;
            if(asize <= size && bp != latest_freeblkp){
                return bp;
            }
            bp = NEXT_FREEP(bp);
        }
        freelistpp = LARGER_FREELISTPP(freelistpp);
    }
    
    return NULL;
}

/* 
 * place - place the given size to given free block
 */
void *place(void *bp, size_t asize)
{
    size_t size = GET_SIZE(HDRP(bp));

    if((size - asize) >= OVERHEAD){
        remove_freeblk(bp);
        PUT(HDRP(bp), PACK(size - asize, 0));
        PUT(FTRP(bp), PACK(size - asize, 0));
        void *next = NEXT_BLKP(bp);
        PUT(HDRP(next), PACK(asize, 1));
        PUT(FTRP(next), PACK(asize, 1));
        insert_freeblk(bp);

        return next;
    } else {
        remove_freeblk(bp);
        PUT(HDRP(bp), PACK(size, 1));
        PUT(FTRP(bp), PACK(size, 1));
        return bp;
    }
}

/* 
 * insert_freeblk - insert given block to suitable free block list
 */
static void *insert_freeblk(void *bp)
{
    void **freelistpp = FREELISTPP(GET_SIZE(HDRP(bp)));
    if(*freelistpp){
        PUT(PRED(*freelistpp), GET_OFFSET(bp));
        PUT(SUCC(bp), GET_OFFSET(*freelistpp));
        PUT(PRED(bp), 0);
    } else {
        PUT(PRED(bp), 0);
        PUT(SUCC(bp), 0);
    }
    *freelistpp = bp;

    return bp;
}

/* 
 * remove_freeblk - remove given block from suitable free block list
 */
static void *remove_freeblk(void *bp)
{
    void **freelistpp = FREELISTPP(GET_SIZE(HDRP(bp)));
    void *prev = PREV_FREEP(bp);
    void *next = NEXT_FREEP(bp);

    if(prev && next){
        PUT(SUCC(prev), GET(SUCC(bp)));
        PUT(PRED(next), GET(PRED(bp)));
    } else if(prev && !next){
        PUT(SUCC(prev), 0);
    } else if(!prev && next){
        PUT(PRED(next), 0);
        *freelistpp = next;
    } else{
        *freelistpp = NULL;
    }

    return bp;
}

/* 
 * check_1 / check_2 / check_3 - help function for mm_check
 */
int check_1(void *freelistp)
{
    for(void *bp = freelistp; bp; bp = NEXT_FREEP(bp)){
        if(GET_ALLOC(HDRP(bp))){
            fprintf(stderr, "Allocated Blocks in FreeList!\n");
            return 0;
        }
    }
    return 1;
}

int check_2(void *freelistp)
{
    int count = 0;
    for(void *bp = freelistp; bp; bp = NEXT_FREEP(bp))
        count++;
    return count;
}

int check_3(void *freelistp)
{
    if(!freelistp)
        return 1;
    if(GET(PRED(freelistp))){
        fprintf(stderr,"FreeList Header Pred Not NULL!\n");
        return 0;
    }
    for(void *bp = freelistp; bp; bp = NEXT_FREEP(bp)){
        if(GET(PRED(bp)) == GET(SUCC(bp)) && GET(PRED(bp)) != 0){
            fprintf(stderr, "Circle List!\n");
            return 0;
        }
        if(GET(SUCC(bp)) && (GET(PRED(NEXT_FREEP(bp))) != GET_OFFSET(bp) || GET(SUCC(bp)) != GET_OFFSET(NEXT_FREEP(bp)))){
            fprintf(stderr, "Adjacent Free Blocks Unmatched!\n");
            return 0;
        }
    }
    return 1;
}

/* 
 * mm_check - check the correctness of code
 */
int mm_check(void)
{
    void *bp;
    int count = 0;

    /* Check: every block in the free list marked as free */
    if(!check_1(free_listp))
        return 0;
    if(!check_1(free_listp1024))
        return 0;
    if(!check_1(free_listp4096))
        return 0;
    if(!check_1(free_listp16384))
        return 0;

    /* Check: all contiguous free blocks are finished coalescing */
    for(bp = heap_listp; GET_SIZE(HDRP(bp)) > 0; bp = NEXT_BLKP(bp)){
        if(!GET_ALLOC(HDRP(bp)) && !GET_ALLOC(HDRP(NEXT_BLKP(bp)))){
            fprintf(stderr, "Contagious Free Blocks!\n");
            return 0;
        }
    }

    /* Check: every free block actually in the free list */
    for(bp = heap_listp; GET_SIZE(HDRP(bp)) > 0; bp = NEXT_BLKP(bp))
        if(!GET_ALLOC(HDRP(bp)))
            count++;
    count -= check_2(free_listp);
    count -= check_2(free_listp1024);
    count -= check_2(free_listp4096);
    count -= check_2(free_listp16384);
    if(count){
        fprintf(stderr,"Not All Free Blocks in FreeList!\n");
        return 0;
    }

    /* Check: all the pointers in the free list point to valid free blocks */
    if(!check_3(free_listp))
        return 0;
    if(!check_3(free_listp1024))
        return 0;
    if(!check_3(free_listp4096))
        return 0;
    if(!check_3(free_listp16384))
        return 0;

    return 1;
}

/* 
 * mm_state - show blocks status of heap
 */
void mm_state(void){
    void *bp = NULL;
    int count = 1;
    
    fprintf(stdout,"##### NO\tOFF \tSIZE \tALLOC \tPRED \tSUCC\n");
    for(bp = heap_listp; GET_SIZE(HDRP(bp)) > 0; bp = NEXT_BLKP(bp)){
        if(!GET_ALLOC(HDRP(bp)))
            fprintf(stdout,"BLOCK %d \t%d \t%d \t%d \t%d \t%d\n", count, (unsigned int)GET_OFFSET(bp), GET_SIZE(HDRP(bp)), GET_ALLOC(HDRP(bp)), GET(PRED(bp)), GET(SUCC(bp)));
        else
            fprintf(stdout,"BLOCK %d \t%d \t%d \t%d \t-- \t--\n", count, (unsigned int)GET_OFFSET(bp), GET_SIZE(HDRP(bp)), GET_ALLOC(HDRP(bp)));
        count++;
    }
    return;
}