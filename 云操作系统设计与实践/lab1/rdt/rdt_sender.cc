/*
 * FILE: rdt_sender.cc
 * DESCRIPTION: Reliable data transfer sender.
 * NOTE: This implementation assumes there exists packet loss, corruption, or 
 *       reordering. In this implementation, the packet format is laid out as 
 *       the following:
 *       
 *       |<-  4 byte  ->|<-  2 byte  ->|<-  1 byte  ->|<-            the rest            ->|
 *       | sequence_num |  hash_value  | payload size |<-            payload             ->|
 *
 *       The first 4 bytes indicates the unique sequence number of each packet
 *       The following 2 bytes indicates hash value of the payload to handle with corruption
 *       The following 1 byte indicates the size of the payload
 *       The rest (<= 121) bytes are payload of each packet
 */


#include <cstring>
#include <deque>
#include <functional>
#include <iostream>
#include <mutex>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>

#include "rdt_struct.h"
#include "rdt_sender.h"


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

unsigned int sender_base = 0;
unsigned int sender_next_seq_num = 0;
std::deque<packet> sender_window = std::deque<packet>();

/* hash the data for checksum */
const std::hash<std::string> hash_fn;

std::mutex sender_mtx;

/* sender initialization, called once at the very beginning */
void Sender_Init()
{
    fprintf(stdout, "At %.2fs: sender initializing ...\n", GetSimulationTime());

    sender_base = 0;
    sender_next_seq_num = 0;
    sender_window.clear();
}

/* sender finalization, called once at the very end.
   you may find that you don't need it, in which case you can leave it blank.
   in certain cases, you might want to take this opportunity to release some 
   memory you allocated in Sender_init(). */
void Sender_Final()
{
    fprintf(stdout, "At %.2fs: sender finalizing ...\n", GetSimulationTime());
}

/* event handler, called when a message is passed from the upper layer at the 
   sender */
void Sender_FromUpperLayer(struct message *msg)
{
    sender_mtx.lock();

    /* maximum payload size */
    int maxpayload_size = RDT_PKTSIZE - header_size;

    /* reuse the same packet data structure */
    packet pkt;

    /* the cursor always points to the first unsent byte in the message */
    int cursor = 0;

    while (cursor < msg->size) {
        /* fill in the packet's seq_num, hash_value payload_size and data */
        char payload_size = std::min(maxpayload_size, msg->size - cursor);
        memcpy(pkt.data + pointer_payload_size, &payload_size, sizeof_payload_size); // payload size     

        memcpy(pkt.data + pointer_payload, msg->data + cursor, payload_size); // payload

        unsigned int seq_num = sender_window.size();
        memcpy(pkt.data + pointer_sequence_num, &seq_num, sizeof_sequence_num); // sequence number

        std::string data(msg->data + cursor, msg->data + cursor + payload_size);
        short hash_value = hash_fn(std::to_string(payload_size) + std::to_string(seq_num) + data) % hash_mod;
        memcpy(pkt.data + pointer_hash_value, &hash_value, sizeof_hash_value); // hash value

        /* throw it into sender_window */
        sender_window.push_back(pkt);

        // printf("Sender: packet[%d] size %d\n", seq_num, payload_size);

        /* move the cursor */
        cursor += maxpayload_size;
    };

    /* send first N packet */
    while (sender_next_seq_num < sender_base + window_size && sender_next_seq_num < sender_window.size()) {
        Sender_ToLowerLayer(&sender_window[sender_next_seq_num]); // printf("Sender: send packet[%d]\n", sender_next_seq_num);
        sender_next_seq_num++;
    };

    Sender_StartTimer(timeout);
    
    sender_mtx.unlock();
}

/* event handler, called when a packet is passed from the lower layer at the 
   sender */
void Sender_FromLowerLayer(struct packet *pkt)
{
    ASSERT(pkt != NULL);

    sender_mtx.lock();

    Sender_StartTimer(timeout);

    unsigned int ack_num = 0;
    short hash_value = 0;
    char is_ack = -1;
    memcpy(&ack_num, pkt->data + pointer_sequence_num, sizeof_sequence_num);
    memcpy(&hash_value, pkt->data + pointer_hash_value, sizeof_hash_value);
    memcpy(&is_ack, pkt->data + pointer_ack, sizeof_ack);
    // printf("Sender: get (n)ack %d sender_next_seq_num %d\n", ack_num, sender_next_seq_num);

    if (hash_value != hash_fn(std::to_string(ack_num) + std::to_string(is_ack)) % hash_mod) {
        // printf("Sender: (n)ack %d corrupted\n", ack_num);
    }
    else if (is_ack == 1) { // ack message
        sender_base = ack_num + 1;
        while (sender_next_seq_num < sender_base + window_size && sender_next_seq_num < sender_window.size()) {
            Sender_ToLowerLayer(&sender_window[sender_next_seq_num]); // printf("Sender: send packet[%d]\n", sender_next_seq_num);
            sender_next_seq_num++;
        };
    } else if (is_ack == 0) { // nack message
        unsigned int nack_num = ack_num;
        Sender_ToLowerLayer(&sender_window[nack_num]); // printf("Sender: resend packet[%d]\n", nack_num);
    } else {
        // printf("Sender: msg corrputed");
    };

    if (sender_base == sender_next_seq_num) 
        Sender_StopTimer();

    sender_mtx.unlock();
}

/* event handler, called when the timer expires */
void Sender_Timeout()
{
    sender_mtx.lock();

    Sender_StartTimer(timeout);

    /* resend all packet in the window */
    for (unsigned int i = sender_base; i < sender_next_seq_num; i++) {
        Sender_ToLowerLayer(&sender_window[i]); // printf("Sender: [timeout] send packet[%d]\n", sender_next_seq_num);
    };

    sender_mtx.unlock();
};
