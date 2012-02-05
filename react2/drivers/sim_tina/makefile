
export CPATH=../../../include:../../

libsimtinadrv.so: simtinadrv.o simtinadrv.h
	g++ -std=c++0x -Wall -o libsimtinadrv.so -shared simtinadrv.o 

simtinadrv.o: simtinadrv.cpp simtinadrv.h
	g++ -std=c++0x -Wall -c -fpic simtinadrv.cpp

clean:
	rm *.o *.so

