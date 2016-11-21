#include <stdio.h>

#include "thread.h"
#include "xtimer.h"

#include "heartbeat.h"
#include "HAF_protocol.h"
#include "connection.h"
#include "localization_request.h"
#include "global.h"
#include "net/ipv6/addr.h"

#define HEARTBEAT_TIMEOUT_USEC	4000000
#define HEARTBEAT_TIME_USEC		2000000

xtimer_t timer_recv;
xtimer_t timer_send;

void _heartbeat_handler_Task(void) {
#ifdef HAF_DEBUG
	puts("HEARTBEAT TIMEOUT!");
#endif
	send_localization_request();
	xtimer_set(&timer_recv, HEARTBEAT_TIMEOUT_USEC);
}

void _heartbeat_handler(void) {
	//xtimer_remove(&timer_recv);
	xtimer_set(&timer_recv, HEARTBEAT_TIMEOUT_USEC);
}

void heartbeat_handler_init(void) {
	xtimer_init();
    timer_recv.target = 0;
    timer_recv.long_target = 0;
    timer_recv.callback = (void*)_heartbeat_handler_Task;
	xtimer_set(&timer_recv, HEARTBEAT_TIMEOUT_USEC);
}

void handle_heartbeat(void) {
	_heartbeat_handler();
}

void _heartbeat_sender_Task(void) {	
    ipv6_addr_t d = IPV6_ADDR_UNSPECIFIED;
	ipv6_addr_from_str(&d, MONITORED_ITEM_IP);
	
	heartbeat_t ret_pkg;
	ret_pkg.type = HEARTBEAT;
	udp_send(&ret_pkg, sizeof(ret_pkg), &d);
	xtimer_set(&timer_send, HEARTBEAT_TIME_USEC);
#ifdef HAF_DEBUG
	puts("HEARTBEAT sent.");
#endif
}

void heartbeat_sender_start(void) {
	xtimer_init();
    timer_send.target = 0;
    timer_send.long_target = 0;
    timer_send.callback = (void*)_heartbeat_sender_Task;
	xtimer_set(&timer_send, HEARTBEAT_TIME_USEC);
}

void heartbeat_sender_stop(void) {
	xtimer_remove(&timer_send);
}
