#include <dirent.h>
#include <errno.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include "checked.h"
#include "io_lock.h"
#include "producer.h"
#include "queue.h"

void *
produce(void *arg_)
{
	struct produce_arg *arg = arg_;
	int i;

	traverse(arg->path, arg->q);

	for (i = 0; i < arg->consumer_count; i++) {
		enqueue(NULL, arg->q);
	}

	return (NULL);
}

void
traverse(char *path, struct queue *q)
{
	DIR *d = checked_opendir(path);
	struct dirent *entry;
	struct stat stat;
	int p_size;

	if (!d) {
		fprintf_ts(stderr, "Cannot open directory %s\n", path);
		return;
	}

	p_size = strlen(path);

	while ((entry = checked_readdir(d))) {
		int extra;
		int full;
		int len;

		char *new_path;

		if (strcmp(entry->d_name, ".")  == 0 ||
		    strcmp(entry->d_name, "..") == 0) {
			continue;
		}

		extra = strlen(entry->d_name);
		/*
		 * One extra space for slash and another one
		 * for \0.
		 */
		full = p_size + extra + 2;
		new_path = checked_malloc(sizeof (char) * full);
		len = snprintf(new_path, full, "%s/%s",
		    path, entry->d_name);
		new_path[len] = 0;

		checked_lstat(new_path, &stat);
		if (S_ISDIR(stat.st_mode)) {
			traverse(new_path, q);
			free(new_path);
			continue;
		}

		if (S_ISREG(stat.st_mode)) {
			enqueue(q, new_path);
		}
	}

	checked_closedir(d);
}
