
export CPATH=../../../include:../../

libsimpumpdrv.so: simpumpdrv.o simpumpdrv.h
	g++ -o libsimpumpdrv.so -shared simpumpdrv.o 

simpumpdrv.o: simpumpdrv.cpp
	g++ -c -fpic simpumpdrv.cpp

clean:
	rm *.o *.so

