#ifndef HAF_QUEUE_H
#define HAF_QUEUE_H

#define MAX_QUEUE_ELEMENTS 10

typedef struct haf_queue {
	void* f;
	struct haf_queue* next;
} haf_queue_t;

void haf_queue_init(void);
int haf_queue_enqueue(void* func);
void* haf_queue_dequeue(void);

#endif /* HAF_QUEUE_H */
