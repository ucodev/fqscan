CC=`cat ../../.compiler`
INSTALL_DIR=`cat ../../.install_dir`
CCFLAGS=-std=c++11 -Wall -I/usr/local/include/gnuradio -I/usr/local/include -I../include -O2 -Wno-unused-function -c
LDFLAGS=-lgnuradio-core -lgnuradio-osmosdr -lboost_system -lgruel -lmysqlclient_r -lfqscan++ -lrfsp++
SOURCES=args.cpp main.cpp sql.cpp runtime.cpp hook.cpp
OBJECTS=args.o main.o sql.o runtime.o hook.o
TARGET=fqscan

all:
	${CC} ${CCFLAGS} ${SOURCES}
	${CC} -o ${TARGET} ${OBJECTS} ${LDFLAGS}

install:
	cp ${TARGET} ${INSTALL_DIR}/bin/

clean:
	rm -f ${OBJECTS}
	rm -f ${TARGET}
