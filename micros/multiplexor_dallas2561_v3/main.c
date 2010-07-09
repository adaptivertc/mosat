/*****************************************************
This program was produced by the
CodeWizardAVR V1.25.5 Professional
Automatic Program Generator
© Copyright 1998-2007 Pavel Haiduc, HP InfoTech s.r.l.
http://www.hpinfotech.com

Project : 
Version : 
Date    : 25/06/2010
Author  : F4CG                            
Company : F4CG                            
Comments: 


Chip type           : ATmega2561
Program type        : Application
Clock frequency     : 16.000000 MHz
Memory model        : Small
External SRAM size  : 0
Data Stack size     : 2048
*****************************************************/

#include <mega2561.h>

// 1 Wire Bus functions
#asm
   .equ __w1_port=0x02 ;PORTA
   .equ __w1_bit=2
#endasm
#include <1wire.h>

// DS1820 Temperature Sensor functions
#include <ds18b20.h>

// maximum number of DS1820 devices
// connected to the 1 Wire bus
#define MAX_DS1820 20
// number of DS1820 devices
// connected to the 1 Wire bus
unsigned char ds1820_devices;
// DS1820 devices ROM code storage area,
// 9 bytes are used for each device
// (see the w1_search function description in the help)
unsigned char ds1820_rom_codes[MAX_DS1820][9];
unsigned char ds1820_rom_codes_sorted[256][9];

#define RXB8 1
#define TXB8 0
#define UPE 2
#define OVR 3
#define FE 4
#define UDRE 5
#define RXC 7

#define FRAMING_ERROR (1<<FE)
#define PARITY_ERROR (1<<UPE)
#define DATA_OVERRUN (1<<OVR)
#define DATA_REGISTER_EMPTY (1<<UDRE)
#define RX_COMPLETE (1<<RXC)

// USART0 Receiver buffer
#define RX_BUFFER_SIZE0 256

char rx_buffer0[RX_BUFFER_SIZE0];

#if RX_BUFFER_SIZE0<256
unsigned char rx_wr_index0,rx_rd_index0,rx_counter0;
#else
unsigned int rx_wr_index0,rx_rd_index0,rx_counter0;
#endif

// This flag is set on USART0 Receiver buffer overflow
bit rx_buffer_overflow0;

// USART0 Receiver interrupt service routine
interrupt [USART0_RXC] void usart0_rx_isr(void)
{
char status,data;
status=UCSR0A;
data=UDR0;
if ((status & (FRAMING_ERROR | PARITY_ERROR | DATA_OVERRUN))==0)
   {
   rx_buffer0[rx_wr_index0]=data;
   if (++rx_wr_index0 == RX_BUFFER_SIZE0) rx_wr_index0=0;
   if (++rx_counter0 == RX_BUFFER_SIZE0)
      {
      rx_counter0=0;
      rx_buffer_overflow0=1;
      };
   };
}

#ifndef _DEBUG_TERMINAL_IO_
// Get a character from the USART0 Receiver buffer
#define _ALTERNATE_GETCHAR_
#pragma used+
char getchar(void)
{
char data;
while (rx_counter0==0);
data=rx_buffer0[rx_rd_index0];
if (++rx_rd_index0 == RX_BUFFER_SIZE0) rx_rd_index0=0;
#asm("cli")
--rx_counter0;
#asm("sei")
return data;
}
#pragma used-
#endif





// USART0 Transmitter buffer
#define TX_BUFFER_SIZE0 256
char tx_buffer0[TX_BUFFER_SIZE0];

#if TX_BUFFER_SIZE0<256
unsigned char tx_wr_index0,tx_rd_index0,tx_counter0;
#else
unsigned int tx_wr_index0,tx_rd_index0,tx_counter0;
#endif

// USART0 Transmitter interrupt service routine
interrupt [USART0_TXC] void usart0_tx_isr(void)
{
if (tx_counter0)
   {
   --tx_counter0;
   UDR0=tx_buffer0[tx_rd_index0];
   if (++tx_rd_index0 == TX_BUFFER_SIZE0) tx_rd_index0=0;
   };
}

#ifndef _DEBUG_TERMINAL_IO_
// Write a character to the USART0 Transmitter buffer
#define _ALTERNATE_PUTCHAR_
#pragma used+
void putchar(char c)
{
while (tx_counter0 == TX_BUFFER_SIZE0);
#asm("cli")
if (tx_counter0 || ((UCSR0A & DATA_REGISTER_EMPTY)==0))
   {
   tx_buffer0[tx_wr_index0]=c;
   if (++tx_wr_index0 == TX_BUFFER_SIZE0) tx_wr_index0=0;
   ++tx_counter0;
   }
else
   UDR0=c;
#asm("sei")
}
#pragma used-
#endif



// USART1 Receiver buffer
#define RX_BUFFER_SIZE1 64
char rx_buffer1[RX_BUFFER_SIZE1];

#if RX_BUFFER_SIZE1<256
unsigned char rx_wr_index1,rx_rd_index1,rx_counter1;
#else
unsigned int rx_wr_index1,rx_rd_index1,rx_counter1;
#endif

// This flag is set on USART1 Receiver buffer overflow
bit rx_buffer_overflow1;

// USART1 Receiver interrupt service routine
interrupt [USART1_RXC] void usart1_rx_isr(void)
{
char status,data;
status=UCSR1A;
data=UDR1;
if ((status & (FRAMING_ERROR | PARITY_ERROR | DATA_OVERRUN))==0)
   {
   rx_buffer1[rx_wr_index1]=data;
   if (++rx_wr_index1 == RX_BUFFER_SIZE1) rx_wr_index1=0;
   if (++rx_counter1 == RX_BUFFER_SIZE1)
      {
      rx_counter1=0;
      rx_buffer_overflow1=1;
      };
   };
}

// Get a character from the USART1 Receiver buffer
#pragma used+
char getchar1(void)
{
char data;
while (rx_counter1==0);
data=rx_buffer1[rx_rd_index1];
if (++rx_rd_index1 == RX_BUFFER_SIZE1) rx_rd_index1=0;
#asm("cli")
--rx_counter1;
#asm("sei")
return data;
}
#pragma used-
// USART1 Transmitter buffer
#define TX_BUFFER_SIZE1 64
char tx_buffer1[TX_BUFFER_SIZE1];

#if TX_BUFFER_SIZE1<256
unsigned char tx_wr_index1,tx_rd_index1,tx_counter1;
#else
unsigned int tx_wr_index1,tx_rd_index1,tx_counter1;
#endif

// USART1 Transmitter interrupt service routine
interrupt [USART1_TXC] void usart1_tx_isr(void)
{
if (tx_counter1)
   {
   --tx_counter1;
   UDR1=tx_buffer1[tx_rd_index1];
   if (++tx_rd_index1 == TX_BUFFER_SIZE1) tx_rd_index1=0;
   };
}

// Write a character to the USART1 Transmitter buffer
#pragma used+
void putchar1(char c)
{
while (tx_counter1 == TX_BUFFER_SIZE1);
#asm("cli")
if (tx_counter1 || ((UCSR1A & DATA_REGISTER_EMPTY)==0))
   {
   tx_buffer1[tx_wr_index1]=c;
   if (++tx_wr_index1 == TX_BUFFER_SIZE1) tx_wr_index1=0;
   ++tx_counter1;
   }
else
   UDR1=c;
#asm("sei")
}
#pragma used-

// Standard Input/Output functions
#include <stdio.h> 
#include <stdlib.h>
#include <delay.h> 
#include "character_map.h"
#include "rt_modbus_crc.h"
#include "mod_server_msg.h"
#include "mod_mega2561_plex_imp.h"


// Declare your global variables here 
uint16_t cantidad_columnas;
eeprom uint16_t cantidad_sensores[COL_SIZE];
uint16_t cantidad_sensores_detectados[COL_SIZE];
uint16_t temperatures[COL_NUM][COL_SIZE];

eeprom int garbage;
int unit_id;
int last_number_showed;


void detect_dallas(int number_expected);
void chose_channel(int channel);  
void read_info();
void led_show_temp();

void flush_usart()
{
	while(rx_counter0 > 0)
		getchar();
}                    




void main(void)
{
// Declare your local variables here
int i, j, z;
char letras[5];
unsigned long int timer;
float temp_temp;
char letra;

uint8_t buf[128];
int min_size;
int total_size;
int n;
int buf_index;
int reply_size;

// Crystal Oscillator division factor: 1
#pragma optsize-
CLKPR=0x80;
CLKPR=0x00;
#ifdef _OPTIMIZE_SIZE_
#pragma optsize+
#endif

// Input/Output Ports initialization
// Port A initialization
// Func7=In Func6=Out Func5=Out Func4=Out Func3=Out Func2=In Func1=Out Func0=In 
// State7=T State6=0 State5=0 State4=0 State3=0 State2=T State1=0 State0=T 
PORTA=0x00;
DDRA=0x7A;

// Port B initialization
// Func7=In Func6=In Func5=In Func4=In Func3=In Func2=In Func1=In Func0=In 
// State7=P State6=P State5=P State4=P State3=P State2=P State1=T State0=P 
PORTB=0xFD;
DDRB=0x00;

// Port C initialization
// Func7=In Func6=In Func5=In Func4=In Func3=In Func2=In Func1=In Func0=In 
// State7=T State6=T State5=T State4=T State3=T State2=T State1=T State0=T 
PORTC=0x00;
DDRC=0x00;

// Port D initialization
// Func7=Out Func6=Out Func5=Out Func4=Out Func3=In Func2=In Func1=Out Func0=In 
// State7=0 State6=0 State5=0 State4=0 State3=T State2=T State1=0 State0=T 
PORTD=0x00;
DDRD=0xF2;

// Port E initialization
// Func7=In Func6=In Func5=In Func4=In Func3=In Func2=In Func1=In Func0=In 
// State7=T State6=T State5=T State4=T State3=T State2=T State1=T State0=T 
PORTE=0x00;
DDRE=0x00;

// Port F initialization
// Func7=In Func6=In Func5=In Func4=In Func3=In Func2=In Func1=In Func0=In 
// State7=T State6=T State5=T State4=T State3=T State2=T State1=T State0=T 
PORTF=0x00;
DDRF=0x00;

// Port G initialization
// Func5=In Func4=In Func3=In Func2=In Func1=In Func0=In 
// State5=T State4=P State3=P State2=T State1=T State0=T 
PORTG=0x18;
DDRG=0x00;

// Timer/Counter 0 initialization
// Clock source: System Clock
// Clock value: Timer 0 Stopped
// Mode: Normal top=FFh
// OC0A output: Disconnected
// OC0B output: Disconnected
TCCR0A=0x00;
TCCR0B=0x00;
TCNT0=0x00;
OCR0A=0x00;
OCR0B=0x00;

// Timer/Counter 1 initialization
// Clock source: System Clock
// Clock value: Timer 1 Stopped
// Mode: Normal top=FFFFh
// OC1A output: Discon.
// OC1B output: Discon.
// OC1C output: Discon.
// Noise Canceler: Off
// Input Capture on Falling Edge
// Timer 1 Overflow Interrupt: Off
// Input Capture Interrupt: Off
// Compare A Match Interrupt: Off
// Compare B Match Interrupt: Off
// Compare C Match Interrupt: Off
TCCR1A=0x00;
TCCR1B=0x00;
TCNT1H=0x00;
TCNT1L=0x00;
ICR1H=0x00;
ICR1L=0x00;
OCR1AH=0x00;
OCR1AL=0x00;
OCR1BH=0x00;
OCR1BL=0x00;
OCR1CH=0x00;
OCR1CL=0x00;

// Timer/Counter 2 initialization
// Clock source: System Clock
// Clock value: Timer 2 Stopped
// Mode: Normal top=FFh
// OC2A output: Disconnected
// OC2B output: Disconnected
ASSR=0x00;
TCCR2A=0x00;
TCCR2B=0x00;
TCNT2=0x00;
OCR2A=0x00;
OCR2B=0x00;

// Timer/Counter 3 initialization
// Clock source: System Clock
// Clock value: Timer 3 Stopped
// Mode: Normal top=FFFFh
// Noise Canceler: Off
// Input Capture on Falling Edge
// OC3A output: Discon.
// OC3B output: Discon.
// OC3C output: Discon.
// Timer 3 Overflow Interrupt: Off
// Input Capture Interrupt: Off
// Compare A Match Interrupt: Off
// Compare B Match Interrupt: Off
// Compare C Match Interrupt: Off
TCCR3A=0x00;
TCCR3B=0x00;
TCNT3H=0x00;
TCNT3L=0x00;
ICR3H=0x00;
ICR3L=0x00;
OCR3AH=0x00;
OCR3AL=0x00;
OCR3BH=0x00;
OCR3BL=0x00;
OCR3CH=0x00;
OCR3CL=0x00;

// Timer/Counter 4 initialization
// Clock source: System Clock
// Clock value: Timer 4 Stopped
// Mode: Normal top=FFFFh
// OC4A output: Discon.
// OC4B output: Discon.
// OC4C output: Discon.
// Noise Canceler: Off
// Input Capture on Falling Edge
// Timer 4 Overflow Interrupt: Off
// Input Capture Interrupt: Off
// Compare A Match Interrupt: Off
// Compare B Match Interrupt: Off
// Compare C Match Interrupt: Off
TCCR4A=0x00;
TCCR4B=0x00;
TCNT4H=0x00;
TCNT4L=0x00;
ICR4H=0x00;
ICR4L=0x00;
OCR4AH=0x00;
OCR4AL=0x00;
OCR4BH=0x00;
OCR4BL=0x00;
OCR4CH=0x00;
OCR4CL=0x00;

// Timer/Counter 5 initialization
// Clock source: System Clock
// Clock value: Timer 5 Stopped
// Mode: Normal top=FFFFh
// OC5A output: Discon.
// OC5B output: Discon.
// OC5C output: Discon.
// Noise Canceler: Off
// Input Capture on Falling Edge
// Timer 5 Overflow Interrupt: Off
// Input Capture Interrupt: Off
// Compare A Match Interrupt: Off
// Compare B Match Interrupt: Off
// Compare C Match Interrupt: Off
TCCR5A=0x00;
TCCR5B=0x00;
TCNT5H=0x00;
TCNT5L=0x00;
ICR5H=0x00;
ICR5L=0x00;
OCR5AH=0x00;
OCR5AL=0x00;
OCR5BH=0x00;
OCR5BL=0x00;
OCR5CH=0x00;
OCR5CL=0x00;

// External Interrupt(s) initialization
// INT0: Off
// INT1: Off
// INT2: Off
// INT3: Off
// INT4: Off
// INT5: Off
// INT6: Off
// INT7: Off
EICRA=0x00;
EICRB=0x00;
EIMSK=0x00;
// PCINT0 interrupt: Off
// PCINT1 interrupt: Off
// PCINT2 interrupt: Off
// PCINT3 interrupt: Off
// PCINT4 interrupt: Off
// PCINT5 interrupt: Off
// PCINT6 interrupt: Off
// PCINT7 interrupt: Off
// PCINT8 interrupt: Off
// PCINT9 interrupt: Off
// PCINT10 interrupt: Off
// PCINT11 interrupt: Off
// PCINT12 interrupt: Off
// PCINT13 interrupt: Off
// PCINT14 interrupt: Off
// PCINT15 interrupt: Off
// PCINT16 interrupt: Off
// PCINT17 interrupt: Off
// PCINT18 interrupt: Off
// PCINT19 interrupt: Off
// PCINT20 interrupt: Off
// PCINT21 interrupt: Off
// PCINT22 interrupt: Off
// PCINT23 interrupt: Off
PCMSK0=0x00;
PCMSK1=0x00;
PCMSK2=0x00;
PCICR=0x00;

// Timer/Counter 0 Interrupt(s) initialization
TIMSK0=0x00;
// Timer/Counter 1 Interrupt(s) initialization
TIMSK1=0x00;
// Timer/Counter 2 Interrupt(s) initialization
TIMSK2=0x00;
// Timer/Counter 3 Interrupt(s) initialization
TIMSK3=0x00;
// Timer/Counter 4 Interrupt(s) initialization
TIMSK4=0x00;
// Timer/Counter 5 Interrupt(s) initialization
TIMSK5=0x00;

// USART0 initialization
// Communication Parameters: 8 Data, 1 Stop, No Parity
// USART0 Receiver: On
// USART0 Transmitter: On
// USART0 Mode: Asynchronous
// USART0 Baud Rate: 19200
UCSR0A=0x00;
UCSR0B=0xD8;
UCSR0C=0x06;
UBRR0H=0x00;
UBRR0L=0x33;

// USART1 initialization
// Communication Parameters: 8 Data, 1 Stop, No Parity
// USART1 Receiver: On
// USART1 Transmitter: On
// USART1 Mode: Asynchronous
// USART1 Baud Rate: 19200
UCSR1A=0x00;
UCSR1B=0xD8;
UCSR1C=0x06;
UBRR1H=0x00;
UBRR1L=0x33;

// Analog Comparator initialization
// Analog Comparator: Off
// Analog Comparator Input Capture by Timer/Counter 1: Off
ACSR=0x80;
ADCSRB=0x00;

// Determine the number of DS1820 devices
// connected to the 1 Wire bus
//ds1820_devices=w1_search(0xf0,ds1820_rom_codes);

// Global enable interrupts
#asm("sei")
  

unit_id = 4;
cantidad_columnas = 16;
/*cantidad_sensores[0] = 16;
cantidad_sensores[1] = 16;
cantidad_sensores[2] = 16;
cantidad_sensores[3] = 16;
cantidad_sensores[4] = 16;
cantidad_sensores[5] = 16;
cantidad_sensores[6] = 16;
cantidad_sensores[7] = 16;
cantidad_sensores[8] = 16;
cantidad_sensores[9] = 16;
cantidad_sensores[10] = 16;
cantidad_sensores[11] = 16;
cantidad_sensores[12] = 16;
cantidad_sensores[13] = 16;
cantidad_sensores[14] = 16;
cantidad_sensores[15] = 16;*/

PORTD.1 = 1;

last_number_showed = 0; 

buf_index = 0; 

MUX_ENABLE = 1; 

letra = 'a';
i = 0;

//for(i = 0; i < 16; i++)
//{
        chose_channel(1);
        
        detect_dallas(cantidad_sensores[i-1]);
//}


while (1)
      {
      // Place your code here
      //printf("HOla");
      
      if(i == cantidad_columnas)
        i = 1;
      else
        i++;
      
      chose_channel(i);
      
      detect_dallas(cantidad_sensores[i-1]);
      
      cantidad_sensores_detectados[i-1] = ds1820_devices;
      
      if(ds1820_devices <= 0)
      {
        for(j = 0; j < cantidad_sensores[i-1]; j++)
                temperatures[i-1][j] = 9999;
                
      }

      
      for(j = 0; j < cantidad_sensores[i-1]; j++)
      {
              
              if(rx_counter0 > 0)
         {
         	timer = 0;
         	buf_index = 0;
         	
         	
         	while(rx_counter0 < 2)
         	{
                	timer++;
                	if(timer == TIME_OUT)
                        	break;
         	}
         	if(timer == TIME_OUT && rx_counter0 < 2)
         	{
         		flush_usart();
         		buf_index = 0;
         	}
         	else
         	{
         	        
         	        buf[buf_index++] = getchar();
         	        buf[buf_index++] = getchar();
         		//buf[buf_index++] = getchar();
         		
         		min_size = rt_modbus_min_bytes(buf);
         		
         		
         		timer = 0;
         		
         		while(rx_counter0 < min_size - 2)
         		{
         		        timer++;
         		        if(timer == TIME_OUT)
         		                break;
         		}
         		
         		if(timer == TIME_OUT && rx_counter0 < min_size - 2)
         		{
         		        flush_usart();
         		        buf_index = 0;
         		}
         		else
         		{
         		        for(z = 0; z < min_size - 2; z++)
         		                buf[buf_index++] = getchar();
         		        
         		        total_size = rt_modbus_total_bytes(buf, min_size);
         		        
         		        
                               
         		        
         		        if(total_size > min_size)
         		        {
         		                timer = 0;
         		                
         		                //led_char('t');
         		                
         		                while(rx_counter0 < total_size - min_size)
         		                {
         		                        timer++;
         		                        if(timer == TIME_OUT)
         		                                break;
         		                }
         		                
         		                if(timer == TIME_OUT && rx_counter0 < total_size - min_size)
         		                {
         		                  flush_usart();
         		                  buf_index = 0;
         		                }
         		                else
         		                {
         		                          for(z = 0; z < total_size - min_size; z++)
         		                                  buf[buf_index++] = getchar();
         		                          
         		                          
         		                }
         		        }
         		        
         		        if (unit_id != buf[0])
         		        {
         		                
         		                flush_usart();
         		                buf_index = 0;
                                }
                                else
                                {
                                
                               
                                
                                        //printf("test");
                                        
                 		        reply_size = rt_modbus_process_request(buf, total_size);
                 		        
                 		        add_CRC(buf, reply_size, 0xffff);
                 		        
                 		        
                 		        
                 		        //0102035555559731
                 		        //printf("%c%c%c%c%c%c%c%c",0x01,0x02,0x03,0x55,0x55,0x55,0x97,0x31);
                 		        
                 		        for(z = 0; z < reply_size; z++)
                 		                putchar(buf[z]);
                 		        
                 		        
                 		        flush_usart();
                 		        buf_index = 0;
                 		                //printf("%c",buf[i]);
                 		}
         		}
         	}
         }
              
              
              
            
      
       temp_temp = ds18b20_temperature(ds1820_rom_codes_sorted[j]);
        
        if(temp_temp == -9999)
                temperatures[i-1][j] = 9999;
      
        else
                temperatures[i-1][j] = temp_temp* 10;
        
                
        //led_char(letra);
        //letra++;

        
        
      
      led_show_temp();

              
      }
      
      
      }; 
      
      
      
}

unsigned char read_bit(unsigned char bit_index, unsigned char value)
{
unsigned char temp;
unsigned char mask;

mask = 1 << bit_index;

temp = value & mask;
        if(temp == 0)
                return 0;
        else
                return 1;
}

void led_show_temp()
{
        int temp_val;
        unsigned char col, sens;
        char buff[5];
        
        col = sens = 0;
        
        if(read_bit(4,~PING) == 1)
                sens += 1;
        if(~PINB.7 == 1)
                sens += 2;
        if(~PINB.6 == 1)
                sens += 4;
        if(~PINB.5 == 1)
                sens += 8;
                
        if(~PINB.4 == 1)
                col += 1;
        if(~PINB.3 == 1)
                col += 2;
        if(~PINB.2 == 1)
                col += 4;
        if(~PINB.0 == 1)
                col += 8;
                
        if(temperatures[col][sens] == 9999)
        {
                if(last_number_showed != 9999)
                {       
                        led_char('x');
                        led_char('x');
                        last_number_showed = 9999;
                }
        }
        else
        {
                temp_val = temperatures[col][sens]/10;
                
                itoa(temp_val,buff);
                if(last_number_showed != temp_val)
                {
                        if(temp_val < 10)
                        {
                                led_char(buff[0]);
                                led_char('0');
                        }
                        else
                        {
                                led_char(buff[1]);
                                led_char(buff[0]);
                        }
                        
                        last_number_showed = temp_val;
                }
        }
        
        //printf("VAlor de col: %d   Valor de sens: %d \n\r",col,sens);
        
        
        
}

void read_info()
{

int i, j;

        printf("#%05d\n\r",cantidad_columnas);
        delay_ms(110);
        //calculate_humidity();
        
        for(i = 0; i < cantidad_columnas; i++)
        {
                printf("#%05d\n\r",cantidad_sensores[i]);
                delay_ms(110);
        }
                                              
        for(i = 0; i < cantidad_columnas; i++)
                for(j = 0; j < cantidad_sensores[i]; j++)
                {
                        printf("#%05d\n\r",temperatures[i][j]);
                        delay_ms(110);
                }
        
        
        
        printf("#0000000000000000\n\r");
        delay_ms(110);
        printf("#00000000000000000000000\n\r");


}

void detect_dallas(int number_expected)
{ 
        int i;
        
        
        for(i = 0;i < 2; i++)
        {
                ds1820_devices=w1_search(0xf0,ds1820_rom_codes);
                if(ds1820_devices == number_expected)
                        break;
        }
        
        /*for(i = 0; i < MAX_DS1820; i++)
        {
             ds1820_rom_codes_sorted[__ds18b20_scratch_pad.temp_low-1][0] = ds1820_rom_codes[i][0];
                        ds1820_rom_codes_sorted[i][1] = ds1820_rom_codes[i][1];
                        ds1820_rom_codes_sorted[i][2] = ds1820_rom_codes[i][2];
                        ds1820_rom_codes_sorted[i][3] = ds1820_rom_codes[i][3];
                        ds1820_rom_codes_sorted[i][4] = ds1820_rom_codes[i][4];
                        ds1820_rom_codes_sorted[i][5] = ds1820_rom_codes[i][5];
                        ds1820_rom_codes_sorted[i][6] = ds1820_rom_codes[i][6];
                        ds1820_rom_codes_sorted[i][7] = ds1820_rom_codes[i][7];
                        ds1820_rom_codes_sorted[i][8] = ds1820_rom_codes[i][8];
        }*/
        
        for(i = 0; i < ds1820_devices; i++)
        {
                ds18b20_read_spd(ds1820_rom_codes[i]);
                if(__ds18b20_scratch_pad.temp_low >= 1 && __ds18b20_scratch_pad.temp_low < 17)
                {
                        ds1820_rom_codes_sorted[__ds18b20_scratch_pad.temp_low-1][0] = ds1820_rom_codes[i][0];
                        ds1820_rom_codes_sorted[__ds18b20_scratch_pad.temp_low-1][1] = ds1820_rom_codes[i][1];
                        ds1820_rom_codes_sorted[__ds18b20_scratch_pad.temp_low-1][2] = ds1820_rom_codes[i][2];
                        ds1820_rom_codes_sorted[__ds18b20_scratch_pad.temp_low-1][3] = ds1820_rom_codes[i][3];
                        ds1820_rom_codes_sorted[__ds18b20_scratch_pad.temp_low-1][4] = ds1820_rom_codes[i][4];
                        ds1820_rom_codes_sorted[__ds18b20_scratch_pad.temp_low-1][5] = ds1820_rom_codes[i][5];
                        ds1820_rom_codes_sorted[__ds18b20_scratch_pad.temp_low-1][6] = ds1820_rom_codes[i][6];
                        ds1820_rom_codes_sorted[__ds18b20_scratch_pad.temp_low-1][7] = ds1820_rom_codes[i][7];
                        ds1820_rom_codes_sorted[__ds18b20_scratch_pad.temp_low-1][8] = ds1820_rom_codes[i][8];
                }
        }
}

void chose_channel(int channel)
{
        if(channel == 1)
        {
                MUX_A0 = 0;
                MUX_A1 = 0;
                MUX_A2 = 0;
                MUX_A3 = 0;
        }
        
        if(channel == 2)
        {
                MUX_A0 = 1;
                MUX_A1 = 0;
                MUX_A2 = 0;
                MUX_A3 = 0;
        }
        
        if(channel == 3)
        {
                MUX_A0 = 0;
                MUX_A1 = 1;
                MUX_A2 = 0;
                MUX_A3 = 0;
        }        
        
        if(channel == 4)
        {
                MUX_A0 = 1;
                MUX_A1 = 1;
                MUX_A2 = 0;
                MUX_A3 = 0;
        }
        
        if(channel == 5)
        {
                MUX_A0 = 0;
                MUX_A1 = 0;
                MUX_A2 = 1;
                MUX_A3 = 0;
        }                  
        
        if(channel == 6)
        {
                MUX_A0 = 1;
                MUX_A1 = 0;
                MUX_A2 = 1;
                MUX_A3 = 0;
        }                  
        
        if(channel == 7)
        {
                MUX_A0 = 0;
                MUX_A1 = 1;
                MUX_A2 = 1;
                MUX_A3 = 0;
        }
        
        if(channel == 8)
        {
                MUX_A0 = 1;
                MUX_A1 = 1;
                MUX_A2 = 1;
                MUX_A3 = 0;
        }
        
        if(channel == 9)
        {
                MUX_A0 = 1;
                MUX_A1 = 1;
                MUX_A2 = 1;
                MUX_A3 = 1;
        }
        
        if(channel == 10)
        {
                MUX_A0 = 0;
                MUX_A1 = 1;
                MUX_A2 = 1;
                MUX_A3 = 1;
        }
        
        if(channel == 11)
        {
                MUX_A0 = 1;
                MUX_A1 = 0;
                MUX_A2 = 1;
                MUX_A3 = 1;
        }                  
        
        if(channel == 12)
        {
                MUX_A0 = 0;
                MUX_A1 = 0;
                MUX_A2 = 1;
                MUX_A3 = 1;
        }
        
        if(channel == 13)
        {
                MUX_A0 = 1;
                MUX_A1 = 1;
                MUX_A2 = 0;
                MUX_A3 = 1;
        }                  
        
        if(channel == 14)
        {
                MUX_A0 = 0;
                MUX_A1 = 1;
                MUX_A2 = 0;
                MUX_A3 = 1;
        }
        
        if(channel == 15)
        {
                MUX_A0 = 1;
                MUX_A1 = 0;
                MUX_A2 = 0;
                MUX_A3 = 1;
        }                  
        
        if(channel == 16)
        {
                MUX_A0 = 0;
                MUX_A1 = 0;
                MUX_A2 = 0;
                MUX_A3 = 1;
        }
        
        MUX_ENABLE = 1;
                        
}

/*void chose_channel(int channel)
{
        if(channel == 1)
                PORTC.0 = 1;
        else
                PORTC.0 = 0;
                
        if(channel == 2)
                PORTC.2 = 1;
        else
                PORTC.2 = 0;
                
        if(channel == 3)
                PORTC.1 = 1;
        else
                PORTC.1 = 0;
                
        if(channel == 4)
                PORTC.3 = 1;
        else
                PORTC.3 = 0;
                
        if(channel == 5)
                PORTC.4 = 1;
        else
                PORTC.4 = 0;
                
        if(channel == 6)
                PORTC.6 = 1;
        else
                PORTC.6 = 0;
                
        if(channel == 7)
                PORTC.5 = 1;
        else
                PORTC.5 = 0;
                
        if(channel == 8)
                PORTC.7 = 1;
        else
                PORTC.7 = 0;
                
        if(channel == 9)
                PORTA.7 = 1;
        else
                PORTA.7 = 0;
                
        if(channel == 10)
                PORTA.5 = 1;
        else
                PORTA.5 = 0;
                
        if(channel == 11)
                PORTA.6 = 1;
        else
                PORTA.6 = 0;
                
        if(channel == 12)
                PORTA.4 = 1;
        else
                PORTA.4 = 0;
                
        if(channel == 13)
                PORTA.3 = 1;
        else
                PORTA.3 = 0;
                
        if(channel == 14)
                PORTA.1 = 1;
        else
                PORTA.1 = 0;
                
        if(channel == 15)
                PORTA.2 = 1;
        else
                PORTA.2 = 0;
                
        if(channel == 16)
                PORTA.0 = 1;
        else
                PORTA.0 = 0;
                        
}*/
