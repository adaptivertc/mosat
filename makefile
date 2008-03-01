# Add  a comment
#
# bla bla
#

DIRS= lib modbus mosat react trainpos 

MAKE= make

all:
	for i in $(DIRS); do cd "$$i"; $(MAKE); cd ..; done

clean:
	for i in $(DIRS); do cd "$$i"; $(MAKE) clean; cd ..; done
