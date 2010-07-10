#include "mod_mega2561_plex_imp.h"     

unsigned char  check_range_di(int start, int end) { return 0; }
unsigned char  check_range_do(int start, int end) { return 0; }
unsigned char  check_range_ai(int start, int end) { return 1; }
unsigned char  check_range_reg_in(int start, int end) { return 1; }
unsigned char  check_range_reg_out(int start, int end) { return 1; }

unsigned char  read_di(int n) { return 0; }
unsigned char  read_do(int n) { return 0; }
void send_do(int n, unsigned char  val) 
{  
  //printf("value: %c, addr: %d\n", (val != 0)?'T':'F', n); 
}
//void send_do(int n, unsigned char val);

uint16_t read_ai(int n)
{ 
        if(n >= 0 && n <= 15)
        {
                return cantidad_sensores[n]; 
        }                                   
        else
        if(n >= 16 && n <= 31)
        {
                return cantidad_sensores_detectados[n-16];
        } 
        else
        if(n >= 100 && n <= (100 + cantidad_sensores[0]))
        {                                              
                return temperatures[0][n-100];
        }
        else
        if(n >= 200 && n <= (200 + cantidad_sensores[1]))
        {                                              
                return temperatures[1][n-200];
        }
        else
        if(n >= 300 && n <= (300 + cantidad_sensores[2]))
        {                                              
                return temperatures[2][n-300];
        }
        else
        if(n >= 400 && n <= (400 + cantidad_sensores[3]))
        {                                              
                return temperatures[3][n-400];
        }   
        else
        if(n >= 500 && n <= (500 + cantidad_sensores[4]))
        {                                              
                return temperatures[4][n-500];
        }   
        else
        if(n >= 600 && n <= (600 + cantidad_sensores[5]))
        {                                              
                return temperatures[5][n-600];
        }   
        else
        if(n >= 700 && n <= (700 + cantidad_sensores[6]))
        {                                              
                return temperatures[6][n-700];
        }   
        else
        if(n >= 800 && n <= (800 + cantidad_sensores[7]))
        {                                              
                return temperatures[7][n-800];
        }   
        else
        if(n >= 900 && n <= (900 + cantidad_sensores[8]))
        {                                              
                return temperatures[8][n-900];
        }   
        else
        if(n >= 1000 && n <= (1000 + cantidad_sensores[9]))
        {                                              
                return temperatures[9][n-1000];
        }   
        else                         
        if(n >= 1100 && n <= (1100 + cantidad_sensores[10]))
        {                                              
                return temperatures[10][n-1100];
        }   
        else
        if(n >= 1200 && n <= (1200 + cantidad_sensores[11]))
        {                                              
                return temperatures[11][n-1200];
        }   
        else
        if(n >= 1300 && n <= (1300 + cantidad_sensores[12]))
        {                                              
                return temperatures[12][n-1300];
        }   
        else
        if(n >= 1400 && n <= (1400 + cantidad_sensores[13]))
        {                                              
                return temperatures[13][n-1400];
        }   
        else
        if(n >= 1500 && n <= (1500 + cantidad_sensores[14]))
        {                                              
                return temperatures[14][n-1500];
        }   
        else
        if(n >= 1600 && n <= (1600 + cantidad_sensores[15]))
        {                                              
                return temperatures[15][n-1600];
        }
        
        
        return 9999;
}
uint16_t read_register(int n) { return read_ai(n); }
void write_register(int n, uint16_t val) 
{ 
        if(n >= 0 && n <=15 && val >=0 && val <= 65535)
        {
                cantidad_sensores[n] = val;
        }
  //printf("*************** Write Register value: %hx, addr: %d\n", val, n); 
}