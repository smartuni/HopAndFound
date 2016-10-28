#include <stdio.h>

#include "thread.h"
#include "xtimer.h"

#include "heartbeat.h"
#include "../../HAF_protocol.h"

#define HEARTBEAT_TIMEOUT_USEC	3000000
#define HEARTBEAT_TIME_USEC		1000000

xtimer_t timer_recv;
xtimer_t timer_send;
static char stack[THREAD_STACKSIZE_DEFAULT];

void _heartbeat_handler_Task(void) {
	puts("HEARTBEAT TIMEOUT!");
	//TODO call heartbeat_timeout_handler
}

void* _heartbeat_handler(void *args) {
	xtimer_set(&timer_recv, HEARTBEAT_TIMEOUT_USEC);
	return NULL;
}

void heartbeat_handler_init(void) {
	xtimer_init();
    timer_recv.target = 0;
    timer_recv.long_target = 0;
    timer_recv.callback = (void*)_heartbeat_handler_Task;
	xtimer_set(&timer_recv, HEARTBEAT_TIMEOUT_USEC);
}

int handle_heartbeat(void) {
	if (thread_create(stack, THREAD_STACKSIZE_DEFAULT, THREAD_PRIORITY_MAIN - 1, THREAD_CREATE_STACKTEST, 
						_heartbeat_handler, NULL, "heartbeat_handler") <= KERNEL_PID_UNDEF) {
		return -1;
	}	
	return 0;
}

void _heartbeat_sender_Task(void) {
	//TODO send heartbeat
	xtimer_set(&timer_send, HEARTBEAT_TIME_USEC);
	puts("HEARTBEAT sent.");
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
