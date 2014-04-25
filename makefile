PROG=psearch

OBJ=queue.o kmp.o producer.o consumer.o io_lock.o checked.o main.o

LFLAGS=-lpthread
WFLAGS=-Wall -pedantic

FLAGS=${LFLAGS} ${WFLAGS}

all: ${PROG}

${PROG}: ${OBJ}
	${CC} ${CFLAGS} ${FLAGS} -o ${PROG} ${OBJ}

%.o: CFLAGS += ${FLAGS}

clean:
	rm -f -- ${PROG} ${OBJ}

