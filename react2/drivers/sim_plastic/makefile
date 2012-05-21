
export CPATH=../../../include:../../

libsimplasticdrv.so: simplasticdrv.o simplasticdrv.h
	g++ -std=c++0x -Wall -o libsimplasticdrv.so -shared simplasticdrv.o 

simplasticdrv.o: simplasticdrv.cpp
	g++ -std=c++0x -Wall -c -fpic simplasticdrv.cpp

clean:
	rm *.o *.so

