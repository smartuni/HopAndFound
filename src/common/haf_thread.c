#include "thread.h"
#include "haf_thread.h"

#define THREAD_PRIORITY THREAD_PRIORITY_MAIN + 1

char thread_stack[THREAD_STACKSIZE_MAIN];

void _thread_handler(void) {
	/*
		while(1)
			msg receive
			do shit
	*/
}

kernel_pid_t haf_thread_create(void){
	return thread_create(	thread_stack, sizeof(thread_stack), 
							THREAD_PRIORITY, THREAD_CREATE_STACKTEST, 
							(void *) _thread_handler, NULL, "HAF_handler_thread");
}
