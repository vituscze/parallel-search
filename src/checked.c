#include <dirent.h>
#include <err.h>
#include <errno.h>
#include <fcntl.h>
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include "checked.h"

void
checked_thread_create(pthread_t *t, const pthread_attr_t *attr,
    void *(*func)(void *), void *arg)
{
	int err = pthread_create(t, attr, func, arg);
	if (err != 0) {
		errx(1, "Failed to create a thread: %s", strerror(err));
	}
}

void
checked_thread_join(pthread_t t, void **retval)
{
	int err = pthread_join(t, retval);
	if (err != 0) {
		errx(1, "Failed to join a thread: %s", strerror(err));
	}
}

void
checked_mutex_init(pthread_mutex_t *m)
{
	int err = pthread_mutex_init(m, NULL);
	if (err != 0) {
		errx(1, "Failed to initialize mutex: %s", strerror(err));
	}
}

void
checked_cond_init(pthread_cond_t *c)
{
	int err = pthread_cond_init(c, NULL);
	if (err != 0) {
		errx(1, "Failed to initialize condition variable: %s",
		    strerror(err));
	}
}

void
checked_mutex_destroy(pthread_mutex_t *m)
{
	int err = pthread_mutex_destroy(m);
	if (err != 0) {
		errx(1, "Failed to destroy a mutex: %s", strerror(err));
	}
}

void
checked_cond_destroy(pthread_cond_t *c)
{
	int err = pthread_cond_destroy(c);
	if (err != 0) {
		errx(1, "Failed to destroy a condition variable: %s",
		    strerror(err));
	}
}

void
checked_lock(pthread_mutex_t *m)
{
	int err = pthread_mutex_lock(m);
	if (err != 0) {
		errx(1, "Failed to lock a mutex: %s", strerror(err));
	}
}

void
checked_unlock(pthread_mutex_t *m)
{
	int err = pthread_mutex_unlock(m);
	if (err != 0) {
		errx(1, "Failed to unlock a mutex: %s", strerror(err));
	}
}

void
checked_wait(pthread_cond_t *c, pthread_mutex_t *m)
{
	int err = pthread_cond_wait(c, m);
	if (err != 0) {
		errx(1, "Failed to wait for a condition variable: %s",
		    strerror(err));
	}
}

void
checked_broadcast(pthread_cond_t *c)
{
	int err = pthread_cond_broadcast(c);
	if (err != 0) {
		errx(1, "Failed to broadcast a condition variable: %s",
		    strerror(err));
	}
}

char *
checked_strdup(const char *s)
{
	char *ns = strdup(s);
	if (ns == NULL) {
		errx(1, "Failed to duplicate a string: %s", strerror(errno));
	}

	return (ns);
}

void *
checked_malloc(size_t size)
{
	void *addr = malloc(size);
	if (addr == NULL) {
		errx(1, "Failed to allocate memory: %s", strerror(errno));
	}

	return (addr);
}

void
checked_lstat(const char *path, struct stat *stat)
{
	int err = lstat(path, stat);
	if (err != 0) {
		errx(1, "lstat failed: %s", strerror(errno));
	}
}

DIR *
checked_opendir(const char *path)
{
	DIR *d = opendir(path);
	if (d == NULL &&
	    errno != EACCES &&
	    errno != ENOENT &&
	    errno != ENOTDIR) {
		errx(1, "Failed to open a directory: %s", strerror(errno));
	}

	return (d);
}

struct dirent *
checked_readdir(DIR *d)
{
	struct dirent *e;

	errno = 0;
	e = readdir(d);
	if (e == NULL && errno != 0) {
		errx(1, "Failed to read a directory: %s", strerror(errno));
	}

	return (e);
}

void
checked_closedir(DIR *d)
{
	int err = closedir(d);
	if (err != 0) {
		errx(1, "Failed to close a directory: %s", strerror(errno));
	}
}

int
checked_open(const char *path, int flags)
{
	int fd = open(path, flags);
	if (fd == -1 &&
	    errno != EACCES &&
	    errno != ENOENT) {
		errx(1, "Failed to open a file: %s", strerror(errno));
	}

	return (fd);
}

ssize_t
checked_read(int fd, void *buffer, size_t size)
{
	ssize_t s = read(fd, buffer, size);
	if (s == -1) {
		errx(1, "Failed to read a file: %s", strerror(errno));
	}

	return (s);
}

void
checked_close(int fd)
{
	int err = close(fd);
	if (err != 0) {
		errx(1, "Failed to close a file: %s", strerror(errno));
	}
}
