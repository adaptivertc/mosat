INCLUDES = hp2mod.h  hprtu.h  modio.h  modsim.h  mtype.h  trace.h

CC = g++ -Wall


ALL= remote_record ntt runit ppc_record sync auto_report frames ppc_all

all:	$(ALL)

%.o:	%.cpp $(INCLUDES)
	$(CC) -c -o $@ -Wall $<

ppc_record: ppc_record.o train_detect.o evlog.o train_detect.h ppc_logger.o ppc_logger.h event_detect.o
	g++ -g -o ppc_record -lrtcommon -lpthread \
 	   ppc_record.o evlog.o train_detect.o ppc_logger.o event_detect.o

remote_record: remote_record.o train_detect.o evlog.o train_detect.h o_evlog.o
	g++ -o remote_record -lrtcommon -lpthread remote_record.o \
	   o_evlog.o evlog.o train_detect.o

ntt:	timetable.cpp rf.cpp n_ractual.cpp
	g++ -o ntt -Wall -lrt -lrtcommon timetable.cpp rf.cpp n_ractual.cpp

runit:	runit.cpp
	g++ -o runit runit.cpp

clean:	
	rm -fv *.o *.so $(ALL) 
	make -f makefile.ppc clean


ppc_all:
	make -f makefile.ppc

sync:	sync.cpp
	g++ -Wall -o sync sync.cpp -lrtcommon

auto_report:	auto_report.cpp
	g++ -Wall -o auto_report auto_report.cpp

frames:	frames.cpp
	g++ -Wall -o frames frames.cpp


