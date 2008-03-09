
DIRS= lib modbus mosat react trainpos onboard classic_tcontrol

MAKE= make

all:
	for i in $(DIRS); do cd "$$i"; $(MAKE); cd ..; done

clean:
	for i in $(DIRS); do cd "$$i"; $(MAKE) clean; cd ..; done

tp:
	cd lib; $(MAKE); cd ..
	cd trainpos; $(MAKE); cd ..

