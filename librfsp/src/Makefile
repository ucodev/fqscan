CC=`cat ../../.compiler`
INSTALL_DIR=`cat ../../.install_dir`
CCFLAGS=-std=c++11 -Wall -I../include -O2 -Wno-unused-function -fPIC -c
LDFLAGS=-lboost_system -fPIC -shared
SOURCES=rfsp_window.cpp polynom.cpp zero.cpp
OBJECTS=rfsp_window.o polynom.o zero.o
HEADERS=../include/*.hpp
TARGET=librfsp++.so

all:
	${CC} ${CCFLAGS} ${SOURCES}
	${CC} -o ${TARGET} ${OBJECTS} ${LDFLAGS} 

install:
	cp ${TARGET} ${INSTALL_DIR}/lib/
	mkdir -p ${INSTALL_DIR}/include/librfsp
	cp ${HEADERS} ${INSTALL_DIR}/include/librfsp/
	ldconfig

clean:
	rm -f *.o
	rm -f ${TARGET}
