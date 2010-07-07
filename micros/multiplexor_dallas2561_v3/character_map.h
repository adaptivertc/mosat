#define CLK PORTD.5
#define LATCH PORTD.6
#define OUTPUT_EN PORTD.7
#define DATA PORTD.4
 


void clock()
{
        CLK = 1;
        delay_us(10);
        CLK = 0;
} 

void reset()
{
        OUTPUT_EN = 0;  //output enable
        LATCH = 0;  //strobe
        CLK = 0;  //clk
        DATA = 0;  //data
}

void led_char(char data)
{
        if(data == 'A' || data == 'a')
        {
                LATCH = 0;
                DATA = 1;       //G
                clock();
                DATA = 0;       //T
                clock();
                DATA = 0;       //S
                clock();
                DATA = 1;       //U
                clock();
                DATA = 1;       //H
                clock();
                DATA = 0;       //K
                clock();
                DATA = 0;       //M
                clock();
                DATA = 1;       //A
                clock();
                DATA = 1;        //B
                clock();
                DATA = 0;       //N
                clock();
                DATA = 1;       //C
                clock();
                DATA = 1;       //P
                clock();
                DATA = 0;       //R
                clock();
                DATA = 1;       //D
                clock();
                DATA = 0;       //DP
                clock();
                DATA = 0;       //F y E
                clock();
                LATCH = 1;
        }
        else
        if(data == 'B' || data == 'b')
        {
                LATCH = 0;
                DATA = 0;       //G
                clock();
                DATA = 0;       //T
                clock();
                DATA = 1;       //S
                clock();
                DATA = 0;       //U
                clock();
                DATA = 0;       //H
                clock();
                DATA = 0;       //K
                clock();
                DATA = 1;       //M
                clock();
                DATA = 1;       //A
                clock();
                DATA = 1;        //B
                clock();
                DATA = 0;       //N
                clock();
                DATA = 1;       //C
                clock();
                DATA = 1;       //P
                clock();
                DATA = 0;       //R
                clock();
                DATA = 1;       //D
                clock();
                DATA = 0;       //DP
                clock();
                DATA = 1;       //F y E
                clock();
                LATCH = 1;
        }
        else
        if(data == 'C' || data == 'c')
        {
                LATCH = 0;
                DATA = 1;       //G
                clock();
                DATA = 0;       //T
                clock();
                DATA = 0;       //S
                clock();
                DATA = 0;       //U
                clock();
                DATA = 1;       //H
                clock();
                DATA = 0;       //K
                clock();
                DATA = 0;       //M
                clock();
                DATA = 1;       //A
                clock();
                DATA = 1;        //B
                clock();
                DATA = 0;       //N
                clock();
                DATA = 0;       //C
                clock();
                DATA = 0;       //P
                clock();
                DATA = 0;       //R
                clock();
                DATA = 0;       //D
                clock();
                DATA = 0;       //DP
                clock();
                DATA = 1;       //F y E
                clock();
                LATCH = 1;
        }
        else
        if(data == 'D' || data == 'd')
        {
                LATCH = 0;
                DATA = 0;       //G
                clock();
                DATA = 0;       //T
                clock();
                DATA = 1;       //S
                clock();
                DATA = 0;       //U
                clock();
                DATA = 0;       //H
                clock();
                DATA = 0;       //K
                clock();
                DATA = 1;       //M
                clock();
                DATA = 1;       //A
                clock();
                DATA = 1;       //B
                clock();
                DATA = 0;       //N
                clock();
                DATA = 1;       //C
                clock();
                DATA = 0;       //P
                clock();
                DATA = 0;       //R
                clock();
                DATA = 1;       //D
                clock();
                DATA = 0;       //DP
                clock();
                DATA = 1;       //F y E
                clock();
                LATCH = 1;
        }
	  else
	  if(data == 'E' || data == 'e')
        {
                LATCH = 0;
                DATA = 1;       //G
                clock();
                DATA = 0;       //T
                clock();
                DATA = 0;       //S
                clock();
                DATA = 1;       //U
                clock();
                DATA = 1;       //H
                clock();
                DATA = 0;       //K
                clock();
                DATA = 0;       //M
                clock();
                DATA = 1;       //A
                clock();
                DATA = 1;        //B
                clock();
                DATA = 0;       //N
                clock();
                DATA = 0;       //C
                clock();
                DATA = 1;       //P
                clock();
                DATA = 0;       //R
                clock();
                DATA = 0;       //D
                clock();
                DATA = 0;       //DP
                clock();
                DATA = 1;       //F y E
                clock();
                LATCH = 1;
        }
	  else
	  if(data == 'F' || data == 'f')
        {
                LATCH = 0;
                DATA = 1;       //G
                clock();
                DATA = 0;       //T
                clock();
                DATA = 0;       //S
                clock();
                DATA = 1;       //U
                clock();
                DATA = 1;       //H
                clock();
                DATA = 0;       //K
                clock();
                DATA = 0;       //M
                clock();
                DATA = 1;       //A
                clock();
                DATA = 1;        //B
                clock();
                DATA = 0;       //N
                clock();
                DATA = 0;       //C
                clock();
                DATA = 1;       //P
                clock();
                DATA = 0;       //R
                clock();
                DATA = 0;       //D
                clock();
                DATA = 0;       //DP
                clock();
                DATA = 0;       //F y E
                clock();
                LATCH = 1;
        }
	  else
	  if(data == 'G' || data == 'g')
        {
                LATCH = 0;
                DATA = 1;       //G
                clock();
                DATA = 0;       //T
                clock();
                DATA = 0;       //S
                clock();
                DATA = 0;       //U
                clock();
                DATA = 1;       //H
                clock();
                DATA = 0;       //K
                clock();
                DATA = 0;       //M
                clock();
                DATA = 1;       //A
                clock();
                DATA = 1;        //B
                clock();
                DATA = 0;       //N
                clock();
                DATA = 0;       //C
                clock();
                DATA = 1;       //P
                clock();
                DATA = 0;       //R
                clock();
                DATA = 1;       //D
                clock();
                DATA = 0;       //DP
                clock();
                DATA = 1;       //F y E
                clock();
                LATCH = 1;
        }
	  else
	  if(data == 'H' || data == 'h')
        {
                LATCH = 0;
                DATA = 1;       //G
                clock();
                DATA = 0;       //T
                clock();
                DATA = 0;       //S
                clock();
                DATA = 1;       //U
                clock();
                DATA = 1;       //H
                clock();
                DATA = 0;       //K
                clock();
                DATA = 0;       //M
                clock();
                DATA = 0;       //A
                clock();
                DATA = 0;        //B
                clock();
                DATA = 0;       //N
                clock();
                DATA = 1;       //C
                clock();
                DATA = 1;       //P
                clock();
                DATA = 0;       //R
                clock();
                DATA = 1;       //D
                clock();
                DATA = 0;       //DP
                clock();
                DATA = 0;       //F y E
                clock();
                LATCH = 1;
        }
	  else
	  if(data == 'I' || data == 'i')
        {
                LATCH = 0;
                DATA = 0;       //G
                clock();
                DATA = 0;       //T
                clock();
                DATA = 1;       //S
                clock();
                DATA = 0;       //U
                clock();
                DATA = 0;       //H
                clock();
                DATA = 0;       //K
                clock();
                DATA = 1;       //M
                clock();
                DATA = 1;       //A
                clock();
                DATA = 1;        //B
                clock();
                DATA = 0;       //N
                clock();
                DATA = 0;       //C
                clock();
                DATA = 0;       //P
                clock();
                DATA = 0;       //R
                clock();
                DATA = 0;       //D
                clock();
                DATA = 0;       //DP
                clock();
                DATA = 1;       //F y E
                clock();
                LATCH = 1;
        }
	  else
	  if(data == 'J' || data == 'j')
        {
                LATCH = 0;
                DATA = 1;       //G
                clock();
                DATA = 0;       //T
                clock();
                DATA = 0;       //S
                clock();
                DATA = 0;       //U
                clock();
                DATA = 0;       //H
                clock();
                DATA = 0;       //K
                clock();
                DATA = 0;       //M
                clock();
                DATA = 0;       //A
                clock();
                DATA = 0;        //B
                clock();
                DATA = 0;       //N
                clock();
                DATA = 1;       //C
                clock();
                DATA = 0;       //P
                clock();
                DATA = 0;       //R
                clock();
                DATA = 1;       //D
                clock();
                DATA = 0;       //DP
                clock();
                DATA = 1;       //F y E
                clock();
                LATCH = 1;
        }
        else
        if(data == 'K' || data == 'k')
        {
                LATCH = 0;
                DATA = 1;       //G
                clock();
                DATA = 0;       //T
                clock();
                DATA = 0;       //S
                clock();
                DATA = 1;       //U
                clock();
                DATA = 1;       //H
                clock();
                DATA = 0;       //K
                clock();
                DATA = 0;       //M
                clock();
                DATA = 0;       //A
                clock();
                DATA = 0;        //B
                clock();
                DATA = 1;       //N
                clock();
                DATA = 0;       //C
                clock();
                DATA = 0;       //P
                clock();
                DATA = 1;       //R
                clock();
                DATA = 0;       //D
                clock();
                DATA = 0;       //DP
                clock();
                DATA = 0;       //F y E
                clock();
                LATCH = 1;
        }
        else
        if(data == 'L' || data == 'l')
        {
                LATCH = 0;
                DATA = 1;       //G
                clock();
                DATA = 0;       //T
                clock();
                DATA = 0;       //S
                clock();
                DATA = 0;       //U
                clock();
                DATA = 1;       //H
                clock();
                DATA = 0;       //K
                clock();
                DATA = 0;       //M
                clock();
                DATA = 0;       //A
                clock();
                DATA = 0;        //B
                clock();
                DATA = 0;       //N
                clock();
                DATA = 0;       //C
                clock();
                DATA = 0;       //P
                clock();
                DATA = 0;       //R
                clock();
                DATA = 0;       //D
                clock();
                DATA = 0;       //DP
                clock();
                DATA = 1;       //F y E
                clock();
                LATCH = 1;
        }
        else
        if(data == 'M' || data == 'm')
        {
                LATCH = 0;
                DATA = 1;       //G
                clock();
                DATA = 0;       //T
                clock();
                DATA = 0;       //S
                clock();
                DATA = 0;       //U
                clock();
                DATA = 1;       //H
                clock();
                DATA = 1;       //K
                clock();
                DATA = 0;       //M
                clock();
                DATA = 0;       //A
                clock();
                DATA = 0;        //B
                clock();
                DATA = 1;       //N
                clock();
                DATA = 1;       //C
                clock();
                DATA = 0;       //P
                clock();
                DATA = 0;       //R
                clock();
                DATA = 1;       //D
                clock();
                DATA = 0;       //DP
                clock();
                DATA = 0;       //F y E
                clock();
                LATCH = 1;
        }
        else
        if(data == 'N' || data == 'n')
        {
                LATCH = 0;
                DATA = 1;       //G
                clock();
                DATA = 0;       //T
                clock();
                DATA = 0;       //S
                clock();
                DATA = 0;       //U
                clock();
                DATA = 1;       //H
                clock();
                DATA = 1;       //K
                clock();
                DATA = 0;       //M
                clock();
                DATA = 0;       //A
                clock();
                DATA = 0;        //B
                clock();
                DATA = 0;       //N
                clock();
                DATA = 1;       //C
                clock();
                DATA = 0;       //P
                clock();
                DATA = 1;       //R
                clock();
                DATA = 1;       //D
                clock();
                DATA = 0;       //DP
                clock();
                DATA = 0;       //F y E
                clock();
                LATCH = 1;
        }
        else
        if(data == 'O' || data == 'o')
        {
                LATCH = 0;
                DATA = 1;       //G
                clock();
                DATA = 0;       //T
                clock();
                DATA = 0;       //S
                clock();
                DATA = 0;       //U
                clock();
                DATA = 1;       //H
                clock();
                DATA = 0;       //K
                clock();
                DATA = 0;       //M
                clock();
                DATA = 1;       //A
                clock();
                DATA = 1;        //B
                clock();
                DATA = 0;       //N
                clock();
                DATA = 1;       //C
                clock();
                DATA = 0;       //P
                clock();
                DATA = 0;       //R
                clock();
                DATA = 1;       //D
                clock();
                DATA = 0;       //DP
                clock();
                DATA = 1;       //F y E
                clock();
                LATCH = 1;
        }
        else
        if(data == 'P' || data == 'p')
        {
                LATCH = 0;
                DATA = 1;       //G
                clock();
                DATA = 0;       //T
                clock();
                DATA = 0;       //S
                clock();
                DATA = 1;       //U
                clock();
                DATA = 1;       //H
                clock();
                DATA = 0;       //K
                clock();
                DATA = 0;       //M
                clock();
                DATA = 1;       //A
                clock();
                DATA = 1;        //B
                clock();
                DATA = 0;       //N
                clock();
                DATA = 1;       //C
                clock();
                DATA = 1;       //P
                clock();
                DATA = 0;       //R
                clock();
                DATA = 0;       //D
                clock();
                DATA = 0;       //DP
                clock();
                DATA = 0;       //F y E
                clock();
                LATCH = 1;
        }
        else
        if(data == 'Q' || data == 'q')
        {
                LATCH = 0;
                DATA = 1;       //G
                clock();
                DATA = 0;       //T
                clock();
                DATA = 0;       //S
                clock();
                DATA = 0;       //U
                clock();
                DATA = 1;       //H
                clock();
                DATA = 0;       //K
                clock();
                DATA = 0;       //M
                clock();
                DATA = 1;       //A
                clock();
                DATA = 1;        //B
                clock();
                DATA = 0;       //N
                clock();
                DATA = 1;       //C
                clock();
                DATA = 0;       //P
                clock();
                DATA = 1;       //R
                clock();
                DATA = 1;       //D
                clock();
                DATA = 0;       //DP
                clock();
                DATA = 1;       //F y E
                clock();
                LATCH = 1;
        }
        else
        if(data == 'R' || data == 'r')
        {
                LATCH = 0;
                DATA = 1;       //G
                clock();
                DATA = 0;       //T
                clock();
                DATA = 0;       //S
                clock();
                DATA = 1;       //U
                clock();
                DATA = 1;       //H
                clock();
                DATA = 0;       //K
                clock();
                DATA = 0;       //M
                clock();
                DATA = 1;       //A
                clock();
                DATA = 1;        //B
                clock();
                DATA = 0;       //N
                clock();
                DATA = 1;       //C
                clock();
                DATA = 1;       //P
                clock();
                DATA = 1;       //R
                clock();
                DATA = 0;       //D
                clock();
                DATA = 0;       //DP
                clock();
                DATA = 0;       //F y E
                clock();
                LATCH = 1;
        }
        else
        if(data == 'S' || data == 's')
        {
                LATCH = 0;
                DATA = 0;       //G
                clock();
                DATA = 0;       //T
                clock();
                DATA = 0;       //S
                clock();
                DATA = 1;       //U
                clock();
                DATA = 1;       //H
                clock();
                DATA = 0;       //K
                clock();
                DATA = 0;       //M
                clock();
                DATA = 1;       //A
                clock();
                DATA = 1;        //B
                clock();
                DATA = 0;       //N
                clock();
                DATA = 0;       //C
                clock();
                DATA = 1;       //P
                clock();
                DATA = 0;       //R
                clock();
                DATA = 1;       //D
                clock();
                DATA = 0;       //DP
                clock();
                DATA = 1;       //F y E
                clock();
                LATCH = 1;
        }
        else
        if(data == 'T' || data == 't')
        {
                LATCH = 0;
                DATA = 0;       //G
                clock();
                DATA = 0;       //T
                clock();
                DATA = 1;       //S
                clock();
                DATA = 0;       //U
                clock();
                DATA = 0;       //H
                clock();
                DATA = 0;       //K
                clock();
                DATA = 1;       //M
                clock();
                DATA = 1;       //A
                clock();
                DATA = 1;        //B
                clock();
                DATA = 0;       //N
                clock();
                DATA = 0;       //C
                clock();
                DATA = 0;       //P
                clock();
                DATA = 0;       //R
                clock();
                DATA = 0;       //D
                clock();
                DATA = 0;       //DP
                clock();
                DATA = 0;       //F y E
                clock();
                LATCH = 1;
        }
        else
        if(data == 'U' || data == 'u')
        {
                LATCH = 0;
                DATA = 1;       //G
                clock();
                DATA = 0;       //T
                clock();
                DATA = 0;       //S
                clock();
                DATA = 0;       //U
                clock();
                DATA = 1;       //H
                clock();
                DATA = 0;       //K
                clock();
                DATA = 0;       //M
                clock();
                DATA = 0;       //A
                clock();
                DATA = 0;        //B
                clock();
                DATA = 0;       //N
                clock();
                DATA = 1;       //C
                clock();
                DATA = 0;       //P
                clock();
                DATA = 0;       //R
                clock();
                DATA = 1;       //D
                clock();
                DATA = 0;       //DP
                clock();
                DATA = 1;       //F y E
                clock();
                LATCH = 1;
        }               
        else
        if(data == 'V' || data == 'v')
        {
                LATCH = 0;
                DATA = 1;       //G
                clock();
                DATA = 1;       //T
                clock();
                DATA = 0;       //S
                clock();
                DATA = 0;       //U
                clock();
                DATA = 1;       //H
                clock();
                DATA = 0;       //K
                clock();
                DATA = 0;       //M
                clock();
                DATA = 0;       //A
                clock();
                DATA = 0;        //B
                clock();
                DATA = 1;       //N
                clock();
                DATA = 0;       //C
                clock();
                DATA = 0;       //P
                clock();
                DATA = 0;       //R
                clock();
                DATA = 0;       //D
                clock();
                DATA = 0;       //DP
                clock();
                DATA = 0;       //F y E
                clock();
                LATCH = 1;
        }
        else
        if(data == 'W' || data == 'w')
        {
                LATCH = 0;
                DATA = 1;       //G
                clock();
                DATA = 1;       //T
                clock();
                DATA = 0;       //S
                clock();
                DATA = 0;       //U
                clock();
                DATA = 1;       //H
                clock();
                DATA = 0;       //K
                clock();
                DATA = 0;       //M
                clock();
                DATA = 0;       //A
                clock();
                DATA = 0;        //B
                clock();
                DATA = 0;       //N
                clock();
                DATA = 1;       //C
                clock();
                DATA = 0;       //P
                clock();
                DATA = 1;       //R
                clock();
                DATA = 1;       //D
                clock();
                DATA = 0;       //DP
                clock();
                DATA = 0;       //F y E
                clock();
                LATCH = 1;
        }
        else
        if(data == 'X' || data == 'x')
        {
                LATCH = 0;
                DATA = 0;       //G
                clock();
                DATA = 1;       //T
                clock();
                DATA = 0;       //S
                clock();
                DATA = 0;       //U
                clock();
                DATA = 0;       //H
                clock();
                DATA = 1;       //K
                clock();
                DATA = 0;       //M
                clock();
                DATA = 0;       //A
                clock();
                DATA = 0;        //B
                clock();
                DATA = 1;       //N
                clock();
                DATA = 0;       //C
                clock();
                DATA = 0;       //P
                clock();
                DATA = 1;       //R
                clock();
                DATA = 0;       //D
                clock();
                DATA = 0;       //DP
                clock();
                DATA = 0;       //F y E
                clock();
                LATCH = 1;
        }
        else
        if(data == 'Y' || data == 'y')
        {
                LATCH = 0;
                DATA = 0;       //G
                clock();
                DATA = 0;       //T
                clock();
                DATA = 1;       //S
                clock();
                DATA = 0;       //U
                clock();
                DATA = 0;       //H
                clock();
                DATA = 1;       //K
                clock();
                DATA = 0;       //M
                clock();
                DATA = 0;       //A
                clock();
                DATA = 0;        //B
                clock();
                DATA = 1;       //N
                clock();
                DATA = 0;       //C
                clock();
                DATA = 0;       //P
                clock();
                DATA = 0;       //R
                clock();
                DATA = 0;       //D
                clock();
                DATA = 0;       //DP
                clock();
                DATA = 0;       //F y E
                clock();
                LATCH = 1;
        }
        else
        if(data == 'Z' || data == 'z')
        {
                LATCH = 0;
                DATA = 0;       //G
                clock();
                DATA = 1;       //T
                clock();
                DATA = 0;       //S
                clock();
                DATA = 0;       //U
                clock();
                DATA = 0;       //H
                clock();
                DATA = 0;       //K
                clock();
                DATA = 0;       //M
                clock();
                DATA = 1;       //A
                clock();
                DATA = 1;        //B
                clock();
                DATA = 1;       //N
                clock();
                DATA = 0;       //C
                clock();
                DATA = 0;       //P
                clock();
                DATA = 0;       //R
                clock();
                DATA = 0;       //D
                clock();
                DATA = 0;       //DP
                clock();
                DATA = 1;       //F y E
                clock();
                LATCH = 1;
        }
        else
        if(data == '1')
        {
                LATCH = 0;
                DATA = 0;       //G
                clock();
                DATA = 0;       //T
                clock();
                DATA = 1;       //S
                clock();
                DATA = 0;       //U
                clock();
                DATA = 0;       //H
                clock();
                DATA = 0;       //K
                clock();
                DATA = 1;       //M
                clock();
                DATA = 0;       //A
                clock();
                DATA = 0;        //B
                clock();
                DATA = 0;       //N
                clock();
                DATA = 0;       //C
                clock();
                DATA = 0;       //P
                clock();
                DATA = 0;       //R
                clock();
                DATA = 0;       //D
                clock();
                DATA = 0;       //DP
                clock();
                DATA = 0;       //F y E
                clock();
                LATCH = 1;
        }
        else
        if(data == '2')
        {
                LATCH = 0;
                DATA = 1;       //G
                clock();
                DATA = 0;       //T
                clock();
                DATA = 0;       //S
                clock();
                DATA = 1;       //U
                clock();
                DATA = 0;       //H
                clock();
                DATA = 0;       //K
                clock();
                DATA = 0;       //M
                clock();
                DATA = 1;       //A
                clock();
                DATA = 1;        //B
                clock();
                DATA = 0;       //N
                clock();
                DATA = 1;       //C
                clock();
                DATA = 1;       //P
                clock();
                DATA = 0;       //R
                clock();
                DATA = 0;       //D
                clock();
                DATA = 0;       //DP
                clock();
                DATA = 1;       //F y E
                clock();
                LATCH = 1;
        }
        else
        if(data == '3')
        {
                LATCH = 0;
                DATA = 0;       //G
                clock();
                DATA = 0;       //T
                clock();
                DATA = 0;       //S
                clock();
                DATA = 1;       //U
                clock();
                DATA = 0;       //H
                clock();
                DATA = 0;       //K
                clock();
                DATA = 0;       //M
                clock();
                DATA = 1;       //A
                clock();
                DATA = 1;        //B
                clock();
                DATA = 0;       //N
                clock();
                DATA = 1;       //C
                clock();
                DATA = 1;       //P
                clock();
                DATA = 0;       //R
                clock();
                DATA = 1;       //D
                clock();
                DATA = 0;       //DP
                clock();
                DATA = 1;       //F y E
                clock();
                LATCH = 1;
        }
        else
        if(data == '4')
        {
                LATCH = 0;
                DATA = 0;       //G
                clock();
                DATA = 0;       //T
                clock();
                DATA = 0;       //S
                clock();
                DATA = 1;       //U
                clock();
                DATA = 1;       //H
                clock();
                DATA = 0;       //K
                clock();
                DATA = 0;       //M
                clock();
                DATA = 0;       //A
                clock();
                DATA = 0;        //B
                clock();
                DATA = 0;       //N
                clock();
                DATA = 1;       //C
                clock();
                DATA = 1;       //P
                clock();
                DATA = 0;       //R
                clock();
                DATA = 1;       //D
                clock();
                DATA = 0;       //DP
                clock();
                DATA = 0;       //F y E
                clock();
                LATCH = 1;
        }
        else
        if(data == '5')
        {
                LATCH = 0;
                DATA = 0;       //G
                clock();
                DATA = 0;       //T
                clock();
                DATA = 0;       //S
                clock();
                DATA = 1;       //U
                clock();
                DATA = 1;       //H
                clock();
                DATA = 0;       //K
                clock();
                DATA = 0;       //M
                clock();
                DATA = 1;       //A
                clock();
                DATA = 1;        //B
                clock();
                DATA = 0;       //N
                clock();
                DATA = 0;       //C
                clock();
                DATA = 0;       //P
                clock();
                DATA = 1;       //R
                clock();
                DATA = 0;       //D
                clock();
                DATA = 0;       //DP
                clock();
                DATA = 1;       //F y E
                clock();
                LATCH = 1;
        }
        else
        if(data == '6')
        {
                LATCH = 0;
                DATA = 1;       //G
                clock();
                DATA = 0;       //T
                clock();
                DATA = 0;       //S
                clock();
                DATA = 1;       //U
                clock();
                DATA = 1;       //H
                clock();
                DATA = 0;       //K
                clock();
                DATA = 0;       //M
                clock();
                DATA = 1;       //A
                clock();
                DATA = 1;        //B
                clock();
                DATA = 0;       //N
                clock();
                DATA = 0;       //C
                clock();
                DATA = 1;       //P
                clock();
                DATA = 0;       //R
                clock();
                DATA = 1;       //D
                clock();
                DATA = 0;       //DP
                clock();
                DATA = 1;       //F y E
                clock();
                LATCH = 1;
        }
        else
        if(data == '7')
        {
                LATCH = 0;
                DATA = 0;       //G
                clock();
                DATA = 0;       //T
                clock();
                DATA = 0;       //S
                clock();
                DATA = 0;       //U
                clock();
                DATA = 0;       //H
                clock();
                DATA = 0;       //K
                clock();
                DATA = 0;       //M
                clock();
                DATA = 1;       //A
                clock();
                DATA = 1;        //B
                clock();
                DATA = 0;       //N
                clock();
                DATA = 1;       //C
                clock();
                DATA = 0;       //P
                clock();
                DATA = 0;       //R
                clock();
                DATA = 1;       //D
                clock();
                DATA = 0;       //DP
                clock();
                DATA = 0;       //F y E
                clock();
                LATCH = 1;
        }
        else
        if(data == '8')
        {
                LATCH = 0;
                DATA = 1;       //G
                clock();
                DATA = 0;       //T
                clock();
                DATA = 0;       //S
                clock();
                DATA = 1;       //U
                clock();
                DATA = 1;       //H
                clock();
                DATA = 0;       //K
                clock();
                DATA = 0;       //M
                clock();
                DATA = 1;       //A
                clock();
                DATA = 1;        //B
                clock();
                DATA = 0;       //N
                clock();
                DATA = 1;       //C
                clock();
                DATA = 1;       //P
                clock();
                DATA = 0;       //R
                clock();
                DATA = 1;       //D
                clock();
                DATA = 0;       //DP
                clock();
                DATA = 1;       //F y E
                clock();
                LATCH = 1;
        }
        else
        if(data == '9')
        {
                LATCH = 0;
                DATA = 0;       //G
                clock();
                DATA = 0;       //T
                clock();
                DATA = 0;       //S
                clock();
                DATA = 1;       //U
                clock();
                DATA = 1;       //H
                clock();
                DATA = 0;       //K
                clock();
                DATA = 0;       //M
                clock();
                DATA = 1;       //A
                clock();
                DATA = 1;        //B
                clock();
                DATA = 0;       //N
                clock();
                DATA = 1;       //C
                clock();
                DATA = 1;       //P
                clock();
                DATA = 0;       //R
                clock();
                DATA = 1;       //D
                clock();
                DATA = 0;       //DP
                clock();
                DATA = 1;       //F y E
                clock();
                LATCH = 1;
        }
        else
        if(data == '0')
        {
                LATCH = 0;
                DATA = 1;       //G
                clock();
                DATA = 1;       //T
                clock();
                DATA = 0;       //S
                clock();
                DATA = 0;       //U
                clock();
                DATA = 1;       //H
                clock();
                DATA = 0;       //K
                clock();
                DATA = 0;       //M
                clock();
                DATA = 1;       //A
                clock();
                DATA = 1;        //B
                clock();
                DATA = 1;       //N
                clock();
                DATA = 1;       //C
                clock();
                DATA = 0;       //P
                clock();
                DATA = 0;       //R
                clock();
                DATA = 1;       //D
                clock();
                DATA = 0;       //DP
                clock();
                DATA = 1;       //F y E
                clock();
                LATCH = 1;
        }
        else
        if(data == '@')
        {
                LATCH = 0;
                DATA = 1;       //G
                clock();
                DATA = 0;       //T
                clock();
                DATA = 0;       //S
                clock();
                DATA = 0;       //U
                clock();
                DATA = 1;       //H
                clock();
                DATA = 0;       //K
                clock();
                DATA = 1;       //M
                clock();
                DATA = 1;       //A
                clock();
                DATA = 1;        //B
                clock();
                DATA = 0;       //N
                clock();
                DATA = 1;       //C
                clock();
                DATA = 1;       //P
                clock();
                DATA = 0;       //R
                clock();
                DATA = 0;       //D
                clock();
                DATA = 0;       //DP
                clock();
                DATA = 1;       //F y E
                clock();
                LATCH = 1;
        }
        else
        if(data == '$')
        {
                LATCH = 0;
                DATA = 0;       //G
                clock();
                DATA = 0;       //T
                clock();
                DATA = 1;       //S
                clock();
                DATA = 1;       //U
                clock();
                DATA = 1;       //H
                clock();
                DATA = 0;       //K
                clock();
                DATA = 1;       //M
                clock();
                DATA = 1;       //A
                clock();
                DATA = 1;        //B
                clock();
                DATA = 0;       //N
                clock();
                DATA = 0;       //C
                clock();
                DATA = 1;       //P
                clock();
                DATA = 0;       //R
                clock();
                DATA = 1;       //D
                clock();
                DATA = 0;       //DP
                clock();
                DATA = 1;       //F y E
                clock();
                LATCH = 1;
        }
        else
        if(data == '°')
        {
                LATCH = 0;
                DATA = 0;       //G
                clock();
                DATA = 0;       //T
                clock();
                DATA = 0;       //S
                clock();
                DATA = 1;       //U
                clock();
                DATA = 1;       //H
                clock();
                DATA = 0;       //K
                clock();
                DATA = 0;       //M
                clock();
                DATA = 1;       //A
                clock();
                DATA = 1;        //B
                clock();
                DATA = 0;       //N
                clock();
                DATA = 1;       //C
                clock();
                DATA = 1;       //P
                clock();
                DATA = 0;       //R
                clock();
                DATA = 0;       //D
                clock();
                DATA = 0;       //DP
                clock();
                DATA = 0;       //F y E
                clock();
                LATCH = 1;
        }
        else
        if(data == '+')
        {
                LATCH = 0;
                DATA = 0;       //G
                clock();
                DATA = 0;       //T
                clock();
                DATA = 1;       //S
                clock();
                DATA = 1;       //U
                clock();
                DATA = 0;       //H
                clock();
                DATA = 0;       //K
                clock();
                DATA = 1;       //M
                clock();
                DATA = 0;       //A
                clock();
                DATA = 0;        //B
                clock();
                DATA = 0;       //N
                clock();
                DATA = 0;       //C
                clock();
                DATA = 1;       //P
                clock();
                DATA = 0;       //R
                clock();
                DATA = 0;       //D
                clock();
                DATA = 0;       //DP
                clock();
                DATA = 0;       //F y E
                clock();
                LATCH = 1;
        }
        else
        if(data == '-')
        {
                LATCH = 0;
                DATA = 0;       //G
                clock();
                DATA = 0;       //T
                clock();
                DATA = 0;       //S
                clock();
                DATA = 1;       //U
                clock();
                DATA = 0;       //H
                clock();
                DATA = 0;       //K
                clock();
                DATA = 0;       //M
                clock();
                DATA = 0;       //A
                clock();
                DATA = 0;        //B
                clock();
                DATA = 0;       //N
                clock();
                DATA = 0;       //C
                clock();
                DATA = 1;       //P
                clock();
                DATA = 0;       //R
                clock();
                DATA = 0;       //D
                clock();
                DATA = 0;       //DP
                clock();
                DATA = 0;       //F y E
                clock();
                LATCH = 1;
        }
        else
        if(data == '*')
        {
                LATCH = 0;
                DATA = 0;       //G
                clock();
                DATA = 1;       //T
                clock();
                DATA = 1;       //S
                clock();
                DATA = 1;       //U
                clock();
                DATA = 0;       //H
                clock();
                DATA = 1;       //K
                clock();
                DATA = 1;       //M
                clock();
                DATA = 0;       //A
                clock();
                DATA = 0;        //B
                clock();
                DATA = 1;       //N
                clock();
                DATA = 0;       //C
                clock();
                DATA = 1;       //P
                clock();
                DATA = 1;       //R
                clock();
                DATA = 0;       //D
                clock();
                DATA = 0;       //DP
                clock();
                DATA = 0;       //F y E
                clock();
                LATCH = 1;
        }
        else
        if(data == '/')
        {
                LATCH = 0;
                DATA = 0;       //G
                clock();
                DATA = 1;       //T
                clock();
                DATA = 0;       //S
                clock();
                DATA = 0;       //U
                clock();
                DATA = 0;       //H
                clock();
                DATA = 0;       //K
                clock();
                DATA = 0;       //M
                clock();
                DATA = 0;       //A
                clock();
                DATA = 0;        //B
                clock();
                DATA = 1;       //N
                clock();
                DATA = 0;       //C
                clock();
                DATA = 0;       //P
                clock();
                DATA = 0;       //R
                clock();
                DATA = 0;       //D
                clock();
                DATA = 0;       //DP
                clock();
                DATA = 0;       //F y E
                clock();
                LATCH = 1;
        }
        else
        if(data == '\\')
        {
                LATCH = 0;
                DATA = 0;       //G
                clock();
                DATA = 0;       //T
                clock();
                DATA = 0;       //S
                clock();
                DATA = 0;       //U
                clock();
                DATA = 0;       //H
                clock();
                DATA = 1;       //K
                clock();
                DATA = 0;       //M
                clock();
                DATA = 0;       //A
                clock();
                DATA = 0;        //B
                clock();
                DATA = 0;       //N
                clock();
                DATA = 0;       //C
                clock();
                DATA = 0;       //P
                clock();
                DATA = 1;       //R
                clock();
                DATA = 0;       //D
                clock();
                DATA = 0;       //DP
                clock();
                DATA = 0;       //F y E
                clock();
                LATCH = 1;
        }
        else
        if(data == '>')
        {
                LATCH = 0;
                DATA = 0;       //G
                clock();
                DATA = 1;       //T
                clock();
                DATA = 0;       //S
                clock();
                DATA = 0;       //U
                clock();
                DATA = 0;       //H
                clock();
                DATA = 1;       //K
                clock();
                DATA = 0;       //M
                clock();
                DATA = 0;       //A
                clock();
                DATA = 0;        //B
                clock();
                DATA = 0;       //N
                clock();
                DATA = 0;       //C
                clock();
                DATA = 0;       //P
                clock();
                DATA = 0;       //R
                clock();
                DATA = 0;       //D
                clock();
                DATA = 0;       //DP
                clock();
                DATA = 0;       //F y E
                clock();
                LATCH = 1;
        }
        else
        if(data == '<')
        {
                LATCH = 0;
                DATA = 0;       //G
                clock();
                DATA = 0;       //T
                clock();
                DATA = 0;       //S
                clock();
                DATA = 0;       //U
                clock();
                DATA = 0;       //H
                clock();
                DATA = 0;       //K
                clock();
                DATA = 0;       //M
                clock();
                DATA = 0;       //A
                clock();
                DATA = 0;        //B
                clock();
                DATA = 1;       //N
                clock();
                DATA = 0;       //C
                clock();
                DATA = 0;       //P
                clock();
                DATA = 1;       //R
                clock();
                DATA = 0;       //D
                clock();
                DATA = 0;       //DP
                clock();
                DATA = 0;       //F y E
                clock();
                LATCH = 1;
        }
        else
        if(data == '¿')
        {
                LATCH = 0;
                DATA = 1;       //G
                clock();
                DATA = 0;       //T
                clock();
                DATA = 0;       //S
                clock();
                DATA = 1;       //U
                clock();
                DATA = 0;       //H
                clock();
                DATA = 0;       //K
                clock();
                DATA = 1;       //M
                clock();
                DATA = 0;       //A
                clock();
                DATA = 0;        //B
                clock();
                DATA = 0;       //N
                clock();
                DATA = 0;       //C
                clock();
                DATA = 0;       //P
                clock();
                DATA = 0;       //R
                clock();
                DATA = 0;       //D
                clock();
                DATA = 0;       //DP
                clock();
                DATA = 1;       //F y E
                clock();
                LATCH = 1;
        }
        else
        if(data == '?')
        {
                LATCH = 0;
                DATA = 0;       //G
                clock();
                DATA = 0;       //T
                clock();
                DATA = 1;       //S
                clock();
                DATA = 0;       //U
                clock();
                DATA = 0;       //H
                clock();
                DATA = 0;       //K
                clock();
                DATA = 0;       //M
                clock();
                DATA = 1;       //A
                clock();
                DATA = 1;        //B
                clock();
                DATA = 0;       //N
                clock();
                DATA = 1;       //C
                clock();
                DATA = 1;       //P
                clock();
                DATA = 0;       //R
                clock();
                DATA = 0;       //D
                clock();
                DATA = 0;       //DP
                clock();
                DATA = 0;       //F y E
                clock();
                LATCH = 1;
        }
        else
        if(data == ')')
        {
                LATCH = 0;
                DATA = 0;       //G
                clock();
                DATA = 0;       //T
                clock();
                DATA = 0;       //S
                clock();
                DATA = 0;       //U
                clock();
                DATA = 0;       //H
                clock();
                DATA = 0;       //K
                clock();
                DATA = 0;       //M
                clock();
                DATA = 1;       //A
                clock();
                DATA = 1;        //B
                clock();
                DATA = 0;       //N
                clock();
                DATA = 1;       //C
                clock();
                DATA = 0;       //P
                clock();
                DATA = 0;       //R
                clock();
                DATA = 1;       //D
                clock();
                DATA = 0;       //DP
                clock();
                DATA = 1;       //F y E
                clock();
                LATCH = 1;
        }
        else
        if(data == '=')
        {
                LATCH = 0;
                DATA = 0;       //G
                clock();
                DATA = 0;       //T
                clock();
                DATA = 0;       //S
                clock();
                DATA = 1;       //U
                clock();
                DATA = 0;       //H
                clock();
                DATA = 0;       //K
                clock();
                DATA = 0;       //M
                clock();
                DATA = 1;       //A
                clock();
                DATA = 1;        //B
                clock();
                DATA = 0;       //N
                clock();
                DATA = 0;       //C
                clock();
                DATA = 1;       //P
                clock();
                DATA = 0;       //R
                clock();
                DATA = 0;       //D
                clock();
                DATA = 0;       //DP
                clock();
                DATA = 0;       //F y E
                clock();
                LATCH = 1;
        }
        else
        if(data == '_')
        {
                LATCH = 0;
                DATA = 0;       //G
                clock();
                DATA = 0;       //T
                clock();
                DATA = 0;       //S
                clock();
                DATA = 0;       //U
                clock();
                DATA = 0;       //H
                clock();
                DATA = 0;       //K
                clock();
                DATA = 0;       //M
                clock();
                DATA = 0;       //A
                clock();
                DATA = 0;        //B
                clock();
                DATA = 0;       //N
                clock();
                DATA = 0;       //C
                clock();
                DATA = 0;       //P
                clock();
                DATA = 0;       //R
                clock();
                DATA = 0;       //D
                clock();
                DATA = 0;       //DP
                clock();
                DATA = 1;       //F y E
                clock();
                LATCH = 1;
        }
        else
        if(data == 'Ñ' || data == 'ñ')
        {
                LATCH = 0;
                DATA = 1;       //G
                clock();
                DATA = 0;       //T
                clock();
                DATA = 0;       //S
                clock();
                DATA = 0;       //U
                clock();
                DATA = 1;       //H
                clock();
                DATA = 1;       //K
                clock();
                DATA = 0;       //M
                clock();
                DATA = 0;       //A
                clock();
                DATA = 1;        //B
                clock();
                DATA = 0;       //N
                clock();
                DATA = 1;       //C
                clock();
                DATA = 0;       //P
                clock();
                DATA = 1;       //R
                clock();
                DATA = 1;       //D
                clock();
                DATA = 0;       //DP
                clock();
                DATA = 0;       //F y E
                clock();
                LATCH = 1;
        }
        else
        if(data == '.')
        {
                LATCH = 0;
                DATA = 0;       //G
                clock();
                DATA = 0;       //T
                clock();
                DATA = 0;       //S
                clock();
                DATA = 0;       //U
                clock();
                DATA = 0;       //H
                clock();
                DATA = 0;       //K
                clock();
                DATA = 0;       //M
                clock();
                DATA = 0;       //A
                clock();
                DATA = 0;        //B
                clock();
                DATA = 0;       //N
                clock();
                DATA = 0;       //C
                clock();
                DATA = 0;       //P
                clock();
                DATA = 0;       //R
                clock();
                DATA = 0;       //D
                clock();
                DATA = 1;       //DP
                clock();
                DATA = 0;       //F y E
                clock();
                LATCH = 1;
        }
        
        
        
        
        
        
        






        if(data == ' ')
        {
                LATCH = 0;
                DATA = 0;       //G
                clock();
                DATA = 0;       //T
                clock();
                DATA = 0;       //S
                clock();
                DATA = 0;       //U
                clock();
                DATA = 0;       //H
                clock();
                DATA = 0;       //K
                clock();
                DATA = 0;       //M
                clock();
                DATA = 0;       //A
                clock();
                DATA = 0;        //B
                clock();
                DATA = 0;       //N
                clock();
                DATA = 0;       //C
                clock();
                DATA = 0;       //P
                clock();
                DATA = 0;       //R
                clock();
                DATA = 0;       //D
                clock();
                DATA = 0;       //DP
                clock();
                DATA = 0;       //F y E
                clock();
                LATCH = 1;
        }
        
        
}

