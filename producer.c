#include <dirent.h>
#include <errno.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include "producer.h"
#include "io_lock.h"
#include "queue.h"

#include <err.h>

void *
produce(void *arg_)
{
	struct produce_arg *arg = arg_;
	traverse(arg->path, arg->q);

	int i;
	for (i = 0; i < arg->consumer_count; i++) {
		enqueue(NULL, arg->q);
	}

	return (NULL);
}

void
traverse(char *path, struct queue *q)
{
	DIR *d = opendir(path);
	if (!d) {
		pthread_mutex_lock(&io_lock);
		fprintf(stderr, "Cannot open directory %s\n", path);
		pthread_mutex_unlock(&io_lock);
		return;
	}

	struct dirent *entry;
	struct stat stat;
	int p_size = strlen(path);

	while ((entry = readdir(d))) {
		if (   strcmp(entry->d_name, ".")  == 0
		    || strcmp(entry->d_name, "..") == 0) {
			continue;
		}

		int extra = strlen(entry->d_name);
		/*
		 * One extra space for slash and another one
		 * for \0.
		 */
		int full = p_size + extra + 2;
		char *new_path = malloc(sizeof(char) * full);
		int len = snprintf(new_path, full, "%s/%s",
		    path, entry->d_name);
		new_path[len] = 0;
		
		lstat(new_path, &stat);
		// TODO: Check return value and errno... EACCES and stuff.
		
		if (S_ISDIR(stat.st_mode)) {
			traverse(new_path, q);
			free(new_path);
			continue;
		}

		enqueue(new_path, q);
	}

	closedir(d);
}
