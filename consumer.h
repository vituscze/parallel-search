#ifndef CONSUMER_H_
#define CONSUMER_H_

#include "kmp.h"
#include "queue.h"

#define BUFFER_SIZE (16 * 1024)

struct consume_arg
{
	struct queue* q;
	struct kmp_table* t;
};

void
consume(void *queue);

#endif /* CONSUMER_H_ */
