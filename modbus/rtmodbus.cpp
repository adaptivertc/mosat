
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "rtcommon.h"
#include "rt_serial.h"
#include "mtype.h"
#include "modio.h"
#include "hprtu.h"
#include "hp2mod.h"
#include "trace.h"

rtmodbus_t *rt_create_modbus_serial(const char *device_name, int baudrate, float timeout)
{
  rt_mod_serial_client_t *msc = new rt_mod_serial_client_t(device_name, baudrate, timeout);
  MODSerial *modc = new MODSerial(msc);
  return modc;
}

rtmodbus_t *rt_create_modbus(const char *device)
{
  // For right now, it only works with TCP/IP, but we need
  // to make it work with serial devices as well.
  printf("Creating a modbus connection: %s\n", device);
  char dstr[100];
  safe_strcpy(dstr, device, sizeof(dstr));
  char *p = dstr;
  const char *dest_port = "502";
  char *dest_ip = dstr;
  while (*p != '\0')
  {
    if (*p == ':')
    {
       *p = '\0';
       p++;
       dest_port = p; 
       break;
    }
    p++;
  }
  printf("ip = %s, port =%s\n", dest_ip, dest_port);

  int sockfd;
  struct sockaddr_in dest_addr;   /* will hold the destination addr */

  sockfd = socket(AF_INET, SOCK_STREAM, 0); /* do some error checking! */
  if (sockfd < 0)
  {
    react_trace.dperror(6, "Can't create socket");
    exit(0);
  }

  struct timeval tv;
  tv.tv_sec = 2;
  tv.tv_usec = 0;
  int err;

  err = setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));
  if (err != 0)
  {
    react_trace.dperror(6, "setsockopt");
  }

  err = setsockopt(sockfd, SOL_SOCKET, SO_SNDTIMEO, &tv, sizeof(tv));
  if (err != 0)
  {
    react_trace.dperror(6, "setsockopt");
  }

  dest_addr.sin_family = AF_INET;        /* host byte order */
  short temp = (short) atol(dest_port); 
  dest_addr.sin_port = htons(temp); /* short, network byte order */
  dest_addr.sin_addr.s_addr = inet_addr(dest_ip);
  printf("Using %s\n", dest_ip);
  memset(&(dest_addr.sin_zero), 8, 0);       /* zero the rest of the struct */

  printf("Connecting on %s\n", dest_port);
  int n = connect(sockfd, (struct sockaddr *)&dest_addr,
                  sizeof(struct sockaddr));
  if (n < 0)
  {
    react_trace.dperror(6, "Can't connect to port");
    exit(0); 
  }

  mod_tcpip_client_t *mtp = new mod_tcpip_client_t(sockfd, &dest_addr);
  MODSerial *modc = new MODSerial(mtp);
  return modc;
}


