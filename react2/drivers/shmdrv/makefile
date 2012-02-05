
export CPATH=../../../include:../../

libreactshmdrv.so: reactshmdrv.o reactshmio.o
	g++ -std=c++0x -o libreactshmdrv.so -shared reactshmdrv.o reactshmio.o 

reactshmdrv.o:	reactshmdrv.cpp reactshmdrv.h
	g++ -std=c++0x -fpic -c reactshmdrv.cpp 

reactshmio.o:	../../reactshmio.cpp ../../reactshmio.h
	g++ -std=c++0x -fpic -c -o reactshmio.o ../../reactshmio.cpp 

clean:
	rm *.o *.so


