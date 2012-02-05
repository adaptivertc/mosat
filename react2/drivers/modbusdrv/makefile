export CPATH=../../../include:../../

libreactmodbusdrv.so: reactmodbusdrv.o 
	g++ -std=c++0x -Wall -o libreactmodbusdrv.so -shared reactmodbusdrv.o -lrt ../../../modbus/librtmodbus.a 

reactmodbusdrv.o:	reactmodbusdrv.cpp reactmodbusdrv.h
	g++ -std=c++0x -fpic -Wall -c reactmodbusdrv.cpp 

clean:
	rm -v *.o *.so


