export CPATH=../../../include:../../

libdallas_1wire_drv.so: dallas_1wire.o 
	g++ -o libdallas_1wire_drv.so -shared dallas_1wire.o -lrt ../../../modbus/librtmodbus.a 

dallas_1wire.o:	dallas_1wire.cpp dallas_1wire.h
	g++ -fpic -c dallas_1wire.cpp 

clean:
	rm *.o *.so


