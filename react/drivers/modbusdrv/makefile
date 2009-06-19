export CPATH=../../../include:../../

libreactmodbusdrv.so: reactmodbusdrv.o 
	g++ -Wall -o libreactmodbusdrv.so -shared reactmodbusdrv.o -lrt ../../../modbus/librtmodbus.a 

reactmodbusdrv.o:	reactmodbusdrv.cpp reactmodbusdrv.h
	g++ -fpic -Wall -c reactmodbusdrv.cpp 

clean:
	rm -v *.o *.so


