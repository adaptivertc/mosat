# Makefile for react.
OBJ1 = reactmain.o db.o di.o do.o ao.o data.o scan.o dcalc.o shutdown.o alarmwin.o pointstate.o 
OBJ2 = secuencia.o iodriver.o pid.o secuencia_step.o logfile.o control.o web.o config.o displaydata.o script_support.o gen_case.o gen_script.o conio.o
OBJ3 =	int.o pci.o ldiodriver.o timeaccum.o error.o eval.o expr.o exprlist.o exprtree.o point.o  calc.o lddbpoint.o analog.o discrete.o pump.o level.o
OBJS =  $(OBJ1) $(OBJ2) $(OBJ3)

SRC1 =	reactmain.cpp db.cpp di.cpp do.cpp ao.cpp data.cpp scan.cpp dcalc.cpp shutdown.cpp alarmwin.cpp pointstate.cpp
SRC2 = secuencia.cpp iodriver.cpp pid.cpp secuencia_step.cpp logfile.cpp control.cpp web.cpp config.cpp displaydata.cpp
SRC3 = script_support.cpp gen_case.cpp gen_script.cpp conio.cpp int.cpp
SRC4 =	pci.cpp ldiodriver.cpp timeaccum.cpp error.cpp eval.cpp expr.cpp exprlist.cpp exprtree.cpp point.cpp calc.cpp lddbpoint.cpp analog.cpp discrete.cpp pump.cpp level.cpp
SRCS =  $(SRC1) $(SRC2) $(SRC3) $(SRC4)

INCLUDES = db.h db_point.h iodriver.h logfile.h\
error.h exp.h reactpoint.h secuencia.h displaydata.h timeaccum.h gen_script.h

#LIBS = -l pci_dask -ldl 
LIBS = -ldl -lrtcommon

GENFILES =  gen_case.cpp gen_script.cpp gen_script.h

DRIVERDIRS = drivers/pci9111 drivers/pci7248 drivers/sim drivers/modbusdrv drivers/shmdrv drivers/sim_pump drivers/sim_pump_low

CDEBUG =

CFLAGS = -Wall

LDFLAGS =

CC = g++

MAKE = make

all: react integrate utest rconsole rdisplay reactcgi rdrivers rtshmmain ai.so

%.o:	%.cpp $(INCLUDES)
	$(CC) -c -Wall -o $@ $<

rdrivers:
	for i in $(DRIVERDIRS); do cd "$$i"; $(MAKE); cd ../..; done

react:	$(INCLUDES) $(OBJS) dbpoint_util.so
	$(CC) $(LDFLAGS) -rdynamic -o $@ $(OBJS) $(LIBS)

rtshmmain: rtshmmain.o ldiodriver.o reactshmio.o reactshmio.h $(INCLUDES)
	$(CC) -rdynamic -o rtshmmain rtshmmain.o ldiodriver.o reactshmio.o -ldl -lrtcommon

$(OBJS):	$(INCLUDES)

secuencia:	secuencia.cpp
	g++ -o secuencia -lrtcommon secuencia.cpp


test:	pid.cpp 
	g++ -o test pid.cpp

integrate:	integrate.cpp  
	g++ -o integrate -lrtcommon integrate.cpp

CFLAGS=-Wall -Wstrict-prototypes -O2

p9111ai: p9111ai.o conio.o
	g++ -o p9111ai -L/usr/lib -lpci_dask p9111ai.o conio.o

p9111ai.o: p9111ai.c
	g++ -c $(CFLAGS) -I../../../include p9111ai.c

dbpoint_util.so: analog.cpp discrete.cpp
	g++ -fpic -o dbpoint_util.so -shared analog.cpp discrete.cpp 

gen:	generate_script.cpp 
	g++ -o gen -Wall -lrtcommon generate_script.cpp

$(GENFILES):	gen db_point.h
	./gen

TAGS:   $(SRCS)
	etags $(SRCS)


genc:	gen_script.cpp gen_script.h
	g++ -c gen_script.cpp 

avg:	average.cpp
	g++ -o avg -lrtcommon average.cpp

gencase:	gen_script.cpp gen_script.h
	g++ -c gen_case.cpp 

utest:	utest.cpp
	g++ -o utest -lrtcommon utest.cpp
	
ct:	conio.cpp conio.h coniotest.cpp
	g++ -o ct coniotest.cpp conio.cpp

simtest:	simtest.cpp burnersim.cpp sim.h
	g++ -o simtest simtest.cpp burnersim.cpp

genhtml:	genhtml.cpp
	g++ -o genhtml -lrtcommon genhtml.cpp

rconsole: reactconsole.cpp
	g++ -o rconsole reactconsole.cpp

rdisplay: rtdisplay.cpp displaydata.cpp
	g++ -o rdisplay -l curses rtdisplay.cpp displaydata.cpp

cgiTest: cgiTest.c
	g++ -o  cgiTest cgiTest.c

reactcgi: reactcgi.cpp
	g++ -o reactcgi reactcgi.cpp 

usbtest: usbtest.cpp
	g++ -Wall -W usbtest.cpp

split_fields:	split_fields.cpp
	g++ -o split_fields -lrtcommon split_fields.cpp
	cp split_fields /home/carr/bin

gen_images:	gen_images.cpp
	g++ -o gen_images -lrtcomon gen_images.cpp
	cp gen_images /home/carr/bin

exprtest:       error.h exp.h error.cpp eval.cpp expr.cpp exprlist.cpp exprtree.cpp point.cpp treetst.cpp
	g++ -o exprtest error.cpp eval.cpp expr.cpp exprlist.cpp exprtree.cpp point.cpp treetst.cpp

ai.so:	ai.cpp
	g++ -shared -fpic -o ai.so ai.cpp

clean:
	rm -fv *.o *.so react rconsole rdisplay reactcgi gen utest integrate
	rm -fv rtshmmain gen_case.cpp gen_script.cpp gen_script.h
	for i in $(DRIVERDIRS); do cd "$$i"; $(MAKE) clean; cd ../..; done


install:
	cp react /usr/local/bin
	cp rtshmmain /usr/local/bin
	cp rconsole /usr/local/bin
	cp rdisplay /usr/local/bin
	cp ai.so /usr/local/lib
