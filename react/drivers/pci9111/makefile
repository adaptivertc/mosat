
export CPATH=../../../include:../../

libpci9111.so: pci9111.o 
	g++ -o libpci9111.so -shared pci9111.o 

pci9111.o:	pci9111.cpp pci9111.h
	g++ -fpic -c pci9111.cpp 

clean:
	rm *.o *.so

