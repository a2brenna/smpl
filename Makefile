INCLUDE_DIR=$(shell echo ~)/local/include
LIBRARY_DIR=$(shell echo ~)/local/lib
DESDTIR=/
PREFIX=/usr

CXX=clang++
CXXFLAGS=-L${LIBRARY_DIR} -I${INCLUDE_DIR} -g -std=c++11 -fPIC -Wall -Wextra -O2 -march=native

all: src/smpl.h

install: all
	mkdir -p ${DESTDIR}/${PREFIX}/include
	cp src/smpl.h ${DESTDIR}/${PREFIX}/include

uninstall:
	rm ${DESTDIR}/${PREFIX}/include/smpl.h

test:

clean:
	rm -rf *.o
	rm -rf *.a
	rm -rf *.so
