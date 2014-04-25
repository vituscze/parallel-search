#ifndef PRODUCER_H_
#define	PRODUCER_H_

#include "queue.h"

/*
 * Simple aggregate struct that allows
 * passing all three relevant arguments
 * to pthread_create.
 *
 * consumer_count is used to enqueue
 * NULLs to correctly terminate all
 * consumers.
 */
struct produce_arg
{
	struct queue *q;
	char *path;
	int consumer_count;
};

/*
 * Simple wrapper around traverse function
 * below.
 *
 * The pointer should be pointing to a
 * produce_arg struct described above.
 *
 * After traverse terminates, enqueues
 * number of NULLs given by consumer_count.
 * See above.
 */
void *
produce(void *arg);

/*
 * Recursively search directory given by path
 * and add all regular files to the queue.
 *
 * Inaccessible directories are reported to
 * stderr.
 *
 * Note that this function is recursive and
 * terminates when all subdirectories have been
 * searched. It also does not follow symlinks.
 *
 * This function dynamically allocates memory
 * for paths inserted into queue. Any user of
 * the queue should free those after removing
 * them from the queue.
 */
void
traverse(char *path, struct queue *q);

#endif /* PRODUCER_H_ */
