#include <stdio.h>
#include <stdlib.h>

#include "global.h"
#include "haf_queue.h"
#include "mutex.h"

mutex_t queue_mutex;
mutex_t blocking_wait_mutex;
haf_queue_t* queue;
int queue_count;

void haf_queue_init(void){
	mutex_init(&queue_mutex);
	
	queue = (haf_queue_t*) malloc(sizeof(haf_queue_t));
	
	queue->f = NULL;
	queue->next = NULL;
	
	mutex_lock(&blocking_wait_mutex);
}

int haf_queue_enqueue(thread_function_t func){
	if (queue_count >= MAX_QUEUE_ELEMENTS){
		return -1;
	}
	
	mutex_lock(&queue_mutex);
	
	if (queue->f == NULL && queue->next == NULL){
		queue->f = func;
	} else {
		haf_queue_t* next_element = (haf_queue_t*) malloc(sizeof(haf_queue_t));
		
		next_element->f = func;
		next_element->next = NULL;
		
		int i;
		haf_queue_t* main_queue = queue;
		for(i = 0; i < MAX_QUEUE_ELEMENTS; ++i){
			if (main_queue->next == NULL){
				main_queue->next = next_element;
				break;
			}
		}
	}
	
	++queue_count;
	
	mutex_unlock(&blocking_wait_mutex);
	mutex_unlock(&queue_mutex);
	
	return 0;
}

thread_function_t haf_queue_dequeue(void){
	mutex_lock(&blocking_wait_mutex);
	mutex_unlock(&blocking_wait_mutex);
	
	mutex_lock(&queue_mutex);
	
	thread_function_t ret_f = queue->f;
	
	if (queue->next != NULL){
		haf_queue_t* to_be_freed = queue;
		queue = queue->next;
		
		free(to_be_freed);
	} else {
		queue->f = NULL;
	}
	
	--queue_count;
	
	if (queue_count == 0){
		mutex_lock(&blocking_wait_mutex);
	}
	
	mutex_unlock(&queue_mutex);
	
	return ret_f;
}
