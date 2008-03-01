
export CPATH=../include
CC=g++

%.o:    %.cpp $(INCLUDES)
	$(CC) -c -fpic -o $@ -Wall $<

all: librtcommon.so

librtcommon.so: arg.o common.o safestr.o utimer.o memfail.o
	g++ -shared -o librtcommon.so arg.o common.o safestr.o utimer.o memfail.o

install:
	cp librtcommon.so /usr/local/lib
	make -f makefile.ppc install

clean:
	rm -fv *.o *.so
	make -f makefile.ppc clean


ppcall:
	make -f makefile.ppc


