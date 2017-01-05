#ifndef HAF_QUEUE_H
#define HAF_QUEUE_H

#define MAX_QUEUE_ELEMENTS 10

typedef void (*thread_function_t)(void);

typedef struct haf_queue {
	thread_function_t f;
	struct haf_queue* next;
} haf_queue_t;

/* 
 * Initializes haf queue
 */
void haf_queue_init(void);

/* 
 * Enqueues function in queue
 */
int haf_queue_enqueue(thread_function_t func);

/* 
 * Dequeues from queue
 */
thread_function_t haf_queue_dequeue(void);

#endif /* HAF_QUEUE_H */
