
CC=g++

INCLUDES=tcontrol.h alg1.h train_sim.h event_alg.h dc_evalg.h gp_evalg.h

%.o:    %.cpp $(INCLUDES)
	$(CC) -c -g -o $@ $<

all: main line_sim

main: main.o event_dispatch.o alg1.o alarm_notify.o alarm_admin.o \
      msg_queue.o dc_evalg.o gp_evalg.o time_table.o $(INCLUDES) 
	$(CC) -o main -lrtcommon main.o event_dispatch.o alg1.o \
                 alarm_notify.o alarm_admin.o msg_queue.o dc_evalg.o gp_evalg.o time_table.o

line_sim: line_sim.o msg_queue.o train_sim.o $(INCLUDES)
	$(CC) -o line_sim -lrtcommon line_sim.o msg_queue.o train_sim.o

clean:
	rm -fv main line_sim *.o
