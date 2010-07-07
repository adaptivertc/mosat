#include <stdint.h> 

//#ifndef _MODIMP_INCLUDED_
//#define _MODIMP_INCLUDED_

#define TIME_OUT 30000  
#define RETARDO 100
#define MUX_ENABLE PORTA.6
#define MUX_A0 PORTA.5
#define MUX_A1 PORTA.4
#define MUX_A2 PORTA.3
#define MUX_A3 PORTA.1
#define COL_SIZE 16
#define COL_NUM 16



unsigned char read_di(int n);
unsigned char read_do(int n);
void send_do(int n, unsigned char val);
uint16_t read_ai(int n);
uint16_t read_register(int n);
void write_register(int n, uint16_t val);

unsigned char check_range_di(int start, int end);
unsigned char check_range_do(int start, int end);
unsigned char check_range_ai(int start, int end);
unsigned char check_range_reg_in(int start, int end);
unsigned char check_range_reg_out(int start, int end);

//#endif