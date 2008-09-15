
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "rt_serial.h"
#include "simple_ascii.h"


int init_simple_ascii(const char *device, char *error, int size)
{
  int serial_fd; 
  rt_verbose = 1;
  printf("Opening device %s . . . \n", device);
  serial_fd = rt_open_serial(device ,9600,0.0);
  printf("Serial fd: %d \n", serial_fd);
  if (serial_fd < 0)
  {
    perror(device);
  }
  return serial_fd;
}

/******************************************************************/

int read_simple_ascii(int fd, 
           double *ai_vals, int max_ai, 
           bool *di_vals, int max_di, 
           bool *do_vals, int max_do, 
           char *error, int size)
{
  char buf[100];
  write(fd, "R", 1);
  int ret_val = rt_read_serial(fd, buf, 4);
  buf[4] = '\0';
  if (ret_val < 0)
  {
    printf("Error reading serial port\n");
  }
  int n = atol(buf);
  for (int i=0; ((i < n) && (i < max_ai)); i++)
  {
    ret_val = rt_read_serial(fd, buf, 12);
    buf[12] = '\0';
    if (ret_val < 0)
    {
      printf("Error reading serial port\n");
    }
    ai_vals[i] = atof(buf);
  }

  ret_val = rt_read_serial(fd, buf, 14);
  buf[14] = '\0';
  if (ret_val < 0)
  {
    printf("Error reading serial port\n");
  }
  for (int i=0; i < 12; i++)
  {
    di_vals[i] = (buf[i] == '1');
  }
  
  ret_val = rt_read_serial(fd, buf, 14);
  buf[14] = '\0';
  if (ret_val < 0)
  {
    printf("Error reading serial port\n");
  }
  for (int i=0; i < 12; i++)
  {
    do_vals[i] = (buf[i] == '1');
  }
  
  return 0;
  return 0;
}

/******************************************************************/

int send_simple_ascii(int fd, int channel, bool val,        
           char *error, int size)
{
  char buf[10];
  write(fd, "W", 1);
  int ret_val = rt_read_serial(fd, buf, 4);
  buf[4] = '\0';
  if (ret_val < 0)
  {
    printf("Error reading serial port\n");
  }
  if (0 == strncasecmp(buf, "OK", 2))
  {
    return 0;
  }
  else
  {
    int err = atoi(buf);
    printf("******* Error sending do: %d\n", err);
    return err;
  }
  return 0;
}

