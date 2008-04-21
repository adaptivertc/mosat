
export CPATH=../../../include:../../

libsimdriver.so: simdriver.o burnersim.o simiodriver.h
	g++ -o libsimdriver.so -shared simdriver.o burnersim.o

simdriver.o: simdriver.cpp
	g++ -c -fpic simdriver.cpp

burnersim.o: burnersim.cpp
	g++ -c -fpic burnersim.cpp

clean:
	rm *.o *.so

