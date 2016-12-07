#include <stdio.h>

#include "global.h"
#include "thread.h"
#include "haf_thread.h"
#include "haf_queue.h"

#define HAF_THREAD_PRIORITY THREAD_PRIORITY_MAIN + 1

char haf_thread_stack[THREAD_STACKSIZE_MAIN];

void _haf_thread_handler(void) {	
	while (1){
		thread_function_t func = haf_queue_dequeue();
		
#ifdef HAF_DEBUG
		puts("thread function enqueued!");
#endif /* HAF_DEBUG */
		
		func();
	}
}

kernel_pid_t haf_thread_create(void){
	haf_queue_init();
	return thread_create(	haf_thread_stack, sizeof(haf_thread_stack), 
							HAF_THREAD_PRIORITY, THREAD_CREATE_STACKTEST, 
							(void *) _haf_thread_handler, NULL, "HAF_handler_thread");
}
