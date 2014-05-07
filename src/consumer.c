#include <fcntl.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

#include "checked.h"
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

		file = checked_open(path, O_RDONLY);
		if (file == -1) {
			fprintf_ts(stderr, "Cannot open %s\n", path);

			free(path);
			continue;
		}

		while ((bytes_read =
		    checked_read(file, buffer, BUFFER_SIZE)) > 0) {
			int i;
			for (i = 0; i < bytes_read; i++) {
				advance(t, &r, buffer[i], pos);
				pos = r.pos;
				if (r.match) {
					fprintf_ts(stdout, "%s\n", path);
					break;
				}
			}

			if (r.match) {
				break;
			}
		}

		free(path);
		checked_close(file);
	}
}
