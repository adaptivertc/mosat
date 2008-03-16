
export CPATH=../include

CC=g++ -Wall

INCLUDES=tcontrol.h alg1.h train_sim.h event_alg.h dc_evalg.h gp_evalg.h ri_evalg.h ag_evalg.h sim_msg.h tpconfig.h trainpos.h

RTCOMMON=../lib/librtcommon.a

%.o:    %.cpp $(INCLUDES)
	$(CC) -c -g -o $@ $<

all: main line_sim sim_error disp_curses rtautogen

main: main.o event_dispatch.o alg1.o alarm_notify.o alarm_admin.o \
      msg_queue.o dc_evalg.o gp_evalg.o ag_evalg.o ri_evalg.o time_table.o \
      display_reader.o tpconfig.o section_reader.o $(INCLUDES) 
	$(CC) -o main main.o event_dispatch.o alg1.o \
                 alarm_notify.o alarm_admin.o msg_queue.o dc_evalg.o \
                 gp_evalg.o ag_evalg.o ri_evalg.o time_table.o display_reader.o \
		tpconfig.o section_reader.o $(RTCOMMON)

line_sim: line_sim.o msg_queue.o train_sim.o sim_msg.o tpconfig.o $(INCLUDES)
	$(CC) -o line_sim line_sim.o msg_queue.o \
               train_sim.o sim_msg.o tpconfig.o $(RTCOMMON) 

sim_error: sim_error.o $(INCLUDES)
	$(CC) -o sim_error sim_error.o sim_msg.o

disp_curses: disp_curses.cpp
	$(CC) -o disp_curses -lncurses disp_curses.cpp
clean:
	rm -fv main line_sim sim_error disp_curses *.o rtautogen

rtautogen:	rtautogen.cpp
	g++ -Wall -o rtautogen rtautogen.cpp $(RTCOMMON)

