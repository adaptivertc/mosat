
all: main crc.o mod_server_msg.o swap.o hexascii.o

hexascii.o: hexascii.c
	gcc -Wall -c hexascii.c

main.o: main.c
	gcc -Wall -c main.c

crc.o: crc.c 
	gcc -Wall -c crc.c 

mod_server_msg.o: mod_server_msg.c rt_modbus_opcodes.h 
	gcc -Wall -c mod_server_msg.c 

swap.o: swap.c
	gcc -Wall -c swap.c

main: main.o mod_server_msg.o crc.o swap.o hexascii.o
	gcc -Wall -o main main.o mod_server_msg.o crc.o swap.o hexascii.o

clean: 
	rm -v *.o main
