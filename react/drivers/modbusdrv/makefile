export CPATH=../../../include:../../

libreactmodbusdrv.so: reactmodbusdrv.o 
	g++ -o libreactmodbusdrv.so -shared reactmodbusdrv.o -lrt ../../../modbus/librtmodbus.a 

reactmodbusdrv.o:	reactmodbusdrv.cpp reactmodbusdrv.h
	g++ -fpic -c reactmodbusdrv.cpp 

clean:
	rm *.o *.so


