
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>

#include "rt_serial.h"
#include "rt_modbus_crc.h"

extern "C"
{
#include "mod_server_msg.h"
}

int main(int argc, char *argv[])
{
  const char *serial_dev;
  int serial_fd;
  long baud_rate;
  uint8_t unit_id;
  uint8_t buf[512];
  uint8_t ascii[1025];

  if (argc > 1)
  {
    serial_dev = argv[1];
  }
  else
  {
    serial_dev = "/dev/ttyUSB0";
  }

  baud_rate = 9600;
  if (argc > 2)
  {
    baud_rate = atol(argv[2]);
  } 

  unit_id = 1; 
  if (argc > 3)
  {
    unit_id = (uint8_t) atol(argv[3]);
  } 



  serial_fd = rt_open_serial(serial_dev, baud_rate, 0);
  if (serial_fd == -1)
  {
    perror(serial_dev);
    exit(0);
  }
int message=1;
int replysend=0;
  while (true)
  {

    printf("Waiting for next message . . . \n");
    printf("message number %d \n",message);

    rt_set_timeout(serial_fd, 0.0);  // for the first byte, wait forever!!

    int n = rt_read_serial(serial_fd, buf, 1); 

    if (n != 1)
    {
      perror("rt_read_serial");
      continue;
    }

    rt_set_timeout(serial_fd, 2.0); // after the first byte, wait a maximum of 2 seconts. 

    n = rt_read_serial(serial_fd, buf+1, 1); 

    message=message+1;

    printf("First read: %d bytes\n", n);

    if (n != 1)
    {
      perror("rt_read_serial");
      continue;
    }

    int min_size =  rt_modbus_min_bytes(buf);

    if (min_size < 0)
    {
      printf("Unknow opcode . . . . \n");
      continue;
    }

    n = rt_read_serial(serial_fd, buf + 2, min_size - 2); 

    printf("Second read: %d bytes, expecting %d\n", n, min_size - 2);

    if (n != (min_size - 2))
    {
      perror("rt_read_serial");
      continue;
    }

    int total_size = rt_modbus_total_bytes(buf, min_size);

    if (total_size < 0)
    {
      printf("Error in message . . . . \n");
      continue;
    }


    if (total_size > min_size)
    {
      n = rt_read_serial(serial_fd, buf + min_size, total_size - min_size); 

      printf("Third read: %d bytes, expecting %d\n", n, total_size - min_size);
   
      if (n != (total_size - min_size))
      {
        perror("rt_read_serial");
        continue;
      }
    }
    else
    {
      printf("Third read not needed\n");
    }

    binary_to_hexascii(ascii, buf, total_size);
    printf("Message: %s\n", ascii);

    printf("Unit id in message is: %d, my unit id is: %d\n", 
             (int) buf[0], (int) unit_id);
    if (unit_id != buf[0])
    {
      printf("The message is NOT for me, ignoring\n");
      continue;
    }
    else
    {
      printf("The message is MINE, processing . . . \n");
    }

    int reply_size = rt_modbus_process_request(buf, total_size);

    add_CRC(buf, reply_size, 0xffff);

    binary_to_hexascii(ascii, buf, reply_size);
    printf("Reply: %s\n", ascii);
    printf("Reply send: %d\n",replysend);
    replysend=replysend+1;
    write(serial_fd, buf, reply_size);
  }
}
