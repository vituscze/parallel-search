#ifndef CHECKED_H_
#define CHECKED_H_

#include <dirent.h>
#include <pthread.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

/* Checked pthread functions. */

void
checked_thread_create(pthread_t *t, const pthread_attr_t *attr,
    void *(*func)(void *), void *arg);

void
checked_thread_join(pthread_t t, void **retval);

void
checked_mutex_destroy(pthread_mutex_t *m);

void
checked_cond_destroy(pthread_cond_t *c);

void
checked_lock(pthread_mutex_t *m);

void
checked_unlock(pthread_mutex_t *m);

void
checked_wait(pthread_cond_t *c, pthread_mutex_t *m);

void
checked_broadcast(pthread_cond_t *c);

/* Checked memory allocation. */

void *
checked_malloc(size_t size);

/* Checked file and directory operations. */

void
checked_lstat(const char *path, struct stat *stat);

DIR *
checked_opendir(const char *path);

struct dirent *
checked_readdir(DIR *d);

void
checked_closedir(DIR *d);

int
checked_open(const char *path, int flags);

ssize_t
checked_read(int fd, void *buffer, size_t size);

void
checked_close(int fd);

#endif /* CHECKED_H_ */

