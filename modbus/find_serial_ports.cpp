

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>

#include <errno.h>
#include <signal.h>

#include "rt_serial.h" 

static void my_sighandler(int sig)
{
}


int main(int argc, char *argv[])
{
   char dev_name[100];
   char msg[100];
   int device[50];
   int baudrate;
   int i, j;

   signal(SIGIO , my_sighandler);

   rt_verbose = 1;

   if (argc < 3)
   {
     printf("Error, must be at least 2 arguments\n");
     printf("find_serial_ports <baude rate> <port 1> <port 2> . . . <port N>\n");
   }
   
   printf("Using a baudrate of %s for all ports\n", argv[1]);
   baudrate = atoi(argv[1]);

   for (int i=2; i < argc; i++)
   {
     snprintf(dev_name, sizeof(dev_name), "/dev/%s", argv[i]);
     printf("Opening device (%s). . . \n", dev_name);
     device[i] = rt_open_serial(dev_name, baudrate, 2.0);
     if (device[i] < 0)
     {
       printf("device could not be opened: %s\n", dev_name);
     }
     else
     {
       printf("Device opened: %s, fd = %d\n", dev_name, device[i]);
     }
   }
   
   while (true)
   {
     for (int i=2; i < argc; i++)
     {
       if (device[i] < 0) continue;
     
       snprintf(dev_name, sizeof(dev_name), "/dev/%s", argv[i]);
       printf("Writing 'this is %s' to %s\n\r", dev_name, dev_name);
       snprintf(msg, sizeof(msg), "this is %s\n\r", dev_name);
       int bytes = write(device[i],msg,strlen(msg)); 
       if (bytes != strlen(msg))
       {
         printf("Can not write to device, return value = %d\n", bytes);
       }
     }
     printf("\n");
    struct timespec remain;
    struct timespec request;
    request.tv_nsec = 0; 
    request.tv_sec = 2; 
    //printf("waiting (%ld, %ld)\n", request.tv_sec, request.tv_nsec);
    int retval = nanosleep(&request, &remain);
    while (retval == -1)
    {
      if (errno == EINTR)
      {
        request = remain; // Ok, a signal screwed things up, sleep again.
        printf("waiting for rest(%ld, %ld)\n", request.tv_sec, request.tv_nsec);
        retval = nanosleep(&request, &remain);
      }
      else if (errno == EINVAL)
      {
        printf("Error in utimer code %s:%d\n", __FILE__, __LINE__);
        break;
      }
      else if (errno == EFAULT)
      {
        printf("System error in utimer (EFAULT) %s:%d\n", __FILE__, __LINE__);
        break;
      }
      else
      {
        break; // Should never happen, but break to avoid infinite loop; 
      }

    }
    
   }

   for (int i=2; i < argc; i++)
   {
     if (device[i] < 0) continue;
     rt_close_serial(device[i]);
   }

   return 0;
}



