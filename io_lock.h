#ifndef IO_LOCK_H_
#define	IO_LOCK_H_

#include <pthread.h>

/*
 * Synchronization for IO.
 *
 * This mutex is used to synchronize writes
 * from multiple consumers reporting both
 * successful matches and inaccessible files
 * and also a producer reporting inaccessible
 * directories.
 */
extern pthread_mutex_t io_lock;

#endif /* IO_LOCK_H_ */
