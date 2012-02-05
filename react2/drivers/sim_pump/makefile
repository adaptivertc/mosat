
export CPATH=../../../include:../../

libsimpumpdrv.so: simpumpdrv.o simpumpdrv.h
	g++ -std=c++0x -o libsimpumpdrv.so -shared simpumpdrv.o 

simpumpdrv.o: simpumpdrv.cpp
	g++ -std=c++0x -c -fpic simpumpdrv.cpp

clean:
	rm *.o *.so

