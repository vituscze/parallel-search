Parallel search
===============

Usage
-----

    psearch [-n<number of threads>] -s<search pattern> [-d<starting directory>]

`psearch` checks all files in starting directory (specified by `-d`) and its subdirectories looking for a search pattern (specified by `-s`). If such pattern is found, the file is reported to stdout. If a file or directory could not be opened (e.g. the user didn't have necessary rights), it is reported to stderr. The files may be searched by more than one thread (this number can be adjusted by `-n`) and the program makes no guarantees about the order in which the files are reported to stdout.

If starting directory is not specified, the program uses `.`. Number of threads can also be left unspecified, in which case the current number of active CPUs is used (given by `sysconf`). The search pattern is mandatory.

Code
----

`psearch` is implemented using producer-consumer pattern. At the start of the program, one producer thread is created that searches for all regular files in starting directory and its subdirectories (symbolic links are not followed); those files (precisely path to them) are added to a concurrent queue. After that, a number of consumer threads (given by `-n` or `sysconf`) are created that repeatedly retrieve paths from the queue and then search the content of those files for the required pattern (using KMP algorithm).

Once the producer finished its task, a `NULL` is added to the queue for every consumer. When a consumer picks up a `NULL`, it terminates. The main thread then simply waits for the producer and all consumers to finish and then terminates as well.

To prevent mixing up output to stdout and stderr, a global lock is used for synchronization.

Brief overview of source files:

 * `main.c` - parses command line arguments, launches the producer and all consumers and then simply waits for them to finish
 * `kmp.c` - implementation of the KMP algorithm, `advance` function searches the input character by character and can be used from multiple threads
 * `queue.c` - implementation of concurrent queue, when the queue is empty, any attempt to retrieve an element will block the calling thread until a new element is added and vice versa - adding elements to full queue will block until an element is removed
 * `producer.c` - implementation of the producer
 * `consumer.c` - implementation of the consumer
 * `checked.c` - variants of various function that explicitly check for any error conditions and terminate the program if a non-recoverable error is detected
 * `io_lock.c` - single global mutex for I/O synchronization

