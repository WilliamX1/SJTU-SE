/* name: Huidong Xu
 * id: 519021910861
 */
#include "cachelab.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <time.h>
#include <string.h>
#include <stdbool.h>

/* I: an instruction load
 * L: a data load
 * S: a data store
 * M: a data modify(a data load followed by a data store)
 */

typedef struct line{
    bool valid_bit;
    size_t tag;
    clock_t update_time;
}line_t;
typedef struct set{
    line_t *line_table;
}set_t;
/* Parameters */
bool display_mode = false;
int set_bit = 0;
int block_bit = 0;

size_t set_num = 0;
size_t line_num = 0;
size_t block_size = 0;
set_t *set_table = NULL;
/* Helper variables */
size_t tag_mask = 0;
size_t set_mask = 0;
size_t block_mask = 0;

/* print_usage - print help info */
void print_usage(){
    printf("Usage: ./csim [-hv] -s <num> -E <num> -b <num> -t <file>\n");
    printf("Options:\n");
    printf("  -h         Print this help message.\n");
    printf("  -v         Optional verbose flag.\n");
    printf("  -s <num>   Number of set index bits.\n");
    printf("  -E <num>   Number of lines per set.\n");
    printf("  -b <num>   Number of block offset bits.\n");
    printf("  -t <file>  Trace file.\n\n");
    printf("Examples:\n");
    printf("  linux>  ./csim -s 4 -E 1 -b 4 -t traces/yi.trace\n");
    printf("  linux>  ./csim -v -s 8 -E 2 -b 4 -t traces/yi.trace\n");
}

/* app_error - error handler wrapper */
void app_error(char *err_msg, char *object) {
    if (err_msg != NULL) {
        if (object != NULL) printf("%s: %s.\n", object, err_msg);
        else printf("%s.\n", err_msg);
    }
    exit(1);
}

/* sysinit_cache - initial the cache system */
void sysinit_cache(){
    /* Initial set table and line */
    set_table = (set_t*) malloc (set_num * sizeof(set_t));
    for (size_t i = 0; i < set_num; i++)
        set_table[i].line_table = (line_t*) calloc (line_num, sizeof(line_t));

    /* Initial mask */
    block_mask = (1 << block_bit) - 1;
    set_mask = ((1 << (set_bit + block_bit)) - 1) ^ block_mask;
    tag_mask = ~(block_mask | set_mask);
}

/* getinfo_pos - get index info from the address */
void getinfo_pos(size_t address, size_t* tag, size_t* set_index, size_t* block_offset) {
    *tag = (address & tag_mask) >> (set_bit + block_bit);
    * set_index = (address & set_mask) >> block_bit;
    *block_offset = (address & block_mask);
}
/* find_cache - find the cache line related to the address */
line_t* find_cacheline(size_t tag, size_t set_index, size_t block_offset) {
    line_t* line_table = set_table[set_index].line_table;
    for (int i = 0; i < line_num; i++) {
        line_t* line = &line_table[i];
        if (line->valid_bit && line->tag == tag) return line;
    };
    return NULL;
}
/* find_place - find the line that the data should be stored in */
line_t *find_place(size_t tag, size_t set_index, bool *eviction_c) {
    line_t *line_table = set_table[set_index].line_table;
    line_t *lru_line = &line_table[0];
    
    for(int i = 0; i <line_num; i++){
        line_t *line = &line_table[i];
        if(!line->valid_bit){
            *eviction_c = false;
            return line;
        }
        if(line->valid_bit && line->update_time < lru_line->update_time){
            lru_line = line;
        }
    }

    *eviction_c = true;
    return lru_line;
}
/* print_detail - print information for each memory access */
void print_detail(char access_type, size_t address, int byte, bool hit_c, bool eviction_c) {
    if (!display_mode) return;

    switch (access_type)
    {
    case 'L': case 'S':
        if (hit_c) printf("%c %lx,%d hit\n", access_type, address, byte);
        else if (eviction_c) printf("%c %lx,%d misss eviction\n", access_type, address, byte);
        else printf("%c %lx,%d miss\n", access_type, address, byte);
        break;
    case 'M':
        if (hit_c) printf("%c %lx,%d hit hit\n", access_type, address, byte);
        else if (eviction_c) printf("%c %lx,%d miss eviction hit\n", access_type, address, byte);
        else printf("%c %lx,%d miss hit\n", access_type, address, byte);
    default:
        break;
    }
}
/*load_cache - update the info of line */
void load_cache(size_t tag, line_t *line) {
    line->valid_bit = true;
    line->tag = tag;
    line->update_time = clock();
}

int main(int argc, char* argv[])
{
    /* Arguments */
    char *file_name = NULL;
    FILE *trace_file = NULL;

    /* Read Arguments from command line */
    char option;
    while ((option = getopt(argc, argv, "hvs:E:b:t:")) != -1) {
        switch (option)
        {
        case 'h':
            print_usage();
            exit(1);
            break;
        case 'v':
            display_mode = true;
            break;
        case 's':
            set_bit = atoi(optarg);
            break;
        case 'E':
            line_num = atoi(optarg);
            break;
        case 'b':
            block_bit = atoi(optarg);
            break;
        case 't':
            if ((file_name = malloc(strlen(optarg))) == NULL) {
                // printf("-------------------------");
                print_usage();
                exit(1);
            };
            memcpy(file_name, optarg, strlen(optarg));
            break;
        default:
            print_usage();
            exit(1);
            break;
        }
    };
    if (set_bit <= 0 || line_num <= 0 || block_bit <= 0) {
        print_usage();
        exit(1);
    };

    set_num = 1 << set_bit;
    block_size = 1 << block_bit;

    if (!(trace_file = fopen(file_name, "r"))) app_error("No such file or directory", file_name);

    /* Init */
    sysinit_cache();

    /* write and operate on cache */
    char access_type;
    size_t address;
    size_t tag;
    size_t set_index;
    size_t block_offset;
    int byte;
    int hits = 0, misses = 0, evictions = 0;

    while (fscanf(trace_file, "%c %lx,%d", &access_type, &address, &byte) != EOF) {
        line_t *line = NULL;
        /* hit and eviction condition */
        bool hit_c = true;
        bool eviction_c = false;

        /* check for the correctness of access type */
        if (!(access_type == 'L' || access_type == 'M' || access_type == 'S')) continue;

        /* Read information from address */
        getinfo_pos(address, &tag, &set_index, &block_offset);

        if ((line = find_cacheline(tag, set_index, block_offset)) == NULL) {
            hit_c = false;
            line = find_place(tag, set_index, &eviction_c);
        }

        /* print detail if having -v */
        print_detail(access_type, address, byte, hit_c, eviction_c);

        /* General condition for 'S', 'L' and the load operation of 'M' */
        if (hit_c) hits++;
        else misses++;
        if (eviction_c) evictions++;

        /* The store operation of 'M' will hit certainly */
        if (access_type == 'M') hits++;

        /* uodate information of line */
        load_cache(tag, line);
    }
    printSummary(hits, misses, evictions);
    return 0;
}
