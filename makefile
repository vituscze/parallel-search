SRCDIR=src
BUILDDIR=build

OBJ=$(addprefix ${BUILDDIR}/,queue.o kmp.o producer.o consumer.o io_lock.o checked.o main.o)
PROG=${BUILDDIR}/psearch

FLAGS=-pthread

all: ${PROG}

gcc: FLAGS += -Wall -pedantic
gcc: ${PROG}

clang: FLAGS += -Wall -pedantic
clang: ${PROG}

solaris: FLAGS += -mt +w
solaris: ${PROG}

${PROG}: ${OBJ}
	${CC} ${CFLAGS} ${FLAGS} -o ${PROG} ${OBJ}

${OBJ}: | ${BUILDDIR}

${BUILDDIR}:
	mkdir ${BUILDDIR}

${BUILDDIR}/%.o: ${SRCDIR}/%.c
	${CC} ${CFLAGS} ${FLAGS} -c -o $@ $<

clean:
	rm -rf -- ${BUILDDIR}

