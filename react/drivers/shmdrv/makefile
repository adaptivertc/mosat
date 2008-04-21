
export CPATH=../../../include:../../

libreactshmdrv.so: reactshmdrv.o reactshmio.o
	g++ -o libreactshmdrv.so -shared reactshmdrv.o reactshmio.o 

reactshmdrv.o:	reactshmdrv.cpp reactshmdrv.h
	g++ -fpic -c reactshmdrv.cpp 

reactshmio.o:	../../reactshmio.cpp ../../reactshmio.h
	g++ -fpic -c -o reactshmio.o ../../reactshmio.cpp 

clean:
	rm *.o *.so


