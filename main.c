#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "consumer.h"
#include "kmp.h"
#include "producer.h"
#include "queue.h"

#define INITIAL_CAPACITY 5

void
usage(const char *progname)
{
	fprintf(stderr,
	    "Usage: %s -n<thread count> -s<needle> -d<directory>\n"
	    "\n"
	    "Recursively searches all files in directory for\n"
	    "a specified pattern and reports them to stdout.\n"
	    "\n"
	    "If thread count is not specified, number of cores\n"
	    "is used instead.\n"
	    "If directory is not specified, . is used instead\n",
	    progname);
}

int
main(int argc, char **argv)
{
	int opt;
	int threads = -1;
	int size;
	char *needle = NULL;
	char *dir = NULL;

	opterr = 0;

	while ((opt = getopt(argc, argv, "n:s:d:")) != -1) {
		switch (opt) {
			case 'n':
				threads = atoi(optarg);
				break;
			case 's':
				size = strlen(optarg) + 1;
				needle = malloc(sizeof(char) * size);
				strncpy(needle, optarg, size);
				needle[size - 1] = 0;
				break;
			case 'd':
				size = strlen(optarg) + 1;
				dir = malloc(sizeof(char) * size);
				strncpy(dir, optarg, size);
				dir[size - 1] = 0;
				break;
			case '?':
				usage(argv[0]);
				return (1);
		}
	}

	if (!needle) {
		usage(argv[0]);
		return (1);
	}

	if (!dir) {
		dir = malloc(sizeof(char) * 2);
		dir[0] = '.';
		dir[1] = 0;
	}

	if (threads <= 0) {
		threads = 1;
		// TODO: Figure out number of cores.
	}

	struct kmp_table table = alloc_table(needle);
	fill_table(table);

	struct queue q = alloc_queue(INITIAL_CAPACITY);

	struct produce_arg parg = {&q, dir, threads};
	struct consume_arg carg = {&q, &table};
	
	pthread_t producer;
	pthread_t *consumers = malloc(sizeof(pthread_t) * threads);

	pthread_create(&producer, NULL, &produce, &parg);
	int i;
	for (i = 0; i < threads; i++) {
		pthread_create(consumers + i, NULL, &consume, &carg);
	}

	pthread_join(producer, NULL);
	for (i = 0; i < threads; i++) {
		pthread_join(consumers[i], NULL);
	}

	free_queue(&q);
	free_table(table);
	free(consumers);
	free(needle);
	free(dir);

	return 0;
}

