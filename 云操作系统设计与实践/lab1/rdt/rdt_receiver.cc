/*
 * FILE: rdt_receiver.cc
 * DESCRIPTION: Reliable data transfer receiver.
 * NOTE: This implementation assumes there exists packet loss, corruption, or 
 *       reordering. In this implementation, the ack/nack packet format is laid out as 
 *       the following:
 *       
 *       |<-  4 byte  ->|<-  2 byte  ->|<-  1 byte  ->|
 *       | sequence_num |  hash_value  |  ack or nack |
 *
 *       The first 4 bytes indicates the sequence number of each packet
 *       The following 2 bytes indicates hash value of the sequence number to handle with corruption
 *       The following 1 bytes indicates the type of the packet, 1 for ack, 0 for nack
 */


#include <cstring>
#include <functional>
#include <iostream>
#include <mutex>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <unordered_map>

#include "rdt_struct.h"
#include "rdt_receiver.h"

const size_t window_size = 10;
const size_t sizeof_sequence_num = 4;
const size_t sizeof_hash_value = 2;
const size_t sizeof_payload_size = 1;
const size_t sizeof_ack = 1;
/* 1-byte header indicating the size of the payload */
const size_t header_size = sizeof_sequence_num + sizeof_hash_value + sizeof_payload_size;
/* position pointer of each */
const size_t pointer_sequence_num = 0;
const size_t pointer_hash_value = sizeof_sequence_num;
const size_t pointer_payload_size = sizeof_sequence_num + sizeof_hash_value;
const size_t pointer_ack = sizeof_sequence_num + sizeof_hash_value;
const size_t pointer_payload = sizeof_sequence_num + sizeof_hash_value + sizeof_payload_size;

const int hash_mod = 0x00007fff; /* hash_value mask */
const double timeout = 0.3;

unsigned int receiver_next_seq_num = 0;
/* buffer */
std::unordered_map<unsigned int, packet*> receiver_buffer;

/* hash the data for checksum */
const std::hash<std::string> hash_fn;

std::mutex receiver_mutex;

void send_ack(unsigned int ack_num, bool is_ack) {
    struct packet* ack_pkt = (struct packet*) malloc(sizeof(struct packet));
    ASSERT(ack_pkt != NULL);
    short ha_value = hash_fn(std::to_string(ack_num) + std::to_string(is_ack)) % hash_mod;
    memcpy(ack_pkt->data + pointer_sequence_num, &ack_num, sizeof_sequence_num);
    memcpy(ack_pkt->data + pointer_hash_value, &ha_value, sizeof_hash_value);
    memcpy(ack_pkt->data + pointer_ack, &is_ack, sizeof_ack);
    Receiver_ToLowerLayer(ack_pkt); // printf("Receiver: send ack %d abnormal\n", ack_num);
};

/* receiver initialization, called once at the very beginning */
void Receiver_Init()
{
    fprintf(stdout, "At %.2fs: receiver initializing ...\n", GetSimulationTime());
    receiver_next_seq_num = 0;
    receiver_buffer.clear();
}

/* receiver finalization, called once at the very end.
   you may find that you don't need it, in which case you can leave it blank.
   in certain cases, you might want to use this opportunity to release some 
   memory you allocated in Receiver_init(). */
void Receiver_Final()
{
    fprintf(stdout, "At %.2fs: receiver finalizing ...\n", GetSimulationTime());
}

/* event handler, called when a packet is passed from the lower layer at the 
   receiver */
void Receiver_FromLowerLayer(struct packet *pkt)
{
    receiver_mutex.lock();

    unsigned int seq_num = 0;
    memcpy(&seq_num, pkt->data + pointer_sequence_num, sizeof_sequence_num);
    short hash_value = 0;
    memcpy(&hash_value, pkt->data + pointer_hash_value, sizeof_hash_value);
    char payload_size = 0;
    memcpy(&payload_size, pkt->data + pointer_payload_size, sizeof_payload_size); 
    
    /* sanity check in case the packet is corrupted */
    if (payload_size < 0) payload_size = 0;
    if (payload_size > RDT_PKTSIZE - header_size) payload_size = RDT_PKTSIZE - header_size;
    std::string data(pkt->data + pointer_payload, pkt->data + pointer_payload + payload_size);
    short cal_hash_value = hash_fn(std::to_string(payload_size) + std::to_string(seq_num) + data) % hash_mod;

    // printf("Receiver: receive packet[%d] payload_size %d\n", seq_num, payload_size);

    if (hash_value == cal_hash_value && seq_num == receiver_next_seq_num) {
      /* construct a message and deliver to the upper layer */
      receiver_buffer[seq_num] = pkt;

      while (receiver_buffer.find(receiver_next_seq_num) != receiver_buffer.end()) {          
        packet* more_pkt = receiver_buffer[receiver_next_seq_num]; 
        ASSERT(more_pkt != NULL);
        unsigned int more_seq_num = 0;
        memcpy(&more_seq_num, more_pkt->data + pointer_sequence_num, sizeof_sequence_num); ASSERT(more_seq_num == receiver_next_seq_num);
        short more_hash_value = 0;
        memcpy(&more_hash_value, more_pkt->data + pointer_hash_value, sizeof_hash_value);
        char more_payload_size = 0;
        memcpy(&more_payload_size, more_pkt->data + pointer_payload_size, sizeof_payload_size); 

        struct message *more_msg = (struct message*) malloc(sizeof(struct message));
        ASSERT(more_msg != NULL);
        more_msg->size = more_payload_size;

        more_msg->data = (char*) malloc(more_msg->size);
        ASSERT(more_msg->data != NULL);
        memcpy(more_msg->data, more_pkt->data + pointer_payload, more_payload_size);
        Receiver_ToUpperLayer(more_msg); // printf("Receiver: send packet[%d] to upper layer\n", more_seq_num);
        /* don't forget to free the space */
        if (more_msg->data != NULL) free(more_msg->data);
        if (more_msg != NULL) free(more_msg);

        receiver_next_seq_num++;
      };

      /* send back ACK */
      send_ack(receiver_next_seq_num - 1, 1);
    } 
    else if (hash_value == cal_hash_value && receiver_next_seq_num < seq_num) { // buffer it
      packet* buffer_pkt = (struct packet*) malloc(sizeof(struct packet));
      ASSERT(buffer_pkt);
      memcpy(buffer_pkt->data, pkt->data, sizeof(pkt->data));
      receiver_buffer[seq_num] = buffer_pkt;

      /* send back NACK */
      send_ack(receiver_next_seq_num, 0);
    }
    else send_ack(receiver_next_seq_num - 1, 1);

    receiver_mutex.unlock();
}