

#include <stdio.h>

#include "rt_serial.h"

#include "simple_ascii.h"

int main(int argc, char *argv[])
{
   int device;

   char dev_name[60];
   if (argc > 1)
   {
     snprintf(dev_name, sizeof(dev_name), "/dev/%s", argv[1]);
   }
   else
   {
     snprintf(dev_name, sizeof(dev_name), "/dev/ttyUSB0");
   }

   rt_verbose = 1;
   printf("Opening device %s . . . \n", dev_name);
   device = rt_open_serial(dev_name,9600,0.0);
   printf("Device opened: %d \n", device);


   char data[200];
   int ret_val;

  while(true)
  {
    ret_val = rt_read_serial(device, data, 1); 
    if (ret_val < 0)
    {
      printf("Error reading serial port\n");
    }
    else
    {
      printf("Got this: %s\n", data);
    }

    if (data[0] == 'R')
    {
      write(device, "04\n\r", 4);
      write(device, "+12.345678\n\r", 12);
      write(device, "+12.345678\n\r", 12);
      write(device, "+12.345678\n\r", 12);
      write(device, "+12.345678\n\r", 12);
      write(device, "101010101001\n\r", 14);
      write(device, "010000000100\n\r", 14);
    }
    else if (data[0] == 'W')
    {
      ret_val = rt_read_serial(device, data, 3); 
      if (ret_val < 0)
      {
        printf("Error reading serial port\n");
        write(device, "01\n\r", 4);
      }
      int ch = data[1] - '0'; 
      ch += (data[0] - '0') * 10;
      bool val = (data[2] == '1');
      printf("%s channel %d\n", val ? "Turning On" : "Turning OFF", ch);
      write(device, "OK\n\r", 4);
    }
    else
    {
      printf("You FOOL, that is NOT correct\n");
      write(device, "02\n\r", 4);
    }
  }
}
