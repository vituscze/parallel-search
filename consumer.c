#include <fcntl.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

#include "consumer.h"
#include "io_lock.h"
#include "kmp.h"
#include "queue.h"

void *
consume(void *arg_)
{
	struct consume_arg *arg = arg_;
	struct queue *q = arg->q;
	struct kmp_table *t = arg->t;

	char buffer[BUFFER_SIZE];

	for (;;) {
		char *path = dequeue(q);

		int pos = 0;
		int file;
		int bytes_read = 0;

		struct kmp_result r;

		if (!path) {
			return (NULL);
		}

		file = open(path, O_RDONLY);
		if (file == -1) {
			pthread_mutex_lock(&io_lock);
			fprintf(stderr, "Cannot open %s\n", path);
			pthread_mutex_unlock(&io_lock);

			free(path);
			continue;
		}

		while ((bytes_read = read(file, buffer, BUFFER_SIZE)) > 0) {
			int i;
			for (i = 0; i < bytes_read; i++) {
				r = advance(buffer[i], pos, *t);
				pos = r.pos;
				if (r.match) {
					pthread_mutex_lock(&io_lock);
					printf("%s\n", path);
					pthread_mutex_unlock(&io_lock);
					break;
				}
			}

			if (r.match) {
				break;
			}
		}

		free(path);
		close(file);
	}
}

