#include <pthread.h>

#include "io_lock.h"

pthread_mutex_t io_lock = PTHREAD_MUTEX_INITIALIZER;

