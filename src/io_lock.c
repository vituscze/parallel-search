#include <pthread.h>
#include <stdarg.h>
#include <stdio.h>

#include "checked.h"
#include "io_lock.h"

static pthread_mutex_t io_lock = PTHREAD_MUTEX_INITIALIZER;

int
fprintf_ts(FILE *f, const char *fmt, ...)
{
	int res;

	va_list args;

	va_start(args, fmt);
	checked_lock(&io_lock);
	res = vfprintf(f, fmt, args);
	checked_unlock(&io_lock);
	va_end(args);

	return (res);
}
