INCLUDES = hp2mod.h  hprtu.h  modio.h  modsim.h  mtype.h  trace.h rtmodbus.h evlog.h

CPP=g++

%.o:	%.cpp $(INCLUDES)
	$(CPP) -c -o $@ -Wall $<

all:	modsim myconnect librtmodbus.so

modsim:	myservice.o modsim.o trace.h 
	g++ -o modsim -lpthread -lrtmodbus -lrtcommon myservice.o 

myconnect:	myconnect.o
	g++ -o myconnect -lrtcommon myconnect.o -lrtmodbus

librtmodbus.so: trace.o modsim.o trace.h crc.o swap.o modtcpip_client.o\
                modtcpip.o hexascii.o crc.o rtmodbus.o modio.o \
                evlog.o rt_serial.o rt_mod_serial_client.o rt_modreplysize.o $(INCLUDES)
	g++ -o librtmodbus.so -shared modsim.o trace.o crc.o swap.o \
		 modtcpip.o hexascii.o rtmodbus.o modio.o modtcpip_client.o \
		evlog.o rt_serial.o rt_mod_serial_client.o rt_modreplysize.o

install:
	cp librtmodbus.so /usr/local/lib
	cp rtmodbus.h /usr/local/include
	cp modsim /usr/local/bin


clean:	
	rm -fv *.o *.so modsim myconnect *.so *.o

