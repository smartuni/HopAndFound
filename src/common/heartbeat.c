#include <stdio.h>

#include "xtimer.h"

#include "heartbeat.h"
#include "HAF_protocol.h"
#include "localization_request.h"
#include "global.h"
#include "net/ipv6/addr.h"
#include "haf_LED.h"
#include "thread.h"
#include "connection.h"
#include "haf_queue.h"


#define HEARTBEAT_TIMEOUT_USEC	4000000
#define HEARTBEAT_TIME_USEC		2000000


//Timer to receive determine heartbeat timeouts
xtimer_t timer_recv;
//Timer to periodically send heartbeats
xtimer_t timer_send;



//Stack for thread to send after timer_send interrupts
char heartbeat_send_stack[THREAD_STACKSIZE_MAIN];
//Stack for thread to handle heartbeat timeout after timer_recv interrupts	
char heartbeat_timeout_stack[THREAD_STACKSIZE_MAIN];

ipv6_addr_t monitoredItemIP;


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
	if (haf_queue_enqueue((thread_function_t) _heartbeat_timeout_task) == -1){
#ifdef HAF_DEBUG
		printf("_heartbeat_timeout_handler:: too many elements in thread queue.\n");
#endif /* HAF_DEBUG */
	}
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

	//ipv6_addr_t d;
	//ipv6_addr_from_str(&d, MONITORED_ITEM_IP);
	//ipv6_addr_from_str(&d, monitoredItemIP);
	
	heartbeat_t ret_pkg;
	ret_pkg.type = HEARTBEAT;
	udp_send(&ret_pkg, sizeof(ret_pkg), &monitoredItemIP);
	

#ifdef HAF_DEBUG
	puts("HEARTBEAT sent.");
#endif /* HAF_DEBUG */

#ifdef TEST_PRESENTATION
	}
#endif /* TEST_PRESENTATION */

	xtimer_set(&timer_send, HEARTBEAT_TIME_USEC);
}

void _heartbeat_send_handler(void){
	if (haf_queue_enqueue((thread_function_t) _heartbeat_send_task) == -1){
#ifdef HAF_DEBUG
		printf("_heartbeat_send_handler:: too many elements in thread queue.\n");
#endif /* HAF_DEBUG */
	}
}

void heartbeat_sender_init(ipv6_addr_t* miIP) {
	puts("hearbeat_sender_init start");
	heartbeatActive = true;
    timer_send.target = 0;
    timer_send.long_target = 0;
	//monitoredItemIP = miIP;
	puts("hearbeat_sender_init before memcpy");
	memcpy(&monitoredItemIP,miIP,sizeof(ipv6_addr_t));
	puts("hearbeat_sender_init after memcpy");
    timer_send.callback = (void*) _heartbeat_send_handler;
	xtimer_set(&timer_send, HEARTBEAT_TIME_USEC);
	puts("hearbeat_sender_init end");
}

void heartbeat_sender_stop(void) {
	xtimer_remove(&timer_send);
}
