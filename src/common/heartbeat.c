#include <stdio.h>

#include "xtimer.h"

#include "heartbeat.h"
#include "HAF_protocol.h"
#include "localization_request.h"
#include "global.h"
#include "net/ipv6/addr.h"
#include "haf_LED.h"
#include "thread.h"

#ifdef HAF_USE_SOCK_UDP
#include "connection_sock.h"
#else
#include "connection.h"
#endif


#define HEARTBEAT_TIMEOUT_USEC	4000000
#define HEARTBEAT_TIME_USEC		2000000


//Priority of thread for sending heartbeats
#define HEARTBEAT_SEND_THREAD_PRIORITY THREAD_PRIORITY_MAIN + 1
//Priority of thread for handling heartbeat timeouts
#define HEARTBEAT_TIMEOUT_THREAD_PRIORITY THREAD_PRIORITY_MAIN + 1


//Timer to receive determine heartbeat timeouts
xtimer_t timer_recv;
//Timer to periodically send heartbeats
xtimer_t timer_send;


//Stack for thread to send after timer_send interrupts
char heartbeat_send_stack[THREAD_STACKSIZE_MAIN];
//Stack for thread to handle heartbeat timeout after timer_recv interrupts
char heartbeat_timeout_stack[THREAD_STACKSIZE_MAIN];



bool heartbeatActive;

bool getHeartbeatActive(void) {
	return heartbeatActive;
}

void setHeartbeatActive(bool status) {
	heartbeatActive = status;
}



/*
 * HEARTBEAT TIMEOUT AND RECEIVE (MONITORED_ITEM)
 */

void _heartbeat_timeout_task(void) {
#ifdef HAF_DEBUG
	puts("HEARTBEAT TIMEOUT!");
#endif /* HAF_DEBUG */

#ifdef TEST_PRESENTATION
	start_LED_blink(LED_RED, 3);
#endif /* TEST_PRESENTATION */

	send_localization_request();
	xtimer_set(&timer_recv, HEARTBEAT_TIMEOUT_USEC);
}

void _heartbeat_timeout_handler(void){
	thread_create(heartbeat_timeout_stack, sizeof(heartbeat_timeout_stack),
			HEARTBEAT_TIMEOUT_THREAD_PRIORITY, THREAD_CREATE_STACKTEST,
			(void *) _heartbeat_timeout_task, NULL, "heartbeat_timeout_handler");
}

void heartbeat_timeout_init(void) {
    timer_recv.target = 0;
    timer_recv.long_target = 0;
    timer_recv.callback = (void*) _heartbeat_timeout_handler;
	xtimer_set(&timer_recv, HEARTBEAT_TIMEOUT_USEC);
}

void handle_heartbeat(void) {
	xtimer_set(&timer_recv, HEARTBEAT_TIMEOUT_USEC);
}



/*
 * HEARTBEAT SENDER (MONITOR)
 */

void _heartbeat_send_task(void) {
#ifdef TEST_PRESENTATION
	if(heartbeatActive) {
#endif /* TEST_PRESENTATION */

	ipv6_addr_t d;
	ipv6_addr_from_str(&d, MONITORED_ITEM_IP);

	heartbeat_t ret_pkg;
	ret_pkg.type = HEARTBEAT;
	udp_send(&ret_pkg, sizeof(ret_pkg), &d);

#ifdef HAF_DEBUG
	puts("HEARTBEAT sent.");
#endif /* HAF_DEBUG */

#ifdef TEST_PRESENTATION
	}
#endif /* TEST_PRESENTATION */

	xtimer_set(&timer_send, HEARTBEAT_TIME_USEC);
}

void _heartbeat_send_handler(void){
	thread_create(heartbeat_send_stack, sizeof(heartbeat_send_stack),
			HEARTBEAT_SEND_THREAD_PRIORITY, THREAD_CREATE_STACKTEST,
			(void *) _heartbeat_send_task, NULL, "heartbeat_send_handler");
}

void heartbeat_sender_init(void) {
	heartbeatActive = true;
    timer_send.target = 0;
    timer_send.long_target = 0;
    timer_send.callback = (void*) _heartbeat_send_handler;
	xtimer_set(&timer_send, HEARTBEAT_TIME_USEC);
}

void heartbeat_sender_stop(void) {
	xtimer_remove(&timer_send);
}
