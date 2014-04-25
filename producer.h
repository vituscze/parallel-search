#ifndef PRODUCER_H_
#define	PRODUCER_H_

#include "queue.h"

struct produce_arg
{
	struct queue *q;
	char *path;
	int consumer_count;
};

void *
produce(void *arg);

void
traverse(char *path, struct queue *q);

#endif /* PRODUCER_H_ */
