#include <p30f4011.h>
#include "system.h"

#define BAUDRATE 	 9600
#define BRGVAL   	 ((FCY/BAUDRATE)/16)-1

char *UARTCharPtr;

void UART1_Init(void){
	
	U1MODE = 0x0000;//Clear UART1 registers 
	U1STA  = 0x0000;//Clear UART1 registers
	U1BRG = BRGVAL;
	U1MODE = 0x8400;//Enable,U1ARX/U1ATX,1 stop, no parity, 8 bit's data
	U1STA  = 0x0400;//Enable transmission	
}

void putByteU1(char c){
	while(U1STAbits.UTXBF);
	U1TXREG = c;
}

void putStringU1(char *s){
	while(*s)
		putByteU1(*s++);
}


char getByteU1(void){
	while(!U1STAbits.URXDA);
	return (U1RXREG);
}

char *getStringU1(char *s, int bytesMaximos){
	char *ptr = s;
	do{
		*s = getByteU1();
		putByteU1(*s);

			if((*s==0x8)&&(s>ptr)){
				putByteU1(' ');
				putByteU1(0x8);
				bytesMaximos++;
				s--;
				continue;
			}
		
		if(*s=='\n')
			continue;
		if(*s=='\r')
			break;
		s++;
		bytesMaximos--;
	}while(bytesMaximos>1);
	*s = '\0';
	return ptr;
}


//eof
