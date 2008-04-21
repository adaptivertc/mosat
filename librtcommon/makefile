
export CPATH=../include

CC=g++

%.o:    %.cpp $(INCLUDES)
	$(CC) -c -fpic -o $@ -Wall $<

all: librtcommon.so librtcommon.a 

librtcommon.so: arg.o common.o safestr.o utimer.o memfail.o
	g++  -shared -o librtcommon.so arg.o common.o safestr.o utimer.o memfail.o

librtcommon.a: arg.o common.o safestr.o utimer.o memfail.o
	  ar r librtcommon.a arg.o common.o safestr.o utimer.o memfail.o


install:
	cp librtcommon.so /usr/local/lib

installppc:
	make -f makefile.ppc install

installarm:
	make -f makefile.arm install

clean:
	rm -fv *.o librtcommon.so librtcommon.a
	make -f makefile.ppc clean 
	make -f makefile.arm clean

ppcall:
	make librtcommon.a
	make -f makefile.ppc


armcall:
	make librtcommon.a
	make -f makefile.arm

