/*****************************************************
This program was produced by the
CodeWizardAVR V1.25.5 Professional
Automatic Program Generator
© Copyright 1998-2007 Pavel Haiduc, HP InfoTech s.r.l.
http://www.hpinfotech.com

Project : 
Version : 
Date    : 22/11/2008
Author  : F4CG                            
Company : F4CG                            
Comments: 


Chip type           : ATmega16
Program type        : Application
Clock frequency     : 8.000000 MHz
Memory model        : Small
External SRAM size  : 0
Data Stack size     : 256
*****************************************************/

#include <mega16.h>
#include <string.h>
#include <ctype.h>

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
#define MAX_DS1820 30
// number of DS1820 devices
// connected to the 1 Wire bus
unsigned char ds1820_devices;
// DS1820 devices ROM code storage area,
// 9 bytes are used for each device
// (see the w1_search function description in the help)
unsigned char ds1820_rom_codes[MAX_DS1820][9];

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
#define RX_BUFFER_SIZE 8
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
#define TX_BUFFER_SIZE 8
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

void send_temperatures(unsigned char devices)
{
        int i;
        
        printf("%02d\n\r",devices);
        
        for (i=0;i<devices;i++)
               printf("%+010d\n\r",(int)ds18b20_temperature(&ds1820_rom_codes[i][0]));

}

void read_di_do()
{       
        int i;       
        char buf[20];
        //sprintf(buf,"%02d\n\r",devices);
        
        
        /*for(j=0;j<devices;j++)
        {
                sprintf(buf,"%010d\n\r",PINB.0);
                for(i=0;i<10;i++)
                        putchar(buf[i]);
        }*/
        
        sprintf(buf,"%d%d%d%d%d%d%d%d%d%d00\n\r",~PINB.0,~PINB.1,~PINB.2,~PINB.3,~PINB.4,~PIND.2,~PIND.3,~PIND.4,~PIND.5,~PIND.7);
        for(i=0;i<14;i++)
                        putchar(buf[i]);
                        
        sprintf(buf,"%d%d%d%d%d%d%d%d%d%d%d%d\n\r",PORTA.4,PORTA.5,PORTA.6,PORTA.7,PORTC.7,PORTC.6,PORTC.5,PORTC.4,PORTC.3,PORTC.2,PORTC.1,PORTC.0);
        for(i=0;i<14;i++)
                        putchar(buf[i]);
}

void show_serials(unsigned char devices)
{
        int i,j;
        char tempBuff[20];
        
        sprintf(tempBuff,"%02d\n\r",devices);
        
        for (i=0; i < strlen(tempBuff); i++)
        {
                putchar(tempBuff[i]);
                        
        };
        
        /*for(i=0;i<devices;i++)
        {
                //temp=ds1820_temperature_10(&rom_codes[i,0]);
                sprintf(tempBuff,"%p\n\r", rom_codes[i,0]);
                for (j=0; j < sizeof(rom_codes); j++)
                {
                        putchar(rom_codes[i,j]);
                        
                };
                putchar('\n');
                putchar('\r');
        };*/
        for (i=0;i<devices;i++)
        {
                //printf("DEVICE #%-u ROM CODE IS:", i+1);

                for (j=0;j<8;j++)
                        printf("%02X",ds1820_rom_codes[i][j]);
                
                printf("\n\r");

       };
       /*for(i=0;i<devices;i++)
        {
                //binary_to_hexascii(uint8 hex_buf[], uint8 bin_buf[], int n_bin)
                k=binary_to_hexascii(tempBuff,rom_codes[i], 13);
                //sprintf(tempBuff,"%s\n\r", rom_codes[i,0]);
                for (j=0; j < k; j++)
                {
                        putchar(tempBuff[j]);
                        
                };
                putchar('\n');
                putchar('\r');
        };*/
       
}

void write_do(char ch1, char ch2, char val)
{
        if(isdigit(ch1) != 1 || isdigit(ch2) != 1 || ( val != '1' && val !='0'))
        {
                putchar('0');
                putchar('1');
                putchar('\n');
                putchar('\r');
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
                                        putchar('0');
                                        putchar('3');
                                        putchar('\n');
                                        putchar('\r');
                                        return;
                                }
                }
                else
                {
                        putchar('0');
                        putchar('3');
                        putchar('\n');
                        putchar('\r');
                        return;
                }
        
        putchar('O');
        putchar('K');
        putchar('\n');
        putchar('\r');
}

void test(unsigned char devices)
{       
        int k,j;
        char temp_buf[15];
        //float temp;
        int temp;
        
        k = 0;
        
        if(devices > 0)
                while(k < 30)
                {
                        temp=ds18b20_temperature(&ds1820_rom_codes[0][0]);
                        sprintf(temp_buf, "%d:%+010d\n\r", k, temp);
                        for (j=0; j < strlen(temp_buf); j++)
                        {
                                putchar(temp_buf[j]);
                        
                        };
                        k++;
                }
                
}

void main(void)
{
// Declare your local variables here
char letra, ch1, ch2, val;
int time_out;
//int i;


// Input/Output Ports initialization
// Port A initialization
// Func7=In Func6=In Func5=In Func4=In Func3=In Func2=In Func1=In Func0=In 
// State7=T State6=T State5=T State4=T State3=T State2=T State1=T State0=T 
PORTA=0x00;
DDRA=0xF0;

// Port B initialization
// Func7=In Func6=In Func5=In Func4=In Func3=In Func2=In Func1=In Func0=In 
// State7=T State6=T State5=T State4=T State3=T State2=T State1=T State0=T 
PORTB=0x00;
DDRB=0x00;

// Port C initialization
// Func7=In Func6=In Func5=In Func4=In Func3=In Func2=In Func1=In Func0=In 
// State7=T State6=T State5=T State4=T State3=T State2=T State1=T State0=T 
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

w1_init();
ds1820_devices=w1_search(0xf0,ds1820_rom_codes);
/*for(i=0; i<ds1820_devices; i++)
        if(ds18b20_init(&ds1820_rom_codes[i][0],25,35,DS18B20_9BIT_RES))
        
                printf("Correctamente inicializado\n\r");
        
        else
                printf("Fallo al inicializar\n\r");    
 */       

letra = '#';
time_out = 0;

// Global enable interrupts
#asm("sei")

//rx_counter = 0;

while (1)
      {
      // Place your code here
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
                                putchar('2');
                                putchar('\n');
                                putchar('\r');
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
               if(letra == 'T' || letra == 't')
               {
                        test(ds1820_devices);
                        letra = '#';
               }
      };
}
