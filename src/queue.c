#include <pthread.h>
#include <stdlib.h>

#include "checked.h"
#include "queue.h"

void
alloc_queue(struct queue *q, int capacity)
{
	q->data = checked_malloc(sizeof (void *) * capacity);
	q->start = 0;
	q->capacity = capacity;
	q->size = 0;
	checked_mutex_init(&q->queue_lock);
	checked_cond_init(&q->empty);
	checked_cond_init(&q->full);
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
enqueue(struct queue *q, void *elem)
{
	checked_lock(&q->queue_lock);
	while (q->size == q->capacity) {
		checked_wait(&q->full, &q->queue_lock);
	}
	q->data[(q->start + q->size++) % q->capacity] = elem;
	checked_unlock(&q->queue_lock);
	checked_broadcast(&q->empty);
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
	checked_unlock(&q->queue_lock);
	checked_broadcast(&q->full);
	return (res);
}
