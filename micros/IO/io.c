/* use %s/^V^M//g to remove the ^M thingees, that is CTRL-V+CTRL-M*/


#include <p30f4013.h>
#include "system.h"
#include "UART.h"
#include <stdio.h>

char adaptive_getchar(void);
char adaptive_putchar(char ch);
short adaptive_read_di(void);
short adaptive_read_do(void);
void adaptive_send_do(short n, short val);


_FOSC(CSW_FSCM_OFF & XT_PLL8);  
_FWDT(WDT_OFF);     
_FBORPOR(MCLR_EN & PWRT_OFF);   
_FGS(CODE_PROT_OFF); 

int main (void){

char c;
char input_char;
int i, do_n;
short d_val;
char parameter[3];


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

/*this is the implementation of the simple ascii protocol, there are only 2 message types:
If we receive an ascii 'R' (this is a READ) we write out 24 zeroes and ones, the first 12 correspond to the inputs
and the second 12 correspond to the outputs
If we receive an ascii 'W' (this is a WRITE) or more specific a request to set a single bit on the output
The 'W' must be followed by three more bytes.
  The first two bytes are the channel number or the bit to set (Discrete Output)
  The third byte must be zero or one and it is the value to set the bit to*/
	/*each byte corresponds to a bit in ascii*/

  while(1){
    input_char = adaptive_getchar(); //get an R, W or NULL from the terminal
    if (input_char != -1){
       if(input_char == 'R'){
  	 d_val = adaptive_read_di();//read input's 12 bits packed into a short
  	 for(i = 0 ; i<12 ; i++){ //we ignore the upper 4 bits
    	   if((d_val & 0x1) == 0){ //checking if the first bit is zero
      	     adaptive_putchar('0'); //to serial
           }
           else{
             adaptive_putchar('1');//to serial
           }
    	   d_val >>= 1; //shift right to the next bit and repeat
  	 }
         d_val = adaptive_read_do(); //now reading the output's 12 bits packed into a short
  	 for(i = 0 ; i<12 ; i++){
    	   if((d_val & 0x1) == 0){ //checking if the first bit is zero
      	     adaptive_putchar('0');
    	   }
    	   else{
      	     adaptive_putchar('1');
    	   }
  	   d_val >>= 1; //shift right to the next bit and repeat
 	  }
       }

       else if (input_char == 'W'){
	 for(i = 0; i<3 ; i++){
           parameter[i] = adaptive_getchar(); //must deal with the timeout, reading from the serial
         }

  /*The next two lines convert the first two characters to an integer.
  Also assumes that the first digit is an ascii '0' or an ascii '1'.*/

 	 do_n = parameter[1] - '0';
  	 if(parameter[0] == '1') do_n += 10;
         if(parameter[2] == '0') adaptive_send_do(do_n, 0);
         else adaptive_send_do(do_n, 1);
       }
    }
  }
} 

//eof
