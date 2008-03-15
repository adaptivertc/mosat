
DIRS= main

MAKE= make
MAKEPPC = make -f makefile.ppc

all:
	for i in $(DIRS); do cd "$$i"; $(MAKE); $(MAKEPPC); cd ..; done

clean:
	for i in $(DIRS); do cd "$$i"; $(MAKE) clean; $(MAKEPPC) clean; cd ..; done
