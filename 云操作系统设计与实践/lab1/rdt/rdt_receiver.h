/*
 * FILE: rdt_receiver.h
 * DESCRIPTION: The header file for reliable data transfer receiver.
 * NOTE: Do not touch this file!
 */


#ifndef _RDT_RECEIVER_H_
#define _RDT_RECEIVER_H_

#include "rdt_struct.h"


/*[]------------------------------------------------------------------------[]
  |  routines that you can call
  []------------------------------------------------------------------------[]*/

/* get simulation time (in seconds) */
double GetSimulationTime();

/* pass a packet to the lower layer at the receiver */
void Receiver_ToLowerLayer(struct packet *pkt);

/* deliver a message to the upper layer at the receiver */
void Receiver_ToUpperLayer(struct message *msg);


/*[]------------------------------------------------------------------------[]
  |  routines to be changed/enhanced by you
  []------------------------------------------------------------------------[]*/

/* receiver initialization, called once at the very beginning.
   this routine is here to help you.  leave it blank if you don't need it.*/
void Receiver_Init();

/* receiver finalization, called once at the very end.
   this routine is here to help you.  leave it blank if you don't need it.
   in certain cases, you might want to use this opportunity to release some 
   memory you allocated in Receiver_init(). */
void Receiver_Final();

/* event handler, called when a packet is passed from the lower layer at the 
   receiver */
void Receiver_FromLowerLayer(struct packet *pkt);

#endif  /* _RDT_RECEIVER_H_ */
