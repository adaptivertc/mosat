/*****************************************************
This program was produced by the
CodeWizardAVR V1.25.5 Professional
Automatic Program Generator
© Copyright 1998-2007 Pavel Haiduc, HP InfoTech s.r.l.
http://www.hpinfotech.com

Project : 
Version : 
Date    : 02/09/2009
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
#define TX_BUFFER_SIZE 32
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

#include <delay.h>

#define FIRST_ADC_INPUT 0
#define LAST_ADC_INPUT 0
unsigned int adc_data[LAST_ADC_INPUT-FIRST_ADC_INPUT+1];
#define ADC_VREF_TYPE 0x00

// ADC interrupt service routine
// with auto input scanning
interrupt [ADC_INT] void adc_isr(void)
{
register static unsigned char input_index=0;
// Read the AD conversion result
adc_data[input_index]=ADCW;
// Select next ADC input
if (++input_index > (LAST_ADC_INPUT-FIRST_ADC_INPUT))
   input_index=0;
ADMUX=(FIRST_ADC_INPUT | (ADC_VREF_TYPE & 0xff))+input_index;
// Delay needed for the stabilization of the ADC input voltage
delay_us(10);
// Start the AD conversion
ADCSRA|=0x40;
}

// Declare your global variables here
#define BUZZER_1 7
#define MAXBUZZ 5
#define MAXTICK 1000
#define AMPERS 200
#define FACTOR_AMPERS 3464   //200 * sqrt(3) * 10

eeprom int garbage;
eeprom int defaults;
int current_limits[4];
eeprom int error;
int current_index;
long actual_current;
unsigned int raw_current;
//int do_channel;
int tick;
int buzz_top;
int buzzers[MAXBUZZ];
unsigned int low_limit;
unsigned int low_normal_limit;
unsigned int normal_limit;
unsigned int high_normal_limit;
unsigned int high_limit;
unsigned int very_high_limit;

void set_current_index()
{
        if(~PINC.2 == 0)
        {
                if(~PINC.3 == 0)
                        current_index = 0;
                else
                        current_index = 1;
        }
        else
        {
                if(~PINC.3 == 0)
                        current_index = 2;
                else
                        current_index = 3;
        }
        low_limit = current_limits[current_index] - 400;
        low_normal_limit = current_limits[current_index] - 200;
        normal_limit = current_limits[current_index];
        high_normal_limit = current_limits[current_index] + 200;
        high_limit = current_limits[current_index] + 400;
        very_high_limit = current_limits[current_index] + 600;
        //printf("Current index: %d\n\r",current_index);
        //printf("Current limits %d: %d\n\r",current_index,current_limits[current_index]);
        //delay_ms(2000);
}

void inicializar()
{
int i;

    current_limits[0] = 1400;
    current_limits[1] = 1600;
    current_limits[2] = 1800;
    current_limits[3] = 2000;
    set_current_index();
    tick = 0;
    for(i = 0; i < MAXBUZZ; i++)
            buzzers[i] = -1;
    buzz_top = -1;
                defaults = 0;
} 

void write_do(int do_channel, char val)
{
        if(val != '1' && val !='0')
                return;

        if(do_channel == 0)
                if(val == '0')
                        PORTB.0 = 0;
                else
                        PORTB.0 = 1;
        else
        if(do_channel == 1)
                if(val == '0')
                        PORTB.1 = 0;
                else
                        PORTB.1 = 1;
        else
        if(do_channel == 2)
                if(val == '0')
                        PORTB.2 = 0;
                else
                        PORTB.2 = 1;
        else
        if(do_channel == 3)
                if(val == '0')
                        PORTB.3 = 0;
                else
                        PORTB.3 = 1;
        else
        if(do_channel == 4)
                if(val == '0')
                        PORTB.4 = 0;
                else
                        PORTB.4 = 1;
        else
        if(do_channel == 5)
                if(val == '0')
                        PORTB.5 = 0;
                else
                        PORTB.5 = 1;
        else
        if(do_channel == 6)
                if(val == '0')
                        PORTB.6 = 0;
                else
                        PORTB.6 = 1;
        else
        if(do_channel == 7)
                if(val == '0')
                        PORTB.7 = 0;
                else
                        PORTB.7 = 1;
        else
        if(do_channel == 8)
                if(val == '0')
                        PORTD.2 = 0;
                else
                        PORTD.2 = 1;
        else
        if(do_channel == 9)
                if(val == '0')
                        PORTD.3 = 0;
                else
                        PORTD.3 = 1;
        else
        if(do_channel == 10)
                if(val == '0')
                        PORTD.4 = 0;
                else
                        PORTD.4 = 1;
        else
        if(do_channel == 11)
                if(val == '0')
                        PORTD.5 = 0;
                else
                        PORTD.5 = 1;
        else
        if(do_channel == 12)
                if(val == '0')
                        PORTD.6 = 0;
                else
                        PORTD.6 = 1;
        else
        if(do_channel == 13)
                if(val == '0')
                        PORTD.7 = 0;
                else
                        PORTD.7 = 1;
        else
        if(do_channel == 14)
                if(val == '0')
                        PORTC.0 = 0;
                else
                        PORTC.0 = 1;
        else
        if(do_channel == 15)
                if(val == '0')
                        PORTC.1 = 0;
                else
                        PORTC.1 = 1;
        else
        {
                //printf("03\n\r");
                return;
        }

        //printf("OK\n\r");
}

int read_do(int do_channel)
{
        if(do_channel == 0)
                return PORTB.0;

        if(do_channel == 1)
                return PORTB.1;

        if(do_channel == 2)
                return PORTB.2;
            
        if(do_channel == 3)
                return PORTB.3;
            
        if(do_channel == 4)
                return PORTB.4;
                
        if(do_channel == 5)
                return PORTB.5;
            
        if(do_channel == 6)
                return PORTB.6;

        if(do_channel == 7)
                return PORTB.7;

        if(do_channel == 8)
                return PORTD.2;

        if(do_channel == 9)
                return PORTD.3;

        if(do_channel == 10)
                return PORTD.4;

        if(do_channel == 11)
                return PORTD.5;

        if(do_channel == 12)
                return PORTD.6;

        if(do_channel == 13)
                return PORTD.7;

        if(do_channel == 14)
                return PORTC.0;

        if(do_channel == 15)
                return PORTC.1;
}


void get_current()
{            
        raw_current = adc_data[0];
        actual_current = ((long)FACTOR_AMPERS*(long)raw_current)/(long)1023;
}

void set_buzz(unsigned int channel)
{
int i;
        if(buzz_top < MAXBUZZ -1)
        {
                for(i = 0; i <= buzz_top; i++)
                    if(buzzers[i] == channel)
                        return;
                buzzers[++buzz_top] = channel;
        }
}

void unset_buzz(unsigned int channel)
{
int i,j,bandera;

bandera = 0;
        for(i = 0; i <= buzz_top; i++)
            if(buzzers[i] == channel)
            {
                    bandera = 1;
                    break;
            }
        if(bandera == 1)
        {
            for(j = i;j  < buzz_top; j++)
                buzzers[j] = buzzers[j+1];
            buzz_top--;
            write_do(channel,'0');
        }
}

void buzz()
{
int i;
        tick++;
        if(tick == (MAXTICK))
        {
                i = 0;
                for(i = 0; i <= buzz_top; i++)
                {
                        //printf("Valor i: %d, buzzer %d: %d\n\r",i,buzzers[i],read_do(buzzers[i]));
                        if(read_do(buzzers[i]) == 1)
                                write_do(buzzers[i],'0');
                        else
                                write_do(buzzers[i],'1');
                }
                tick = 0;
        }
}


void read_data()
{
    printf("02\n\r%+010d\n\r%+010d\n\r",raw_current,actual_current);
    printf("Corriente un hiloI: %d\n\r",((long)200*(long)raw_current)/(long)1023);
    printf("Corriente un hiloF: %f\n\r",(200*raw_current)/1023);
    printf("%d%d%d%d%d%d0000000\n\r",~PINC.2,~PINC.3,~PINC.4,~PINC.5,~PINC.6,~PINC.7);
    printf("%d%d%d%d%d%d%d%d%d%d%d%d\n\r",PORTB.0,PORTB.1,PORTB.2,PORTB.3,PORTB.4,PORTB.5,PORTB.6,PORTB.7,PORTD.2,PORTD.3,PORTD.4,PORTD.5);
}


void main(void)
{
// Declare your local variables here
char letra;

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
// Func7=In Func6=In Func5=In Func4=In Func3=In Func2=In Func1=Out Func0=Out 
// State7=P State6=P State5=P State4=P State3=P State2=P State1=0 State0=0 
PORTC=0xFC;
DDRC=0x03;

// Port D initialization
// Func7=Out Func6=Out Func5=Out Func4=Out Func3=Out Func2=Out Func1=In Func0=In 
// State7=0 State6=0 State5=0 State4=0 State3=0 State2=0 State1=T State0=T 
PORTD=0x00;
DDRD=0xFC;

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

// ADC initialization
// ADC Clock frequency: 1000.000 kHz
// ADC Voltage Reference: AREF pin
ADMUX=FIRST_ADC_INPUT | (ADC_VREF_TYPE & 0xff);
ADCSRA=0xCB;

// Global enable interrupts
#asm("sei")

inicializar();
letra = '#';

while (1)
      {
      // Place your code here
      set_current_index();
      buzz();
      get_current();
      
      if(rx_counter > 0)
        letra = getchar();
        
      if(letra == 'r')
      {
        read_data();
        letra = '#';
      }
      
if(actual_current < low_normal_limit)
        write_do(0,'1');
        //set_buzz(0);
else    
        write_do(0,'0');
        //unset_buzz(0);
//printf("Low normal: %u\n\r",low_normal_limit);
        
if(actual_current >= low_normal_limit && actual_current < normal_limit)
        write_do(1,'1');
else
        write_do(1,'0');
        
if(actual_current >= normal_limit && actual_current < high_normal_limit)
        write_do(2,'1');
else
        write_do(2,'0');
//printf("Normal: %u\n\r",normal_limit);
        
if(actual_current > high_normal_limit && actual_current < high_limit)
        write_do(3,'1');
else
        write_do(3,'0');
//printf("High normal: %u\n\r",high_normal_limit);
        
if(actual_current >= high_limit && actual_current <very_high_limit)
        write_do(4,'1');
else
        write_do(4,'0');
//printf("High: %u\n\r",high_limit);
        
if(actual_current > very_high_limit)
{
        set_buzz(5);
        //write_do(5,'1');
}
else
        unset_buzz(5);
        //write_do(5,'0');
//printf("Corriente en bruto: %d Corriente actual: %d\n\r",raw_current,actual_current);
//printf("Corriente actual: %d\n\r",actual_current);

      
      /*apagar todo de los dos
2) if (actual_current < low_limit) low.send_do(1) loop();
3) if (actual_current < low_normal_limit) low_normal.send_do(1) loop()
4) if (actual_current < normal_limit) normal.send_do(1) loop()
5) if (actual_current < high_normal_limit) high_normal.send_do(1) loop()
6) if (actual_current < high_limit) high.send_do(1) loop()*/

      };
}
