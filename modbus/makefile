

export CPATH=../include

RTCOMMON=../librtcommon/librtcommon.a
RTMODBUS=./librtmodbus.a

INCLUDES = hp2mod.h  hprtu.h  modio.h  modsim.h  mtype.h  trace.h evlog.h

CPP=g++

%.o:	%.cpp $(INCLUDES)
	$(CPP) -c -o $@ -Wall $<

all:	 librtmodbus.so librtmodbus.a modsim myconnect 

modsim:	myservice.o modsim.o trace.h 
	g++ -o modsim -lpthread myservice.o $(RTMODBUS) $(RTCOMMON) 

myconnect:	myconnect.o
	g++ -o myconnect myconnect.o  $(RTMODBUS) $(RTCOMMON) 

librtmodbus.so: trace.o modsim.o trace.h crc.o swap.o modtcpip_client.o\
                modtcpip.o hexascii.o crc.o rtmodbus.o modio.o \
                evlog.o rt_serial.o rt_mod_serial_client.o rt_modreplysize.o $(INCLUDES)
	g++ -o librtmodbus.so -shared modsim.o trace.o crc.o swap.o \
		 modtcpip.o hexascii.o rtmodbus.o modio.o modtcpip_client.o \
		evlog.o rt_serial.o rt_mod_serial_client.o rt_modreplysize.o

librtmodbus.a: trace.o modsim.o trace.h crc.o swap.o modtcpip_client.o\
                modtcpip.o hexascii.o crc.o rtmodbus.o modio.o \
                evlog.o rt_serial.o rt_mod_serial_client.o rt_modreplysize.o $(INCLUDES)
	ar r librtmodbus.a modsim.o trace.o crc.o swap.o \
		 modtcpip.o hexascii.o rtmodbus.o modio.o modtcpip_client.o \
		evlog.o rt_serial.o rt_mod_serial_client.o rt_modreplysize.o

install:
	cp librtmodbus.so /usr/local/lib
	cp rtmodbus.h /usr/local/include
	cp modsim /usr/local/bin


clean:	
	rm -fv *.o modsim myconnect librtmodbus.so librtmodbus.a 

ppcall:
	make -f makefile.ppc

ppcclean:
	make -f makefile.ppc clean

armall:
	make -f makefile.arm

armclean:
	make -f makefile.arm clean


