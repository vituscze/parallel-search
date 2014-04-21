#include <pthread.h>
#include <stdlib.h>

#include "queue.h"

struct queue
alloc_queue(int capacity)
{
	struct queue q =
	    { NULL
	    , 0
	    , capacity
	    , 0
	    , PTHREAD_MUTEX_INITIALIZER
	    , PTHREAD_COND_INITIALIZER
	    , PTHREAD_COND_INITIALIZER
	    };
	q.data = malloc(sizeof(void *) * capacity);
	return (q);
}

void
free_queue(struct queue *q)
{
	free(q->data);
	pthread_mutex_destroy(&q->queue_lock);
	pthread_cond_destroy(&q->empty);
	pthread_cond_destroy(&q->full);
}

void
enqueue(void *elem, struct queue *q)
{
	pthread_mutex_lock(&q->queue_lock);
	while (q->size == q->capacity) {
		pthread_cond_wait(&q->full, &q->queue_lock);
	}
	q->data[(q->start + q->size++) % q->capacity] = elem;
	pthread_cond_broadcast(&q->empty);
	pthread_mutex_unlock(&q->queue_lock);
}

void *
dequeue(struct queue *q)
{
	void *res;
	pthread_mutex_lock(&q->queue_lock);
	while (q->size == 0) {
		pthread_cond_wait(&q->empty, &q->queue_lock);
	}
	res = q->data[q->start++];
	q->start %= q->capacity;
	q->size--;
	pthread_cond_broadcast(&q->full);
	pthread_mutex_unlock(&q->queue_lock);
	return (res);
}

