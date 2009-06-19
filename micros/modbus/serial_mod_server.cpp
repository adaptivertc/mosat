
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


  serial_fd = rt_open_serial(serial_dev, 9600, 0);
  if (serial_fd == -1)
  {
    perror(serial_dev);
    exit(0);
  }

  while (true)
  {
    printf("Waiting for next message . . . \n");
    int n = rt_read_serial(serial_fd, buf, 2); 

    printf("First read: %d bytes\n", n);

    if (n != 2)
    {
      perror("rt_read_serial");
      return 0; 
    }

    int min_size =  rt_modbus_min_bytes(buf);

    n = rt_read_serial(serial_fd, buf + 2, min_size - 2); 

    printf("Second read: %d bytes, expecting %d\n", n, min_size - 2);

    if (n != (min_size - 2))
    {
      perror("rt_read_serial");
      return 0; 
    }

    int total_size = rt_modbus_total_bytes(buf, min_size);

    if (total_size > min_size)
    {
      n = rt_read_serial(serial_fd, buf + min_size, total_size - min_size); 

      printf("Third read: %d bytes, expecting\n", n, total_size - min_size);
   
      if (n != (total_size - min_size))
      {
        perror("rt_read_serial");
        return 0; 
      }
    }
    else
    {
      printf("Third read not needed\n");
    }

    binary_to_hexascii(ascii, buf, total_size);
    printf("Message: %s\n", ascii);

    int reply_size = rt_modbus_process_request(buf, total_size);

    add_CRC(buf, reply_size, 0xffff);

    binary_to_hexascii(ascii, buf, reply_size);
    printf("Reply: %s\n", ascii);

    write(serial_fd, buf, reply_size);
  }
}
