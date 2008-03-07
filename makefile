
DIRS= lib modbus mosat react trainpos onboard 

MAKE= make

all:
	for i in $(DIRS); do cd "$$i"; $(MAKE); cd ..; done

clean:
	for i in $(DIRS); do cd "$$i"; $(MAKE) clean; cd ..; done
