
DIRS= librtcommon modbus mosat react trainpos onboard classic_tcontrol

PPCDIRS= librtcommon mosat modbus

MAKE= make

all:
	for i in $(DIRS); do cd "$$i"; $(MAKE); cd ..; done

clean:
	for i in $(DIRS); do cd "$$i"; $(MAKE) clean; cd ..; done

tp:
	cd librtcommon; $(MAKE); cd ..
	cd trainpos; $(MAKE); cd ..

ppcall:
	for i in $(PPCDIRS); do cd "$$i"; $(MAKE) ppcall; cd ..; done

