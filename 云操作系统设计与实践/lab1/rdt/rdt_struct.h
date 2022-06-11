/*
 * FILE: rdt_struct.h
 * DESCRIPTION: The header file for basic data structures.
 * NOTE: Do not touch this file!
 */


#ifndef _RDT_STRUCT_H_
#define _RDT_STRUCT_H_

/* sanity check utility */
#define ASSERT(x) \
    if (!(x)) { \
        fprintf(stdout, "## at file %s line %d: assertion fails\n", __FILE__, __LINE__); \
        exit(-1); \
    }

/* a message is a data unit passed between the upper layer and the rdt layer at 
   the sender */
struct message {
    int size;
    char *data;
};

/* a packet is a data unit passed between rdt layer and the lower layer, each 
   packet has a fixed size */
#define RDT_PKTSIZE 128

struct packet {
    char data[RDT_PKTSIZE];
};

#endif  /* _RDT_STRUCT_H_ */
