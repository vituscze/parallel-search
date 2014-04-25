#ifndef CONSUMER_H_
#define	CONSUMER_H_

#include "kmp.h"
#include "queue.h"

#define	BUFFER_SIZE (16 * 1024)

/*
 * Simple aggregate struct that allows
 * passing both relevant arguments to
 * pthread_create.
 */
struct consume_arg
{
	struct queue *q;
	struct kmp_table *t;
};

/*
 * Given a pointer to consume_arg struct,
 * this function repeatedly removes a path
 * from the queue and searches the file
 * specified by the path for a needle given
 * by kmp_table.
 *
 * Files that contain the given string are
 * printed to stdout. Files that could
 * not be accessed are reported to stderr.
 *
 * This function terminates when the
 * removed path is NULL.
 *
 * Strings removed from the queue are
 * freed.
 */
void *
consume(void *arg);

#endif /* CONSUMER_H_ */
