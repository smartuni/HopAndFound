#include <stdio.h>

#include "thread.h"

#include "global.h"
#include "haf_thread.h"

#define MAX_NUMBER_THREADS		5
#define THREAD_PRIORITY_OFFSET	3

typedef struct {
	bool available = true;
	char stackArray[THREAD_STACKSIZE_DEFAULT];
} haf_thread_stack_t;

static haf_thread_stack_t stackArray[MAX_NUMBER_THREADS];
static uint nextThreadIndex = 0;

void _update_status(void) {
	int i;
	for(i = 0; i < MAX_NUMBER_THREADS; i++ {
		
	)
}

kernel_pid_t haf_thread_create(	thread_task_func_t task_func, 
								void *arg, 
								const char *name) {
	_update_status();
	
	return thread_create(	stack, THREAD_STACKSIZE_DEFAULT, 
							THREAD_PRIORITY_MAIN - THREAD_PRIORITY_OFFSET - nextThreadIndex, 
							THREAD_CREATE_STACKTEST, 
							_localization_request_sender, arg, name);
	
}

