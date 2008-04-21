
export CPATH=../include

CC=g++ -g

CPP=g++ -g

RTCOMMON=../librtcommon/librtcommon.a
RTMODBUS=../modbus/librtmodbus.a

%.o:    %.cpp $(INCLUDES)
	$(CPP) -c -o $@ -Wall $<

%.o:    %.c $(INCLUDES)
	$(CC) -c -o $@ -Wall $<

all: onboard onboard_2x24 drive


onboard:	onboard.o spd_display.o spd_comm.o spd_select.o spd_algo_DC.o spd_algo_RD.o spd_algo_VV.o \
		profile_reader.o  sim_reader.o 
	g++ -g -o onboard -lncurses onboard.o spd_display.o spd_comm.o spd_select.o sim_reader.o \
			spd_algo_DC.o spd_algo_RD.o  spd_algo_VV.o profile_reader.o $(RTMODBUS) $(RTCOMMON)

drive:	onboard.o spd_display.o spd_comm_drive.o spd_select.o spd_algo_DC.o spd_algo_RD.o spd_algo_VV.o \
		profile_reader.o  sim_reader.o 
	g++ -g -o drive -lncurses onboard.o spd_display.o spd_comm_drive.o spd_select.o sim_reader.o \
			spd_algo_DC.o spd_algo_RD.o  spd_algo_VV.o profile_reader.o $(RTMODBUS) $(RTCOMMON)

onboard_4x40:	onboard.o spd_display_4x40.o spd_comm.o spd_select.o
onboard_4x40:	onboard.o spd_display_4x40.o spd_comm.o spd_select.o
	g++ -g -o onboard_4x40 -lncurses -lpthread onboard.o spd_display_4x40.o \
		spd_comm.o spd_select.o spd_algo_DC.o spd_algo_RD.o  spd_algo_VV.o $(RTMODBUS) $(RTCOMMON)

onboard_2x24:	onboard.o spd_display_2x24.o spd_comm.o d2x24_curses.o spd_select.o sim_reader.o \
		spd_algo_DC.o spd_algo_RD.o  spd_algo_VV.o profile_reader.o
	g++ -g -o onboard_2x24 -lncurses onboard.o spd_display_2x24.o spd_comm.o sim_reader.o profile_reader.o \
		d2x24_curses.o spd_select.o  spd_algo_DC.o spd_algo_RD.o  spd_algo_VV.o $(RTMODBUS) $(RTCOMMON)

lcdmesg: lcdmesg.c
	gcc -o lcdmesg lcdmesg.c

test_both:        test_both.o -lpthread d2x24_curses.o
	$(CC) -o test_both -lpthread -lcurses test_both.o d2x24_curses.o $(RTCOMMON)

keypad: keypad.c
	gcc -o keypad keypad.c

check_speed:	check_speed.o
	g++ -o  check_speed check_speed.o -lncurses

#profile_reader: profile_reader.cpp profile_reader.h
#	g++ -o profile_reader profile_reader.cpp  $(RTCOMMON)
		
clean:
	rm -fv $(DIR)*.o
	rm -fv $(DIR)onboard
	rm -fv $(DIR)onboard_2x24
	rm -fv $(DIR)lcdmesg
	rm -fv $(DIR)keypad
