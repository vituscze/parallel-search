#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "consumer.h"
#include "kmp.h"
#include "queue.h"
#include "producer.h"

int
main(int argc, char **argv)
{
	if (argc < 3) {
		return 1;
	}

	struct kmp_table table = alloc_table(argv[1]);
	fill_table(table);

	struct queue q = alloc_queue(20);

	struct produce_arg parg = {&q, argv[2], 1};
	struct consume_arg carg = {&q, &table};
	
	pthread_t t1;
	pthread_t t2;

	pthread_create(&t1, NULL, &produce, &parg);
	pthread_create(&t2, NULL, &consume, &carg);

	pthread_join(t1, NULL);
	pthread_join(t2, NULL);

	free_queue(&q);
	free_table(table);

	return 0;
}

