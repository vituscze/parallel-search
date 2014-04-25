#include <pthread.h>
#include <stdlib.h>

#include "checked.h"
#include "queue.h"

struct queue
alloc_queue(int capacity)
{
	struct queue q = {
	    NULL,
	    0,
	    0,
	    0,
	    PTHREAD_MUTEX_INITIALIZER,
	    PTHREAD_COND_INITIALIZER,
	    PTHREAD_COND_INITIALIZER
	    };
	q.capacity = capacity;
	q.data = checked_malloc(sizeof (void *) * capacity);
	return (q);
}

void
free_queue(struct queue *q)
{
	free(q->data);
	checked_mutex_destroy(&q->queue_lock);
	checked_cond_destroy(&q->empty);
	checked_cond_destroy(&q->full);
}

void
enqueue(void *elem, struct queue *q)
{
	checked_lock(&q->queue_lock);
	while (q->size == q->capacity) {
		checked_wait(&q->full, &q->queue_lock);
	}
	q->data[(q->start + q->size++) % q->capacity] = elem;
	checked_broadcast(&q->empty);
	checked_unlock(&q->queue_lock);
}

void *
dequeue(struct queue *q)
{
	void *res;
	checked_lock(&q->queue_lock);
	while (q->size == 0) {
		checked_wait(&q->empty, &q->queue_lock);
	}
	res = q->data[q->start++];
	q->start %= q->capacity;
	q->size--;
	checked_broadcast(&q->full);
	checked_unlock(&q->queue_lock);
	return (res);
}
