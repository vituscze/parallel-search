#ifndef QUEUE_H_
#define	QUEUE_H_

#include <pthread.h>

/*
 * Queue for multithreaded usage.
 *
 * data is array of pointers to actual data.
 * start is the index of first element.
 * capacity is the maximum queue size.
 * size is the current queue size.
 */
struct queue
{
	void **data;
	int start;
	int capacity;
	int size;

	pthread_mutex_t queue_lock;
	pthread_cond_t empty;
	pthread_cond_t full;
};

/*
 * Allocates a new queue of given capacity.
 */
void
alloc_queue(struct queue *q, int capacity);

/*
 * Frees a queue allocated by alloc_queue.
 */
void
free_queue(struct queue *q);

/*
 * Adds a new element to the end of the queue. If
 * the queue is full, it causes the calling thread
 * to block until the element can be added.
 */
void
enqueue(struct queue *q, void *elem);

/*
 * Removes and returns the first element of the
 * queue. If the queue is empty, it causes the
 * calling thread to block until an element
 * is added.
 */
void *
dequeue(struct queue *q);

#endif /* QUEUE_H_ */
