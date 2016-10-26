#include <stdio.h>

#include "thread.h"
#include "xtimer.h"

#include "heartbeat_handler.h"

#define HEARTBEAT_TIMEOUT_USEC	5000000

xtimer_t timer;
static char stack[THREAD_STACKSIZE_DEFAULT];

void _timerTask(void) {
	puts("HEARTBEAT TIMEOUT!");
	//TODO call heartbeat_timeout_handler
}

void* _heartbeat_handler(void *args) {
	xtimer_set(&timer, HEARTBEAT_TIMEOUT_USEC);
	return NULL;
}

void heartbeat_handler_init(void) {
	xtimer_init();
    timer.target = 0;
    timer.long_target = 0;
    timer.callback = (void*)_timerTask;
}

int heartbeat_handler_start(void) {
	if (thread_create(stack, THREAD_STACKSIZE_DEFAULT, THREAD_PRIORITY_MAIN - 1, THREAD_CREATE_STACKTEST, 
						_heartbeat_handler, NULL, "heartbeat_handler") <= KERNEL_PID_UNDEF) {
		return -1;
	}	
	return 0;
}
