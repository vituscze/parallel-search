SRCDIR=src
BUILDDIR=build

OBJ=$(addprefix ${BUILDDIR}/,queue.o kmp.o producer.o consumer.o io_lock.o checked.o main.o)
PROG=${BUILDDIR}/psearch

LFLAGS=-pthread
WFLAGS=-Wall -pedantic

FLAGS=${LFLAGS} ${WFLAGS}

all: ${PROG}

${PROG}: ${OBJ}
	${CC} ${CFLAGS} ${FLAGS} -o ${PROG} ${OBJ}

${OBJ}: | ${BUILDDIR}

${BUILDDIR}:
	mkdir ${BUILDDIR}

${BUILDDIR}/%.o: ${SRCDIR}/%.c
	${CC} ${CFLAGS} ${FLAGS} -c -o $@ $<

clean:
	rm -f -- ${PROG} ${OBJ}
	rmdir -- ${BUILDDIR}

