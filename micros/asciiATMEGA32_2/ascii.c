/*****************************************************
This program was produced by the
CodeWizardAVR V1.25.5 Professional
Automatic Program Generator
© Copyright 1998-2007 Pavel Haiduc, HP InfoTech s.r.l.
http://www.hpinfotech.com

Project : 
Version : 
Date    : 13/02/2009
Author  : F4CG                            
Company : F4CG                            
Comments: 


Chip type           : ATmega32
Program type        : Application
Clock frequency     : 8.000000 MHz
Memory model        : Small
External SRAM size  : 0
Data Stack size     : 512
*****************************************************/

#include <mega32.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

// 1 Wire Bus functions
#asm
   .equ __w1_port=0x1B ;PORTA
   .equ __w1_bit=0
#endasm
#include <1wire.h>

// DS1820 Temperature Sensor functions
#include <ds18b20.h>

// maximum number of DS1820 devices
// connected to the 1 Wire bus
#define MAX_DS1820 8
// number of DS1820 devices
// connected to the 1 Wire bus
unsigned char ds1820_devices;
// DS1820 devices ROM code storage area,
// 9 bytes are used for each device
// (see the w1_search function description in the help)
unsigned char ds1820_rom_codes[MAX_DS1820][9];

#define MAX_TIME 90000

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

// USART Receiver buffer
#define RX_BUFFER_SIZE 32
char rx_buffer[RX_BUFFER_SIZE];

#if RX_BUFFER_SIZE<256
unsigned char rx_wr_index,rx_rd_index,rx_counter;
#else
unsigned int rx_wr_index,rx_rd_index,rx_counter;
#endif

// This flag is set on USART Receiver buffer overflow
bit rx_buffer_overflow;

// USART Receiver interrupt service routine
interrupt [USART_RXC] void usart_rx_isr(void)
{
char status,data;
status=UCSRA;
data=UDR;
if ((status & (FRAMING_ERROR | PARITY_ERROR | DATA_OVERRUN))==0)
   {
   rx_buffer[rx_wr_index]=data;
   if (++rx_wr_index == RX_BUFFER_SIZE) rx_wr_index=0;
   if (++rx_counter == RX_BUFFER_SIZE)
      {
      rx_counter=0;
      rx_buffer_overflow=1;
      };
   };
}

#ifndef _DEBUG_TERMINAL_IO_
// Get a character from the USART Receiver buffer
#define _ALTERNATE_GETCHAR_
#pragma used+
char getchar(void)
{
char data;
while (rx_counter==0);
data=rx_buffer[rx_rd_index];
if (++rx_rd_index == RX_BUFFER_SIZE) rx_rd_index=0;
#asm("cli")
--rx_counter;
#asm("sei")
return data;
}
#pragma used-
#endif

// USART Transmitter buffer
#define TX_BUFFER_SIZE 24
char tx_buffer[TX_BUFFER_SIZE];

#if TX_BUFFER_SIZE<256
unsigned char tx_wr_index,tx_rd_index,tx_counter;
#else
unsigned int tx_wr_index,tx_rd_index,tx_counter;
#endif

// USART Transmitter interrupt service routine
interrupt [USART_TXC] void usart_tx_isr(void)
{
if (tx_counter)
   {
   --tx_counter;
   UDR=tx_buffer[tx_rd_index];
   if (++tx_rd_index == TX_BUFFER_SIZE) tx_rd_index=0;
   };
}

#ifndef _DEBUG_TERMINAL_IO_
// Write a character to the USART Transmitter buffer
#define _ALTERNATE_PUTCHAR_
#pragma used+
void putchar(char c)
{
while (tx_counter == TX_BUFFER_SIZE);
#asm("cli")
if (tx_counter || ((UCSRA & DATA_REGISTER_EMPTY)==0))
   {
   tx_buffer[tx_wr_index]=c;
   if (++tx_wr_index == TX_BUFFER_SIZE) tx_wr_index=0;
   ++tx_counter;
   }
else
   UDR=c;
#asm("sei")
}
#pragma used-
#endif

// Standard Input/Output functions
#include <stdio.h>

// Declare your global variables here
eeprom int garbage;
eeprom unsigned int dallas_number;
eeprom unsigned int dallas_last_number;
unsigned int temperature_fail;
int timer;



void send_temperatures(unsigned char devices)
{
        int i, temp;
        
        printf("%02d\n\r",devices);
        
        for (i=0;i<devices;i++)
        {
                temp = (int)ds18b20_temperature(&ds1820_rom_codes[i][0]);
                if(temp == -9999)
                        temperature_fail = 1;
                printf("%+010d\n\r",);
        }

}

void read_di_do()
{       
        printf("%d%d%d%d%d%d%d%d%d%d%d%d%d\n\r",~PINB.0,~PINB.1,~PINB.2,~PINB.3,~PINB.4,~PIND.2,~PIND.3,~PIND.4,~PIND.5,~PIND.7,~PINA.1,~PINA.2,~PINA.3);
                        
        printf("%d%d%d%d%d%d%d%d%d%d%d%d\n\r",PORTA.4,PORTA.5,PORTA.6,PORTA.7,PORTC.7,PORTC.6,PORTC.5,PORTC.4,PORTC.3,PORTC.2,PORTC.1,PORTC.0);
}

/*!
  Send two characters representing th number of detected one wire devices followed by the serial number of each one
*/

void show_serials(unsigned char devices)
{
        int i,j;

        printf("%02d\n\r",devices);
        for (i=0;i<devices;i++)
        {
                //printf("DEVICE #%-u ROM CODE IS:", i+1);

                for (j=0;j<8;j++)
                        printf("%02X",ds1820_rom_codes[i][j]);
                
                printf("\n\r");

       };
}

/*!
  Escribe las salidas discretas
*/

void write_do(char ch1, char ch2, char val)
{
        if(isdigit(ch1) != 1 || isdigit(ch2) != 1 || ( val != '1' && val !='0'))
        {
                printf("01\n\r");
                return;
        }

        if(ch1 == '0')
        {
                if(ch2 == '0')
                {
                        if(val == '0')
                                PORTA.4 = 0;
                        else
                                PORTA.4 = 1;
                }
                if(ch2 == '1')
                {
                        if(val == '0')
                                PORTA.5 = 0;
                        else
                                PORTA.5 = 1;
                }
                if(ch2 == '2')
                {
                        if(val == '0')
                                PORTA.6 = 0;
                        else
                                PORTA.6 = 1;
                }
                if(ch2 == '3')
                {
                        if(val == '0')
                                PORTA.7 = 0;
                        else
                                PORTA.7 = 1;
                }
                if(ch2 == '4')
                {
                        if(val == '0')
                                PORTC.7 = 0;
                        else
                                PORTC.7 = 1;
                }
                if(ch2 == '5')
                {
                        if(val == '0')
                                PORTC.6 = 0;
                        else
                                PORTC.6 = 1;
                }
                if(ch2 == '6')
                {
                        if(val == '0')
                                PORTC.5 = 0;
                        else
                                PORTC.5 = 1;
                }
                if(ch2 == '7')
                {
                        if(val == '0')
                                PORTC.4 = 0;
                        else
                                PORTC.4 = 1;
                }
                if(ch2 == '8')
                {
                        if(val == '0')
                                PORTC.3 = 0;
                        else
                                PORTC.3 = 1;
                }
                if(ch2 == '9')
                {
                        if(val == '0')
                                PORTC.2 = 0;
                        else
                                PORTC.2 = 1;
                }
        }
        else
                if(ch1 == '1')
                {
                        if(ch2 == '0')
                        {
                                if(val == '0')
                                        PORTC.1 = 0;
                                else
                                        PORTC.1 = 1;
                        }
                        else
                                if(ch2 == '1')
                                {
                                        if(val == '0')
                                                PORTC.0 = 0;
                                        else
                                                PORTC.0 = 1;
                                }
                                else
                                {
                                        printf("03\n\r");
                                        return;
                                }
                }
                else
                {
                        printf("03\n\r");
                        return;
                }
        printf("OK\n\r");
}

void set_dallas(char ch1, char ch2)
{
        if(isdigit(ch1) != 1 || isdigit(ch2) != 1)
        {
                printf("01\n\r");
                return;
        }
        dallas_number = ((ch1-48) * 10) + (ch2-48);
}

void give_dallas()
{
        printf("%d\n\r",dallas_number);
        printf("%d\n\r",dallas_last_number);
        printf("%d\n\r",ds1820_devices);
}

//!  El main
/*!
*  Aqui inician todas las tonterias
*/


void main(void)
{
// Declare your local variables here
char letra, ch1, ch2, val;
int time_out;
int i;    

// Input/Output Ports initialization
// Port A initialization
// Func7=Out Func6=Out Func5=Out Func4=Out Func3=In Func2=In Func1=In Func0=In 
// State7=0 State6=0 State5=0 State4=0 State3=P State2=P State1=P State0=T 
PORTA=0x0E;
DDRA=0xF0;

// Port B initialization
// Func7=In Func6=In Func5=In Func4=In Func3=In Func2=In Func1=In Func0=In 
// State7=T State6=T State5=T State4=T State3=T State2=T State1=T State0=T 
PORTB=0x00;
DDRB=0x00;

// Port C initialization
// Func7=Out Func6=Out Func5=Out Func4=Out Func3=Out Func2=Out Func1=Out Func0=Out 
// State7=0 State6=0 State5=0 State4=0 State3=0 State2=0 State1=0 State0=0 
PORTC=0x00;
DDRC=0xFF;

// Port D initialization
// Func7=In Func6=In Func5=In Func4=In Func3=In Func2=In Func1=In Func0=In 
// State7=T State6=T State5=T State4=T State3=T State2=T State1=T State0=T 
PORTD=0x00;
DDRD=0x00;

// Timer/Counter 0 initialization
// Clock source: System Clock
// Clock value: Timer 0 Stopped
// Mode: Normal top=FFh
// OC0 output: Disconnected
TCCR0=0x00;
TCNT0=0x00;
OCR0=0x00;

// Timer/Counter 1 initialization
// Clock source: System Clock
// Clock value: Timer 1 Stopped
// Mode: Normal top=FFFFh
// OC1A output: Discon.
// OC1B output: Discon.
// Noise Canceler: Off
// Input Capture on Falling Edge
// Timer 1 Overflow Interrupt: Off
// Input Capture Interrupt: Off
// Compare A Match Interrupt: Off
// Compare B Match Interrupt: Off
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

// Timer/Counter 2 initialization
// Clock source: System Clock
// Clock value: Timer 2 Stopped
// Mode: Normal top=FFh
// OC2 output: Disconnected
ASSR=0x00;
TCCR2=0x00;
TCNT2=0x00;
OCR2=0x00;

// External Interrupt(s) initialization
// INT0: Off
// INT1: Off
// INT2: Off
MCUCR=0x00;
MCUCSR=0x00;

// Timer(s)/Counter(s) Interrupt(s) initialization
TIMSK=0x00;

// USART initialization
// Communication Parameters: 8 Data, 1 Stop, No Parity
// USART Receiver: On
// USART Transmitter: On
// USART Mode: Asynchronous
// USART Baud Rate: 9600
UCSRA=0x00;
UCSRB=0xD8;
UCSRC=0x86;
UBRRH=0x00;
UBRRL=0x33;

// Analog Comparator initialization
// Analog Comparator: Off
// Analog Comparator Input Capture by Timer/Counter 1: Off
ACSR=0x80;
SFIOR=0x00;

// Determine the number of DS1820 devices
// connected to the 1 Wire bus
ds1820_devices=w1_search(0xf0,ds1820_rom_codes);

// Global enable interrupts
#asm("sei")

letra = '#';
time_out = 0;
temperature_fail = 0;
timer = 0;

for (i=0;i<ds1820_devices;i++)
    {
        ds18b20_init(&ds1820_rom_codes[i][0],25,35,DS18B20_9BIT_RES);
    };


while (1)
      {
      // Place your code here
                if(temperature_fail == 1)
                        timer++;
      
                if(rx_counter > 0)
                        letra = getchar();
               if(letra == 'R' || letra == 'r')
               {
                 send_temperatures(ds1820_devices);
                 read_di_do();
                 letra = '#';
               }
               if(letra == 'S' || letra == 's')
               {
                        show_serials(ds1820_devices); 
                        letra = '#';
               }
               
               if(letra == 'W' || letra == 'w')
               {
                        while(rx_counter < 3 && time_out < 900000)
                                time_out++;
                        
                        if(time_out == 900000)
                        {
                                printf("2\n\r");
                        }
                        else
                        {
                                ch1 = getchar();
                                ch2 = getchar();
                                val = getchar();
                                write_do(ch1,ch2,val);
                        }

                        time_out = 0;
                        letra = '#';
               }
               if(letra == 'N' || letra == 'n')
               {
                        while(rx_counter < 3 && time_out < 900000)
                                time_out++;
                        
                        if(time_out == 900000)
                        {
                                printf("2\n\r");
                        }
                        else
                        {
                                ch1 = getchar();
                                ch2 = getchar();
                                set_dallas(ch1,ch2);
                        }

                        time_out = 0;
                        letra = '#';
               }
               if(letra == 'G' || letra == 'g')
               {
                        give_dallas(); 
                        letra = '#';
               }
               if(letra == 'D' || letra == 'd')
               {
                        dallas_last_number = ds1820_devices;
                        ds1820_devices = w1_search(0xf0,ds1820_rom_codes);
                        if(ds1820_devices == dallas_number)
                        {
                                timer = 0;
                                temperature_fail = 0;
                        }
                        letra = '#';
               }
               if(timer == MAX_TIME)
               {
                        dallas_last_number = ds1820_devices;
                        ds1820_devices = w1_search(0xf0,ds1820_rom_codes);
                        if(ds1820_devices == dallas_number)
                                timer = 0;
               }
      };
}
