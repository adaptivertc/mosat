
export CPATH=../include

OBJS=  arg.o common.o safestr.o utimer.o memfail.o  numbered_file.o ap_config.o

CC=g++

%.o:    %.cpp $(INCLUDES)
	$(CC) -c -fpic -o $@ -Wall $<

all: librtcommon.so librtcommon.a 

librtcommon.so: $(OBJS) 
	g++  -shared -o librtcommon.so $(OBJS) 

librtcommon.a: $(OBJS) 
	  ar r librtcommon.a $(OBJS) 


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
	make -f makefile.ppc
	make librtcommon.a

armall:
	make -f makefile.arm
	make librtcommon.a

ppcclean:
	make -f makefile.ppc clean

armclean:
	make -f makefile.arm clean

