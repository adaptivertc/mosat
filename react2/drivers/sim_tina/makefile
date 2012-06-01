
export CPATH=../../../include:../../

libsimtinadrv.so: simtinadrv.o simtinadrv.h
	g++ -std=c++0x -Wall -fPIC -o libsimtinadrv.so -shared simtinadrv.o 

simtinadrv.o: simtinadrv.cpp simtinadrv.h
	g++ -std=c++0x -Wall -c -fPIC simtinadrv.cpp

clean:
	rm *.o *.so

