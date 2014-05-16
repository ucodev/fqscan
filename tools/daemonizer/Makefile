CC=`cat ../../.compiler_c`
INSTALL_DIR=`cat ../../.install_dir`
CCFLAGS=-Wall -O2 -Wno-unused-function -c
LDFLAGS=
SOURCES=daemonizer-v0.9.5.c
OBJECTS=daemonizer-v0.9.5.o
TARGET=daemonizer

all:
	${CC} ${CCFLAGS} ${SOURCES}
	${CC} -o ${TARGET} ${OBJECTS} ${LDFLAGS}

install:
	cp ${TARGET} ${INSTALL_DIR}/bin/

clean:
	rm -f ${OBJECTS}
	rm -f ${TARGET}
