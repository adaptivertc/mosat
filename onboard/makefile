
export CPATH=../include

CC=g++

CPP=g++

%.o:    %.cpp $(INCLUDES)
	$(CPP) -c -o $@ -Wall $<

%.o:    %.c $(INCLUDES)
	$(CC) -c -o $@ -Wall $<

all: speed speed_2x24 

speed:	speed.o spd_display.o spd_comm.o spd_select.o spd_algo_DC.o
	g++ -g -o speed -lncurses ../lib/librtcommon.so ../modbus/librtmodbus.so \
		speed.o spd_display.o spd_comm.o spd_select.o spd_algo_DC.o

speed_4x40:	speed.o spd_display_4x40.o spd_comm.o spd_select.o
	g++ -g -o speed_4x40 -lncurses ../lib/librtcommon.so ../modbus/librtmodbus.so -lpthread \
		speed.o spd_display_4x40.o spd_comm.o spd_select.o spd_algo_DC.o

speed_2x24:	speed.o spd_display_2x24.o spd_comm.o d2x24_curses.o spd_select.o
	g++ -g -o speed_2x24 -lncurses ../lib/librtcommon.so ../modbus/librtmodbus.so speed.o \
		spd_display_2x24.o spd_comm.o d2x24_curses.o spd_select.o  spd_algo_DC.o

lcdmesg: lcdmesg.c
	gcc -o lcdmesg lcdmesg.c

test_both:        test_both.o -lpthread d2x24_curses.o
	$(CC) -o test_both -lpthread -lcurses -lrtcommon test_both.o d2x24_curses.o

keypad: keypad.c
	gcc -o keypad keypad.c

check_speed:	check_speed.o
	g++ -o  check_speed check_speed.o -lncurses
		
clean:
	rm -fv $(DIR)*.o
	rm -fv $(DIR)speed
	rm -fv $(DIR)speed_2x24
	rm -fv $(DIR)lcdmesg
	rm -fv $(DIR)keypad
