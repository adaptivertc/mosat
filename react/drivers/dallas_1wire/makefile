
export CPATH=../../../include:../../

all: dallas_1wire_main libdallas_1wire_drv.so

libdallas_1wire_drv.so: dallas_1wire.o 
	g++ -Wall -o libdallas_1wire_drv.so -shared dallas_1wire.o -lrt ../../../modbus/librtmodbus.a 

dallas_1wire.o:	dallas_1wire.cpp dallas_1wire.h
	g++ -Wall -fpic -c dallas_1wire.cpp 

dallas_1wire_main: dallas_1wire_main.cpp dallas_1wire_read.cpp 
	g++ -Wall -o dallas_1wire_main dallas_1wire_main.cpp dallas_1wire_read.cpp -lrt ../../../modbus/librtmodbus.a

clean:
	rm *.o *.so dallas_1wire_main


