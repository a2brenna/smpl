INCLUDE_DIR=$(shell echo ~)/local/include
LIBRARY_DIR=$(shell echo ~)/local/lib
DESDTIR=/
PREFIX=/usr

CXX=clang++
CXXFLAGS=-L${LIBRARY_DIR} -I${INCLUDE_DIR} -g -std=c++11 -fPIC -Wall -Wextra -O2 -march=native

all: headers

install: all
	mkdir -p ${DESTDIR}/${PREFIX}/include
	cp src/*.h ${DESTDIR}/${PREFIX}/include

test:

headers: src/smpl.h

clean:
	rm -rf *.o
	rm -rf *.a
	rm -rf *.so
