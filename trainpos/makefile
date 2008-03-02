
export CPATH=../include
export COMPILER_PATH=../lib

CC=g++ -Wall

INCLUDES=tcontrol.h alg1.h train_sim.h event_alg.h dc_evalg.h gp_evalg.h ri_evalg.h sim_msg.h tpconfig.h

%.o:    %.cpp $(INCLUDES)
	$(CC) -c -g -o $@ $<

all: main line_sim sim_error disp_curses

main: main.o event_dispatch.o alg1.o alarm_notify.o alarm_admin.o \
      msg_queue.o dc_evalg.o gp_evalg.o ri_evalg.o time_table.o \
      display_reader.o tpconfig.o $(INCLUDES) 
	$(CC) -o main ../lib/librtcommon.so main.o event_dispatch.o alg1.o \
                 alarm_notify.o alarm_admin.o msg_queue.o dc_evalg.o \
                 gp_evalg.o ri_evalg.o time_table.o display_reader.o tpconfig.o

line_sim: line_sim.o msg_queue.o train_sim.o sim_msg.o tpconfig.o $(INCLUDES)
	$(CC) -o line_sim ../lib/librtcommon.so line_sim.o msg_queue.o \
               train_sim.o sim_msg.o tpconfig.o

sim_error: sim_error.o $(INCLUDES)
	$(CC) -o sim_error sim_error.o sim_msg.o

disp_curses: disp_curses.cpp
	$(CC) -o disp_curses -lcurses disp_curses.cpp
clean:
	rm -fv main line_sim sim_error disp_curses *.o
