#include <stdio.h>
#include <string.h>

#include "haf_queue.h"
#include "xtimer.h"

int i = 5;

int main(void){
	xtimer_sleep(2);
	
	printf("Queue Test start..\n");
	
	int j;
	int* p;
	
	haf_queue_init();
	
	printf("Queue initialized. SUCCESS!\n");
	
	if (haf_queue_dequeue() == NULL){
		printf("Dequeue empty queue returns NULL. SUCCESS!\n");
	} else {
		printf("Dequeue empty queue returns NOT NULL. ABORT! Line: %d\n", __LINE__);
		return 0;
	}
	
	if (haf_queue_enqueue((void *)&i) == 0){
		printf("Enqueue object returns 0. SUCCESS!\n");
	} else {
		printf("Enqueue object returns -1. ABORT! Line: %d\n", __LINE__);
		return 0;
	}
	
	p = (int *)haf_queue_dequeue();
	
	if (p != NULL){
		printf("Dequeue object returns %d. SUCCESS!\n", *p);
	} else {
		printf("Dequeue object returns NULL. ABORT! Line: %d\n", __LINE__);
		return 0;
	}
	
	for (j = 0; j < MAX_QUEUE_ELEMENTS; ++j){
		if (haf_queue_enqueue((void *)&i) == -1){
			printf("Enqueue object returns -1 in loop, j=%d. ABORT! Line: %d\n", j, __LINE__);
			return 0;
		}
	}
	
	if (haf_queue_enqueue((void *)&i) == -1){
		printf("Enqueue too many objects returns -1. SUCCESS!\n");
	} else {
		printf("Enqueue too many objects returns 0. ABORT! Line: %d\n", __LINE__);
		return 0;
	}
	
	printf("Queue Test finished successfully!\n");
	
	return 0;
}
