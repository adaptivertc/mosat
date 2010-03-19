/*****************************************************
This program was produced by the
CodeWizardAVR V1.25.5 Professional
Automatic Program Generator
© Copyright 1998-2007 Pavel Haiduc, HP InfoTech s.r.l.
http://www.hpinfotech.com

Project : 
Version : 
Date    : 25/01/2010
Author  : F4CG                            
Company : F4CG                            
Comments: 


Chip type           : ATmega32
Program type        : Application
Clock frequency     : 16.000000 MHz
Memory model        : Small
External SRAM size  : 0
Data Stack size     : 512
*****************************************************/

#include <mega32.h>

// I2C Bus functions
#asm
   .equ __i2c_port=0x12 ;PORTD
   .equ __sda_bit=4
   .equ __scl_bit=3
#endasm
#include <i2c.h>

// PCF8583 Real Time Clock functions
#include <pcf8583.h>

// 1 Wire Bus functions
#asm
   .equ __w1_port=0x12 ;PORTD
   .equ __w1_bit=5
#endasm
#include <1wire.h>

// DS1820 Temperature Sensor functions
#include <ds18b20.h>

// maximum number of DS1820 devices
// connected to the 1 Wire bus
#define MAX_DS1820 32
// number of DS1820 devices
// connected to the 1 Wire bus
unsigned char ds1820_devices;
// DS1820 devices ROM code storage area,
// 9 bytes are used for each device
// (see the w1_search function description in the help)
unsigned char ds1820_rom_codes[MAX_DS1820][9];
unsigned char ds1820_rom_codes_no_order[MAX_DS1820][9];

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
#define RX_BUFFER_SIZE 128
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
#define TX_BUFFER_SIZE 128
char tx_buffer[TX_BUFFER_SIZE];

#if TX_BUFFER_SIZE<256
volatile unsigned char tx_wr_index,tx_rd_index,tx_counter;
#else
volatile unsigned int tx_wr_index,tx_rd_index,tx_counter;
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

#include <delay.h>

unsigned int adc_data;
#define ADC_VREF_TYPE 0x00

// ADC interrupt service routine
interrupt [ADC_INT] void adc_isr(void)
{
// Read the AD conversion result
adc_data=ADCW;
}

// Read the AD conversion result
// with noise canceling
unsigned int read_adc(unsigned char adc_input)
{
ADMUX=adc_input | (ADC_VREF_TYPE & 0xff);
// Delay needed for the stabilization of the ADC input voltage
delay_us(10);
#asm
    in   r30,mcucr
    cbr  r30,__sm_mask
    sbr  r30,__se_bit | __sm_adc_noise_red
    out  mcucr,r30
    sleep
    cbr  r30,__se_bit
    out  mcucr,r30
#endasm
return adc_data;
}

// Declare your global variables here
#include<stdlib.h> 
#include "rt_modbus_crc.h"
#include "mod_server_msg.h"

#define TIME_OUT 30000

eeprom int garbage;


void again()
{
int i;

ds1820_devices=w1_search(0xf0,ds1820_rom_codes_no_order);

        for(i = 0; i < ds1820_devices; i++)
        {
                ds18b20_read_spd(ds1820_rom_codes_no_order[i]);
                ds1820_rom_codes[__ds18b20_scratch_pad.temp_low-1][0] = ds1820_rom_codes_no_order[i][0];
                ds1820_rom_codes[__ds18b20_scratch_pad.temp_low-1][1] = ds1820_rom_codes_no_order[i][1];
                ds1820_rom_codes[__ds18b20_scratch_pad.temp_low-1][2] = ds1820_rom_codes_no_order[i][2];
                ds1820_rom_codes[__ds18b20_scratch_pad.temp_low-1][3] = ds1820_rom_codes_no_order[i][3];
                ds1820_rom_codes[__ds18b20_scratch_pad.temp_low-1][4] = ds1820_rom_codes_no_order[i][4];
                ds1820_rom_codes[__ds18b20_scratch_pad.temp_low-1][5] = ds1820_rom_codes_no_order[i][5];
                ds1820_rom_codes[__ds18b20_scratch_pad.temp_low-1][6] = ds1820_rom_codes_no_order[i][6];
                ds1820_rom_codes[__ds18b20_scratch_pad.temp_low-1][7] = ds1820_rom_codes_no_order[i][7];
                ds1820_rom_codes[__ds18b20_scratch_pad.temp_low-1][8] = ds1820_rom_codes_no_order[i][8];
        }
}

void flush_usart()
{
	while(rx_counter > 0)
		getchar();
}


void main(void)
{
// Declare your local variables here
char letra;
unsigned char hora, minuto, segundo;
char buff[3];
char status_uart;

uint8_t unit_id;
uint8_t buf[128];
unsigned int timer;
int min_size;
int total_size;
int n;
int i;
int buf_index;
int reply_size;


// Input/Output Ports initialization
// Port A initialization
// Func7=In Func6=In Func5=In Func4=In Func3=In Func2=In Func1=In Func0=In 
// State7=T State6=T State5=T State4=T State3=T State2=T State1=T State0=T 
PORTA=0x00;
DDRA=0x00;

// Port B initialization
// Func7=Out Func6=Out Func5=Out Func4=Out Func3=Out Func2=Out Func1=Out Func0=Out 
// State7=0 State6=0 State5=0 State4=0 State3=0 State2=0 State1=0 State0=0 
PORTB=0x00;
DDRB=0xFF;

// Port C initialization
// Func7=In Func6=In Func5=In Func4=In Func3=In Func2=In Func1=In Func0=In 
// State7=P State6=P State5=P State4=P State3=P State2=P State1=P State0=P 
PORTC=0xFF;
DDRC=0x00;

// Port D initialization
// Func7=In Func6=In Func5=In Func4=In Func3=In Func2=In Func1=In Func0=In 
// State7=T State6=T State5=T State4=T State3=T State2=T State1=T State0=T 
PORTD=0x00;
DDRD=0x40;

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
UBRRL=0x67;

// Analog Comparator initialization
// Analog Comparator: Off
// Analog Comparator Input Capture by Timer/Counter 1: Off
ACSR=0x80;
SFIOR=0x00;

// ADC initialization
// ADC Clock frequency: 1000.000 kHz
// ADC Voltage Reference: AREF pin
ADMUX=ADC_VREF_TYPE & 0xff;
ADCSRA=0x8C;

// I2C Bus initialization
i2c_init();

// PCF8583 Real Time Clock initialization
rtc_init(0,0);

// Determine the number of DS1820 devices
// connected to the 1 Wire bus
//ds1820_devices=w1_search(0xf0,ds1820_rom_codes_no_order);
again();

// Global enable interrupts
#asm("sei")

letra = '#';
PORTD.6 = 0;
unit_id = 1;
buf_index = 0;

while (1)
      {
      // Place your code here
      if( tx_counter == 0)
      {
        delay_us(900);
        PORTD.6 = 0;
      }
      
      if(rx_counter > 0)
         {
         	timer = 0;
         	buf_index = 0;
         	
         	
         	if(PORTB.0 == 0)
                        PORTB.0 = 1;
                else
                        PORTB.0 = 0;
         	
         	while(rx_counter < 2)
         	{
                	timer++;
                	if(timer == TIME_OUT)
                        	break;
         	}
         	if(timer == TIME_OUT && rx_counter < 2)
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
         		
         		if(PORTB.1 == 0)
                                PORTB.1 = 1;
                        else
                                PORTB.1 = 0;
         		
         		timer = 0;
         		
         		while(rx_counter < min_size - 2)
         		{
         		        timer++;
         		        if(timer == TIME_OUT)
         		                break;
         		}
         		
         		if(timer == TIME_OUT && rx_counter < min_size - 2)
         		{
         		        flush_usart();
         		        buf_index = 0;
         		}
         		else
         		{
         		        for(i = 0; i < min_size - 2; i++)
         		                buf[buf_index++] = getchar();
         		        
         		        total_size = rt_modbus_total_bytes(buf, min_size);
         		        
         		        
                                PORTB.2 = ~PORTB.2;
         		        
         		        if(total_size > min_size)
         		        {
         		                timer = 0;
         		                PORTB.3 = ~PORTB.3;
         		                while(rx_counter < total_size - min_size)
         		                {
         		                        timer++;
         		                        if(timer == TIME_OUT)
         		                                break;
         		                }
         		                
         		                if(timer == TIME_OUT && rx_counter < total_size - min_size)
         		                {
         		                  flush_usart();
         		                  buf_index = 0;
         		                }
         		                else
         		                {
         		                          for(i = 0; i < total_size - min_size; i++)
         		                                  buf[buf_index++] = getchar();
         		                          
         		                          PORTB.4 = ~PORTB.4;
         		                }
         		        }
         		        
         		        if (unit_id != buf[0])
         		        {
         		                PORTB.7 = ~PORTB.7;
                                        continue;
                                }
                                
                                PORTD.6 = 1;
                                
                                //printf("test");
                                
         		        reply_size = rt_modbus_process_request(buf, total_size);
         		        
         		        add_CRC(buf, reply_size, 0xffff);
         		        
         		        PORTB.5 = ~PORTB.5;
         		        
         		        //0102035555559731
         		        //printf("%c%c%c%c%c%c%c%c",0x01,0x02,0x03,0x55,0x55,0x55,0x97,0x31);
         		        
         		        for(i = 0; i < reply_size; i++)
         		                putchar(buf[i]);
         		        
         		        PORTB.6 = ~PORTB.6;
         		                //printf("%c",buf[i]);
         		}
         	}
         }
      
      
      
      
      
      
      
      /*if( tx_counter == 0)
      {
        //status_uart = UCSRA;
        //if(!((status_uart & DATA_REGISTER_EMPTY)==0))
        delay_us(900);
        PORTD.6 = 0;
      }
      
      if(rx_counter > 0)
      {        
                letra = getchar();
                PORTD.6 = 1;
                if(letra == 't')
                {
                        printf("TEST\n\r");
                        letra = '#';
                }
                else
                if(letra == 'm')
                {
                        printf("Hola mundo\n\r");
                        letra = '#';
                }
                else
                {
                        printf("otra cosa %c   ",'g');
                        putchar('y');
                        putchar(letra);
                        letra = '#';
                }
                
                
      }
      
      //printf("FER ");
      
      //rtc_set_time(0,16,15,0,0);
      
      /*rtc_get_time(0,&hora,&minuto,&segundo,0);
      
      if( tx_counter == 0)
      PORTD.6 = 0;
      
      
      if(rx_counter > 0)
      {
        letra = getchar();
        
        if(letra == 't')
        {
                PORTD.6 = 1;
                printf("\n\r%u:%u:%u\n\r",hora,minuto,segundo);
                letra = '#';
                
        }
        if(letra == 'a')
        {  
                PORTD.6 = 1;
                printf("\n\r%u\n\r%u\n\r%u\n\r%u\n\r%u\n\r%u\n\r%u\n\r%u\n\r",read_adc(0),read_adc(1),read_adc(2),read_adc(3),read_adc(4),read_adc(5),read_adc(6),read_adc(7));
                letra = '#';
                
        }
        if(letra == '0')
        {
                if(PORTB.0 == 0)
                        PORTB.0 = 1;
                else
                        PORTB.0 = 0;
                
                letra = '#';
        }
        if(letra == '1')
        {
                if(PORTB.1 == 0)
                        PORTB.1 = 1;
                else
                        PORTB.1 = 0;
                
                letra = '#';
        }
        if(letra == '2')
        {
                if(PORTB.2 == 0)
                        PORTB.2 = 1;
                else
                        PORTB.2 = 0;
                
                letra = '#';
        }
        if(letra == '3')
        {
                if(PORTB.3 == 0)
                        PORTB.3 = 1;
                else
                        PORTB.3 = 0;
                
                letra = '#';
        }
        if(letra == '4')
        {
                if(PORTB.4 == 0)
                        PORTB.4 = 1;
                else
                        PORTB.4 = 0;
                
                letra = '#';
        }
        if(letra == '5')
        {
                if(PORTB.5 == 0)
                        PORTB.5 = 1;
                else
                        PORTB.5 = 0;
                
                letra = '#';
        }
        if(letra == '6')
        {
                if(PORTB.6 == 0)
                        PORTB.6 = 1;
                else
                        PORTB.6 = 0;
                
                letra = '#';
        }
        if(letra == '7')
        {
                if(PORTB.7 == 0)
                        PORTB.7 = 1;
                else
                        PORTB.7 = 0;
                
                letra = '#';
        }
        if(letra == 'r')
        {
                PORTD.6 = 1;
                printf("\n\r%d%d%d%d%d%d%d%d\n\r",PINC.0,PINC.1,PINC.2,PINC.3,PINC.4,PINC.5,PINC.6,PINC.7,);
                //while(rx_counter > 0)
                letra = '#';
                //PORTD.6 = 0;
        }
        if(letra == 'd')
        {
                PORTD.6 = 1;
                for(i = 0; i < 32; i++)
                        printf("Sensor %i: %f\r",i+1,ds18b20_temperature(ds1820_rom_codes[i]));
                //while(rx_counter > 0)
                letra = '#';
                //delay_ms(1000);
                //PORTD.6 = 0;
        }
        if(letra == 's')
        {
                PORTD.6 = 1;
                printf("\n\rDame la hora(dos digitos)\n\r");
                PORTD.6 = 0;
                buf[0] = getchar();
                buf[1] = getchar();
                buf[2] = '\0';
                hora = atoi(buf);
                PORTD.6 = 1;
                printf("\n\rDame los segundos(dos digitos)\n\r");
                PORTD.6 = 0;
                buf[0] = getchar();
                buf[1] = getchar();
                buf[2] = '\0';
                minuto = atoi(buf);
                letra = '#';
                PORTD.6 = 1;
                printf("\n\rPresiona una tecla para ajustar el reloj\n\r");
                PORTD.6 = 0;
                getchar();
                rtc_set_time(0,hora,minuto,0,0);
        }
        
        if(letra == 'j')
        {       
                PORTD.6 = 1;
                ds18b20_read_spd(0);
                printf("%u",__ds18b20_scratch_pad.temp_low);
                
                letra = '#';
        }
        if(letra == 'p')
        {       
                buf[0] = getchar();
                buf[1] = getchar();
                buf[2] = '\0';
                minuto = atoi(buf);
                ds18b20_init(0,minuto,0,DS18B20_10BIT_RES);
                //printf("%u",__ds18b20_scratch_pad);
                
                letra = '#';
        }
        if(letra == 'n')
        {
                PORTD.6 = 1;
                printf("Numero de dispositivos: %u\n\r",ds1820_devices);
                letra = '#';
        }
        if(letra == 'b')
        {
                again();
                letra = '#';
        }
        
      }*/
      
      };
}
