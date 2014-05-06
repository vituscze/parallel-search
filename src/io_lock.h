#ifndef IO_LOCK_H_
#define	IO_LOCK_H_

#include <pthread.h>
#include <stdio.h>

/*
 * Thread safe version of fprintf.
 *
 * The safety is guaranteed by locking a global
 * mutex inside io_lock.c.
 */

int
fprintf_ts(FILE *f, const char *fmt, ...);

#endif /* IO_LOCK_H_ */
