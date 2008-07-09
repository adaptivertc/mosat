#include <p30f4013.h>
#include "system.h"
#include "UART.h"

_FOSC(CSW_FSCM_OFF & XT_PLL8);  
_FWDT(WDT_OFF);                 
_FBORPOR(MCLR_EN & PWRT_OFF);   
_FGS(CODE_PROT_OFF); 

int main (void){

char c;

//PORTA  
//Input --> RA11


//PORTB  
//Input --> RB0, RB1, RB2, RB3, RB4, RB5, RB6, RB7, RB8
//Output -> RB9, RB10, RB11, RB12 
ADPCFG = 0xffff;
TRISB  = 0x01ff;

//PORTD  
//Input --> RD3, RD9
//Output -> RD0, RD1, RD8
TRISD  = 0x0208;

//PORTF
//Output -> RF0, RF1, RF2, RF3, RF6
TRISF  = 0x0000;

//UART
UART1_Init();



	while(1){
		LATB = 0;
		LATD = 0;
		LATF = 0;
		for(c=0; c<12; c++){
			if(c == 0)
				LATBbits.LATB9 = 1;
			if(c == 1)
				LATBbits.LATB10 = 1;
			if(c == 2)
				LATBbits.LATB11 = 1;
			if(c == 3)
				LATBbits.LATB12 = 1;	 
			if(c == 4)
				LATDbits.LATD0 = 1;
			if(c == 5)
				LATDbits.LATD1 = 1;
			if(c == 6)
				LATDbits.LATD8 = 1;
			if(c == 7)
				LATFbits.LATF0 = 1;	 
			if(c == 8)
				LATFbits.LATF1 = 1;
			if(c == 9)
				LATFbits.LATF2 = 1;
			if(c == 10)
				LATFbits.LATF3 = 1;
			if(c == 11)
				LATFbits.LATF6 = 1;	 
		}
	} 

} 

//eof
