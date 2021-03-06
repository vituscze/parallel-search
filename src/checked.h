#ifndef CHECKED_H_
#define	CHECKED_H_

#include <dirent.h>
#include <pthread.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

/*
 * Unless specified otherwise, these functions just call
 * the relevant function and check for any error
 * return values.
 *
 * If an error value is returned, error message is printed
 * using errx and the program is terminated with non-zero
 * return value.
 */

/* Checked pthread functions. */

void
checked_thread_create(pthread_t *t, const pthread_attr_t *attr,
    void *(*func)(void *), void *arg);

void
checked_thread_join(pthread_t t, void **retval);

void
checked_mutex_init(pthread_mutex_t *m);

void
checked_cond_init(pthread_cond_t *c);

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

char *
checked_strdup(const char *s);

void *
checked_malloc(size_t size);

/* Checked file and directory operations. */

void
checked_lstat(const char *path, struct stat *stat);

/*
 * Returns NULL for non-critical errors, which should
 * not cause the program to halt. Those errors are:
 *
 * - EACCES
 * - ENOENT
 * - ENOTDIR
 */
DIR *
checked_opendir(const char *path);

struct dirent *
checked_readdir(DIR *d);

void
checked_closedir(DIR *d);

/*
 * Returns -1 for non-critical errors, which should
 * not cause the program to halt. Those errors are:
 *
 * - EACCES
 * - ENOENT
 */
int
checked_open(const char *path, int flags);

ssize_t
checked_read(int fd, void *buffer, size_t size);

void
checked_close(int fd);

#endif /* CHECKED_H_ */
