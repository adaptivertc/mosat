

#include <stdio.h>
#include <stdint.h>

#include "mod_server_msg.h"
#include "rt_modbus_crc.h"

uint8_t buf[512];
uint8_t ascii[1024];

void doit(void)
{
  int min_size;
  int total_size;
  int reply_size;

  min_size = rt_modbus_min_bytes(buf);
  printf("Min size = %d\n", min_size);
  total_size = rt_modbus_total_bytes(buf, min_size);
  printf("Total size = %d\n", total_size);

  if (check_CRC(buf, total_size, 0xFFFF))
  {
     printf("CRC matches\n");
  }
  else
  {
     printf("*** BAD CRC\n");
  }

  binary_to_hexascii(ascii, buf, total_size);
  printf("Message: %s\n", ascii);

  reply_size = rt_modbus_process_request(buf, total_size);
  printf("Reply size = %d\n", reply_size);

  binary_to_hexascii(ascii, buf, reply_size);
  printf("Reply: %s\n", ascii);

  if (check_CRC(buf, reply_size, 0xFFFF))
  {
     printf("CRC matches\n");
  }
  else
  {
     printf("*** BAD CRC\n");
  }
}

/*********/

int main(int argc, char *argv[])
{
  int min_size;
  int total_size;
  int reply_size;
  buf[0] = 5;
  buf[1] = 1;
  buf[2] = 0;
  buf[3] = 0x99;
  buf[4] = 0;
  buf[5] = 55;
  add_CRC(buf, 8, 0xFFFF);

  printf("\nExample 1 ---------------------\n\n");
  doit();

  buf[0] = 0x05;
  buf[1] = 0x10;
  buf[2] = 0x00;
  buf[3] = 0x01;
  buf[4] = 0x00;
  buf[5] = 0x02;
  buf[6] = 0x04;
  buf[7] = 0x0A;
  buf[8] = 0x0B;
  buf[9] = 0x01;
  buf[10] = 0x02;
  add_CRC(buf, 13, 0xFFFF);

  printf("\nExample 2 ---------------------\n\n");
  doit();
  
}
