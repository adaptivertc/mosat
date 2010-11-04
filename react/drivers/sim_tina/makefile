
export CPATH=../../../include:../../

libsimtinadrv.so: simtinadrv.o simtinadrv.h
	g++ -Wall -o libsimtinadrv.so -shared simtinadrv.o 

simtinadrv.o: simtinadrv.cpp simtinadrv.h
	g++ -Wall -c -fpic simtinadrv.cpp

clean:
	rm *.o *.so

