#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "checked.h"
#include "consumer.h"
#include "kmp.h"
#include "producer.h"
#include "queue.h"

#define	INITIAL_CAPACITY 50

void
usage(const char *progname)
{
	fprintf(stderr,
	    "Usage: %s [-n<thread count>] -s<needle> [-d<directory>]\n"
	    "\n"
	    "Recursively searches all files in directory for\n"
	    "a specified pattern and reports them to stdout.\n"
	    "The pattern cannot be empty string.\n"
	    "\n"
	    "If thread count is not specified, number of cores\n"
	    "is used instead.\n"
	    "If directory is not specified, . is used instead\n",
	    progname);
}

int
main(int argc, char **argv)
{
	int i;
	int opt;
	int threads = -1;
	int size;
	int cpu;

	char *needle = NULL;
	char *dir = NULL;

	struct kmp_table table;
	struct queue q;
	struct produce_arg parg;
	struct consume_arg carg;

	pthread_t producer;
	pthread_t *consumers;

	while ((opt = getopt(argc, argv, "n:s:d:")) != -1) {
		switch (opt) {
			case 'n':
				threads = atoi(optarg);
				break;
			case 's':
				size = strlen(optarg) + 1;
				needle = checked_malloc(sizeof (char) * size);
				strncpy(needle, optarg, size);
				needle[size - 1] = 0;
				break;
			case 'd':
				size = strlen(optarg) + 1;
				dir = checked_malloc(sizeof (char) * size);
				strncpy(dir, optarg, size);
				dir[size - 1] = 0;
				break;
			case '?':
			default:
				usage(argv[0]);
				return (1);
		}
	}

	if (!needle || strlen(needle) <= 0) {
		usage(argv[0]);
		return (1);
	}

	if (!dir) {
		dir = checked_malloc(sizeof (char) * 2);
		dir[0] = '.';
		dir[1] = 0;
	}

	if (threads <= 0) {
		cpu = sysconf(_SC_NPROCESSORS_ONLN);
		if (cpu <= 1) {
			threads = 1;
		} else if (cpu >= 20) {
			threads = 20;
		} else {
			threads = cpu;
		}
	}

	table = alloc_table(needle);
	fill_table(table);

	q = alloc_queue(INITIAL_CAPACITY);

	parg.q = &q;
	parg.path = dir;
	parg.consumer_count = threads;

	carg.q = &q;
	carg.t = &table;

	consumers = checked_malloc(sizeof (pthread_t) * threads);

	checked_thread_create(&producer, NULL, &produce, &parg);
	for (i = 0; i < threads; i++) {
		checked_thread_create(consumers + i, NULL, &consume, &carg);
	}

	checked_thread_join(producer, NULL);
	for (i = 0; i < threads; i++) {
		checked_thread_join(consumers[i], NULL);
	}

	free_queue(&q);
	free_table(table);
	free(consumers);
	free(needle);
	free(dir);

	return (0);
}
