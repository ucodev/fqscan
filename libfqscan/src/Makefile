CC=`cat ../../.compiler`
INSTALL_DIR=`cat ../../.install_dir`
CCFLAGS=-std=c++11 -Wall -I/usr/local/include/gnuradio -I/usr/local/include -I../include -O2 -Wno-unused-function -fPIC -c
LDFLAGS=-lgnuradio-core -lgnuradio-osmosdr -lboost_system -lgruel -lrfsp++ -fPIC -shared
SOURCES=fq_scan.cpp fq_sink.cpp fq_tb.cpp
OBJECTS=fq_scan.o fq_sink.o fq_tb.o
HEADERS=../include/*.hpp
TARGET=libfqscan++.so

all:
	${CC} ${CCFLAGS} ${SOURCES}
	${CC} -o ${TARGET} ${OBJECTS} ${LDFLAGS} 

install:
	cp ${TARGET} ${INSTALL_DIR}/lib/
	mkdir -p ${INSTALL_DIR}/include/libfqscan
	cp ${HEADERS} ${INSTALL_DIR}/include/libfqscan/
	ldconfig

clean:
	rm -f *.o
	rm -f ${TARGET}
