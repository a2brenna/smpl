INCLUDE_DIR=$(shell echo ~)/local/include
LIBRARY_DIR=$(shell echo ~)/local/lib
DESDTIR=/
PREFIX=/usr

CXX=clang++
CXXFLAGS=-L${LIBRARY_DIR} -I${INCLUDE_DIR} -g -std=c++11 -fPIC -Wall -Wextra -O2 -march=native

all: src/smpl.h

install: libraries
	mkdir -p ${DESTDIR}/${PREFIX}/lib
	mkdir -p ${DESTDIR}/${PREFIX}/include
	cp *.a ${DESTDIR}/${PREFIX}/lib
	cp *.so ${DESTDIR}/${PREFIX}/lib
	cp src/*.h ${DESTDIR}/${PREFIX}/include

test:

libraries:

clean:
	rm -rf test/
	rm -rf *.o
	rm -rf *.a
	rm -rf *.so
