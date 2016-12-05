#include <stdio.h>
#include <stdlib.h>

#include "haf_queue.h"
#include "mutex.h"

mutex_t mux;
haf_queue_t* queue;
int queue_count;

void haf_queue_init(void){
	mutex_init(&mux);
	
	queue = (haf_queue_t*) malloc(sizeof(haf_queue_t));
	
	queue->f = NULL;
	queue->next = NULL;
}

int haf_queue_enqueue(void* func){
	if (queue_count >= MAX_QUEUE_ELEMENTS){
		return -1;
	}
	
	mutex_lock(&mux);
	
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
	
	mutex_unlock(&mux);
	
	return 0;
}

void* haf_queue_dequeue(void){
	if (queue->f == NULL && queue->next == NULL){
		return NULL;
	}
	
	mutex_lock(&mux);
	
	void* ret_f = queue->f;
	
	if (queue->next != NULL){
		haf_queue_t* to_be_freed = queue;
		queue = queue->next;
		
		free(to_be_freed);
	} else {
		queue->f = NULL;
	}
	
	--queue_count;
	
	mutex_unlock(&mux);
	
	return ret_f;
}
