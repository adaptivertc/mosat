
export CPATH=../../../include:../../

libsimdriver.so: simdriver.o burnersim.o simiodriver.h
	g++ -fPIC -std=c++0x -o libsimdriver.so -shared simdriver.o burnersim.o

simdriver.o: simdriver.cpp
	g++ -fPIC -std=c++0x -c simdriver.cpp

burnersim.o: burnersim.cpp
	g++ -fPIC -std=c++0x -c -fpic burnersim.cpp

clean:
	rm *.o *.so

