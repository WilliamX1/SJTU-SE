/*
 * FILE: rdt_sim.cc
 * DESCRIPTION: The main simulation control module for reliable data transfer.
 * NOTE: You are not supposed to change this file.  You can, however, add some
 *       printouts to help you debugging.  But remember to test it with the 
 *       original version before you turn in your programs.
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <unistd.h>

#include "rdt_struct.h"
#include "rdt_sender.h"
#include "rdt_receiver.h"


/*[]------------------------------------------------------------------------[]
  |  generic event chain framework
  []------------------------------------------------------------------------[]*/

/* simulation event base class */
class Event
{
public:
    double sched_time;      /* scheduled occuring time */
    int event_type;         /* application-specific event type */
    class Event *next;      /* next event in the chain */

public:
    Event() { next = NULL; }
};

/* event chain class - the simulation core */
class EventChain
{
public:
    double sim_time;        /* simulation time */
    Event *head;            /* head event in the chain */

public:
    EventChain() {
	sim_time = 0;
	head = NULL;
    }
    
    double time() { return sim_time; }
    
    /* schedule an event - the event chain is maintained on an increasing order 
       of sched_time */
    void schedule(Event *e) {
	/* do nothing if the event is schedule for the past */
	if (e->sched_time<sim_time) return;

	Event **ppcur = &head;
	while ((*ppcur!=NULL) && ((*ppcur)->sched_time<=e->sched_time))
	    ppcur = &((*ppcur)->next);

	e->next = *ppcur;
	*ppcur = e;
    }

    /* cancel an event scheduled for happening in the future */
    void cancel(Event *e) {
	Event **ppcur = &head;
	while ((*ppcur!=NULL) && (*ppcur!=e))
	    ppcur = &((*ppcur)->next);

	if (*ppcur==e) *ppcur=(*ppcur)->next;
    }

    /* advance to the next event */
    Event *next_event() {
	if (head==NULL) return NULL;

	Event *e = head;
	head = head->next;
	sim_time = e->sched_time;

	return e;
    }
};


/*[]------------------------------------------------------------------------[]
  |  event definitions
  []------------------------------------------------------------------------[]*/

enum {EVENT_SENDER_FROMUPPERLAYER=0, EVENT_SENDER_FROMLOWERLAYER, 
      EVENT_SENDER_TIMEOUT, EVENT_RECEIVER_FROMLOWERLAYER};

/* the event that the upper layer at the sender instructs rdt layer to send out 
   a message */
class EventSenderFromUpperLayer : public Event
{
public:
    EventSenderFromUpperLayer() { event_type = EVENT_SENDER_FROMUPPERLAYER; }
};

/* the event that the lower layer at the sender informs the rdt layer that a 
   packet is received from the link */
class EventSenderFromLowerLayer : public Event
{
public:
    struct packet pkt;
public:
    EventSenderFromLowerLayer() { event_type = EVENT_SENDER_FROMLOWERLAYER; }
};

/* the event that the timer at the sender expires */
class EventSenderTimeout : public Event
{
public:
    EventSenderTimeout() { event_type = EVENT_SENDER_TIMEOUT; }
};

/* the event that the lower layer at the receiver informs the rdt layer that a 
   packet is received from the link */
class EventReceiverFromLowerLayer : public Event
{
public:
    struct packet pkt;
public:
    EventReceiverFromLowerLayer() { event_type = EVENT_RECEIVER_FROMLOWERLAYER; }
};


/*[]------------------------------------------------------------------------[]
  |  gloabal variables, statistics, etc.
  []------------------------------------------------------------------------[]*/

/* total simulation time, the simulation will end at this time (in seconds) */
double sim_time;

/* average intervals between consecutive messages passed from the upper layer 
   at the sender (in seconds) */
double msg_arrivalint;

/* average size of messages (in bytes) */
int msg_size;

/* average one-way packet delivery latency, set to be 100ms */
const double pkt_latency = 0.1;

/* the probability that a packet is not delivered with the normal latency:
   a value of 0.1 means that one in ten packets are not delivered with the 
   normal latency */
double outoforder_rate;

/* packet loss probability: a value of 0.1 means that one in ten packets are
   lost on average */
double loss_rate;

/* packet corruption probability: a value of 0.1 means that one in ten packets
   (excluding those lost) are corrupted on average.  note that any part of the 
   packet can be corrupted */
double corrupt_rate;

/* tracing levels (higher level always prints out more information):
   a tracing level of 0 turns off all traces while a tracing, 
   a tracing level of 1 turns on regular traces,
   a tracing level of 2 prints out the delivered message
*/
int tracing_level;

/* simulation event chain core */
EventChain sim_core;

/* sender timer event */
Event *sender_timer = NULL;

/* general statistics */
int tot_chars_sent = 0;
int tot_chars_delivered = 0;
int tot_pkts_passed = 0;

/* error flag set by message verification at the receiver */
bool message_verfication_passed = true;


/*[]------------------------------------------------------------------------[]
  |  simulation routines
  []------------------------------------------------------------------------[]*/

/* generate a random number in [0,1] */
static double myrandom()
{
    return(rand()*1.0/RAND_MAX);
}

/* generate a message 
   NOTE: change this part if you want to generate different messages for 
         testing.  we will certainly use different messages in our grading! */
static struct message *generate_msg()
{
    static char cnt = 0;

    struct message *msg = (struct message*) malloc(sizeof(struct message));
    ASSERT(msg!=NULL);
    msg->size = (int)(myrandom()*2.0*msg_size);
    if (msg->size==0) msg->size=1;
    msg->data = (char*) malloc(msg->size);
    ASSERT(msg->data!=NULL);

    for (int i=0; i<msg->size; i+=1) {
	msg->data[i] = '0' + cnt;
	cnt = (cnt+1) % 10;
    }

    tot_chars_sent += msg->size;

    return msg;
}

/* free the space of a message */
static void free_msg(struct message *msg)
{
    if (msg->data!=NULL) free(msg->data);
    if (msg!=NULL) free(msg);
}

/* get simulation time (in seconds) - for both the sender and the receiver */
double GetSimulationTime()
{
    return sim_core.time();
}

/* start the sender timer with a specified timeout (in seconds).
   the timer is cancelled with Sender_StopTimer() is called or a new 
   Sender_StartTimer() is called before the current timer expires.
   Sender_Timeout() will be called when the timer expires. */
void Sender_StartTimer(double timeout)
{
    if (tracing_level>=1)
	fprintf(stdout, "Time %.2fs (Sender): the timer is started (expires at %.2fs).\n",
		sim_core.time(), sim_core.time() + timeout);

    if (sender_timer!=NULL) {
	sim_core.cancel(sender_timer);
	delete sender_timer;
	sender_timer = NULL;
    }

    EventSenderTimeout *e = new EventSenderTimeout;
    e->sched_time = sim_core.time() + timeout;
    sim_core.schedule(e);

    sender_timer = e;
}

/* stop the sender timer */
void Sender_StopTimer()
{
    if (tracing_level>=1)
	fprintf(stdout, "Time %.2fs (Sender): the timer is stopped.\n", 
		sim_core.time());

    if (sender_timer!=NULL) {
	sim_core.cancel(sender_timer);
	delete sender_timer;
	sender_timer = NULL;
    }
}

/* check whether the sender timer is being set,
   return true if the timer is set, return false otherwise */
bool Sender_isTimerSet()
{
    return (sender_timer!=NULL);
}

/* pass a packet to the lower layer at the sender */
void Sender_ToLowerLayer(struct packet *pkt)
{
    /* packet lost at rate "loss_rate" */
    if (myrandom()<loss_rate) return;

    EventReceiverFromLowerLayer *e = new EventReceiverFromLowerLayer;
    memcpy(&e->pkt.data, pkt->data, RDT_PKTSIZE);

    /* packet corrupted at rate "corrupt_rate" */
    if (myrandom()<corrupt_rate) {
	for (int i=0; i<RDT_PKTSIZE; i++) {
	    e->pkt.data[i] = e->pkt.data[i] + (char)(myrandom()*20) - 10;
	}
    }

    /* schedule the packet arrival event at the other side */
    if (myrandom()<outoforder_rate)
	e->sched_time = sim_core.time() + pkt_latency*2.0*myrandom();
    else
	e->sched_time = sim_core.time() + pkt_latency;
    sim_core.schedule(e);

    tot_pkts_passed ++;
}


/* pass a packet to the lower layer at the receiver */
void Receiver_ToLowerLayer(struct packet *pkt)
{
    /* packet lost at rate "loss_rate" */
    if (myrandom()<loss_rate) return;

    EventSenderFromLowerLayer *e = new EventSenderFromLowerLayer;
    memcpy(&e->pkt.data, pkt->data, RDT_PKTSIZE);

    /* packet corrupted at rate "corrupt_rate" */
    if (myrandom()<corrupt_rate) {
	for (int i=0; i<RDT_PKTSIZE; i++) {
	    e->pkt.data[i] = e->pkt.data[i] + (char)(myrandom()*20) - 10;
	}
    }

    /* schedule the packet arrival event at the other side */
    if (myrandom()<outoforder_rate)
	e->sched_time = sim_core.time() + pkt_latency*2.0*myrandom();
    else
	e->sched_time = sim_core.time() + pkt_latency;	
    sim_core.schedule(e);

    tot_pkts_passed ++;
}

/* deliver a message to the upper layer at the receiver 
   NOTE: change the message verification in this function if you changed 
         generate_msg() for testing. */
void Receiver_ToUpperLayer(struct message *msg)
{
    static char cnt = 0;

    for (int i=0; i<msg->size; i++) {
	/* message verification */
	if (msg->data[i] != '0' + cnt) {
	    message_verfication_passed = false;
	}
	cnt = (cnt+1) % 10;

	if (tracing_level>=2)
	    fputc(msg->data[i], stdout);
    }

    tot_chars_delivered += msg->size;
}


/*[]------------------------------------------------------------------------[]
  |  main simulation control routine
  []------------------------------------------------------------------------[]*/

int main(int argc, char *argv[])
{
    if (argc!=8) {
	fprintf(stderr, "usage: %s <sim_time> <mean_msg_arrivalint> <mean_msg_size> "
		"<outoforder_rate> <loss_rate> <corrupt_rate> <tracing_level>\n", 
		argv[0]);
	exit(-1);
    }

    sim_time = atof(argv[1]);
    if (sim_time<=0) {
	fprintf(stderr, "invalid <sim_time>\n");
	exit(-1);
    }
    msg_arrivalint = atof(argv[2]);
    if (msg_arrivalint<=0) {
	fprintf(stderr, "invalid <msg_arrivalint>\n");
	exit(-1);
    }
    msg_size = atoi(argv[3]);
    if (msg_size<=0) {
	fprintf(stderr, "invalid <msg_size>\n");
	exit(-1);
    }
    outoforder_rate = atof(argv[4]);
    if (outoforder_rate<0 || outoforder_rate>1) {
	fprintf(stderr, "invalid <outoforder_rate>\n");
	exit(-1);
    }
    loss_rate = atof(argv[5]);
    if (loss_rate<0 || loss_rate>1) {
	fprintf(stderr, "invalid <loss_rate>\n");
	exit(-1);
    }
    corrupt_rate = atof(argv[6]);
    if (corrupt_rate<0 || corrupt_rate>1) {
	fprintf(stderr, "invalid <corrupt_rate>\n");
	exit(-1);
    }
    tracing_level = atoi(argv[7]);
    if (tracing_level<0 || tracing_level>2) {
	fprintf(stderr, "invalid <tracing_level>\n");
	exit(-1);
    }
    
    fprintf(stdout, "## Reliable data transfer simulation with:\n"
	    "\tsimulation time is %.3f seconds\n"
	    "\taverage message arrival interval is %.3f seconds\n"
	    "\taverage message size is %d bytes\n"
	    "\taverage out-of-order delivery rate is %.2f%%\n"
	    "\taverage loss rate is %.2f%%\n"
	    "\taverage corrupt rate is %.2f%%\n"
	    "\ttracing level is %d\n"
	    "Please review these inputs and press <enter> to proceed.\n",
	    sim_time, msg_arrivalint, msg_size, outoforder_rate*100.0, 
	    loss_rate*100.0, corrupt_rate*100.0, tracing_level);
    fgetc(stdin);

    /* initialize the random number generator */
    srand(getpid()+getppid());

    /* test the random number generator */
    double randtest_sum = 0.0;
    for (int i=0; i<1000; i++)
	randtest_sum += myrandom();
    double randtest_avg = randtest_sum/1000;
    if (randtest_avg<0.25 || randtest_avg>0.75) {
	fprintf(stderr, 
		"It appears that something is wrong with the random number.\n"
		"Please try to run this again.\n"  
		"Please report to me if the problem PERSISTS.\n");
	exit(-1);
    }

    /* intialize the sender and the receiver */
    Sender_Init();
    Receiver_Init();

    /* scheduling a recurring message arrival event */
    EventSenderFromUpperLayer *e = new EventSenderFromUpperLayer;
    e->sched_time = 0;
    sim_core.schedule(e);

    /* main simulation cycle */
    for (;;) {
	Event *e = sim_core.next_event();
	if (e==NULL) break;

	switch (e->event_type) {
	case EVENT_SENDER_FROMUPPERLAYER:
	    {
		if (tracing_level>=1) {
		    fprintf(stdout, "Time %.2fs (Sender): the upper layer instructs rdt layer to send out a message.\n", sim_core.time());
		}

		EventSenderFromUpperLayer *real_e = (EventSenderFromUpperLayer*) e;

		struct message *msg = generate_msg();
		Sender_FromUpperLayer(msg);
		free_msg(msg);

		/* schedule the recurring event */
		if (sim_core.time() < sim_time) {
		    real_e->sched_time = 
			sim_core.time() + msg_arrivalint*2.0*myrandom();
		    sim_core.schedule(real_e);
		}
		else
		    delete real_e;
	    }
	    break;

	case EVENT_SENDER_FROMLOWERLAYER:
	    {
		if (tracing_level>=1) {
		    fprintf(stdout, "Time %.2fs (Sender): the lower layer informs the rdt layer that a packet is received from the link.\n", sim_core.time());
		}

		EventSenderFromLowerLayer *real_e = (EventSenderFromLowerLayer*) e;

		Sender_FromLowerLayer(&real_e->pkt);

		delete real_e;
	    }
	    break;

	case EVENT_SENDER_TIMEOUT:
	    {
		if (tracing_level>=1) {
		    fprintf(stdout, "Time %.2fs (Sender): the timer expires.\n", sim_core.time());
		}

		EventSenderTimeout *real_e = (EventSenderTimeout*) e;
		delete real_e;
		sender_timer = NULL;

		Sender_Timeout();
	    }
	    break;

	case EVENT_RECEIVER_FROMLOWERLAYER:
	    {
		if (tracing_level>=1) {
		    fprintf(stdout, "Time %.2fs (Receiver): the lower layer informs the rdt layer that a packet is received from the link.\n", sim_core.time());
		}

		EventReceiverFromLowerLayer *real_e = (EventReceiverFromLowerLayer*) e;
		
		Receiver_FromLowerLayer(&real_e->pkt);

		delete real_e;
	    }
	    break;

	default:
	    fprintf(stderr, "undefined event %d\n", e->event_type);
	    break;
	}
    }

    /* finalize the sender and the receiver */
    Sender_Final();
    Receiver_Final();

    fprintf(stdout, "\n");
    fprintf(stdout, "## Simulation completed at time %.2fs with\n" 
	    "\t%d characters sent\n" 
	    "\t%d characters delivered\n"
	    "\t%d packets passed between the sender and the receiver\n", 
	    sim_core.time(), tot_chars_sent, tot_chars_delivered, tot_pkts_passed);

    if (message_verfication_passed && (tot_chars_sent==tot_chars_delivered))
	fprintf(stdout, "## Congratulations! This session is error-free, loss-free, and in order.\n");
    else
	fprintf(stdout, "## Something is wrong! This session is NOT error-free, loss-free, and in order.\n");

    return 0;
}
