
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <errno.h>

 #include <pthread.h>

#include "mtype.h"
//#include "modio.h"
//#include "hprtu.h"
//#include "hp2mod.h"
//#include "utimer.h"
#include "evlog.h"

#include "ppc_logger.h"
#include "train_detect.h"

#include "rtcommon.h"
#include "arg.h"

#define DEST_IP   "127.0.0.1"
//#define DEST_IP   "10.25.50.27"
//#define DEST_IP   "10.25.50.46"
//#define DEST_IP   "172.16.115.27"
//#define DEST_IP   "216.109.112.135"

#define DEST_PORT 3490 
//#define DEST_PORT 502 
//#define DEST_PORT 80 

/****************************************************************/

int send_time_seq_num(int fd, time_t the_time, long the_seq_num)
{
  printf("Sending time, seq num: %ld, %ld . . .\n", the_time, the_seq_num);
  the_time = htonl(the_time);
  the_seq_num = htonl(the_seq_num);
  char buf[8];
  memcpy(buf, &the_time, 4);
  memcpy(buf + 4, &the_seq_num, 4);
  int ns = send(fd, buf, sizeof(buf), MSG_DONTWAIT | MSG_NOSIGNAL);
  printf("Sent\n");
  if (ns = 8) return 0;
  else return -1;
}

/****************************************************************/

int reconnect(int sock, struct sockaddr_in the_dest_addr, time_t last_time, long seq_num)
{
  int err = shutdown(sock, SHUT_RDWR);
  if (err != 0)
  {
    perror("shutdown");
  }
  if (0 != close(sock))
  {
    perror("close");
  }
  /***/

  sock = socket(AF_INET, SOCK_STREAM, 0);
  if (sock < 0)
  {
    perror("Can't create socket");
    exit(0);
  }

  struct timeval tv;
  tv.tv_sec = 30;
  tv.tv_usec = 0;
  err = setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));
  if (err != 0)
  {
    perror("setsockopt");
  }
  err = setsockopt(sock, SOL_SOCKET, SO_SNDTIMEO, &tv, sizeof(tv));
  if (err != 0)
  {
    perror("setsockopt");
  }

  int n_tries = 0;
  int sleep_time = 5;
  while (true)
  {
    /* Ok, now try to reconnect */
    int n = connect(sock, (struct sockaddr *)&the_dest_addr,
                    sizeof(struct sockaddr));
    if (n != 0)
    {
      perror("Can't connect to port");
    }
    else
    {
      printf("Connected, Network Up\n");
      time_t now = time(NULL);
      evlog_print_log('-', "NETUP", now, now, 0, 0, 0);
      // Send the last time and sequence number.
      int retval = send_time_seq_num(sock, last_time, seq_num);
      if (retval == 0)
      {
        break;
      }
      else
      {
        perror("Can't send on new socket");
      }
    }
    n_tries++;
    if (n_tries > 10)
    {
      // After 10 tries, only try every minute to reconnect
      sleep_time = 5;
    }
    sleep(sleep_time);
  }
  return sock;
}


/***************************************************************/

struct log_data_t
{
  char *dest_ip;
  unsigned short dest_port; 
  char *local_dir;
};

/***************************************************************/

void *maintain_log(void *data)
{
  log_data_t *log_data;

  log_data = (log_data_t *) data;
   
  int sockfd;
  struct sockaddr_in dest_addr;   /* will hold the destination addr */

  printf("Max fds: %d\n", getdtablesize());
  sockfd = socket(AF_INET, SOCK_STREAM, 0); /* do some error checking! */
  if (sockfd < 0)
  {
    perror("Can't create socket");
    exit(0);
  }

  struct timeval tv;
  tv.tv_sec = 30;
  tv.tv_usec = 0;
  int err = setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv)); 
  if (err != 0)
  {
    perror("setsockopt");
  }
  err = setsockopt(sockfd, SOL_SOCKET, SO_SNDTIMEO, &tv, sizeof(tv)); 
  if (err != 0)
  {
    perror("setsockopt");
  }

  dest_addr.sin_family = AF_INET;        /* host byte order */
  dest_addr.sin_port = htons(log_data->dest_port); /* short, network byte order */
  dest_addr.sin_addr.s_addr = inet_addr(log_data->dest_ip);
  printf("Using %s\n", log_data->dest_ip);
  memset(&(dest_addr.sin_zero), 8, 0);       /* zero the rest of the struct */

  printf("Connecting on %hu\n", log_data->dest_port); 
  int n = connect(sockfd, (struct sockaddr *)&dest_addr, 
		  sizeof(struct sockaddr));
  if (n < 0)
  {
    perror("Can't connect to port");
    exit(0);
  }
  printf("Connected!: %hu\n", log_data->dest_port); 

  evlog_t *evlog = new evlog_t(log_data->local_dir);

  time_t last_time = time(NULL);
  long last_seq_num = evlog->open_files();
  /***
  if (last_seq_num == -1)
  {
    last_time = 0;
  }
  ***/
  send_time_seq_num(sockfd, last_time, last_seq_num);

  while (true)
  {
    crossing_event_data_t ev;
    char recv_buffer[5 * sizeof(long)];
    int nbytes = recv(sockfd, recv_buffer, sizeof(recv_buffer),  MSG_WAITALL);
    if (nbytes != sizeof(recv_buffer))
    {
      printf("Network down on %s\n", log_data->dest_ip);
      perror("recv");
      time_t now = time(NULL);
      evlog->print_log(EV_NETDN, "NETDN", now, now, -1, -1, -1);
      sockfd = reconnect(sockfd, dest_addr, last_time, last_seq_num);
      now = time(NULL);
      evlog->print_log(EV_NETUP, "NETUP", now, now, -1, -1, -1);
    }
    else
    {
      train_detector_t::cev_from_net(&ev, recv_buffer, sizeof(recv_buffer));
    
      last_time = ev.start_time;
      if (ev.seq_num > 0)
      {
        last_seq_num = ev.seq_num;
      }
      char buf[100];
      struct tm mytm;
      localtime_r(&ev.start_time, &mytm);
      strftime(buf, sizeof(buf), "%F\t%T", &mytm);
      printf("%16s:\t%ld\t%s\t%s\t", log_data->dest_ip, ev.id, get_ev_string(ev.id), buf);
      localtime_r(&ev.end_time, &mytm);
      strftime(buf, sizeof(buf), "%T", &mytm);
      printf("%s\t%ld\t%ld\n", buf, ev.n_pulses, ev.seq_num);
      if (ev.id != EV_WATCHDOG)
      {
        evlog->print_log(ev.id, get_ev_string(ev.id), ev.start_time, ev.end_time, ev.n_pulses, ev.seq_num, 0);
      }
    }
  }
}

/***************************************************************/

int main(int argc, char *argv[])
{
  char *fname = "../config_files/record_config.txt";
  FILE *fp = fopen(fname, "r");
  if (fp == NULL)
  {
    perror(fname);
    exit(0);
  }

  char line[300];
  int n_lines = 0;
  pthread_t thread;
  for (int i=0; NULL != fgets(line, sizeof(line), fp); i++)
  {
    char tmp[300];
    int argc;
    char *argv[25];

    printf("Line %d: %s\n", i, line);
    if (line[0] == '#') continue;
    safe_strcpy(tmp, line, sizeof(tmp));
    argc = get_delim_args(tmp, argv, '|', 2);
    if (argc == 0)
    {
      continue;
    }
    if (argc == 2)
    {
      log_data_t *ldata = new log_data_t;
      ldata->dest_ip = strdup(argv[0]);
      ldata->dest_port = DEST_PORT;
      ldata->local_dir = strdup(argv[1]);
      printf("Starting thread: %s, %s\n", ldata->dest_ip, ldata->local_dir); 
      int ret = pthread_create(&thread, NULL, maintain_log, ldata); 
    }
  }
  pthread_join(thread, NULL); // Ok, the main thread will wait
                              // the last thread to exit.
  return 0;
/***
int  pthread_create(pthread_t  * thread, pthread_attr_t * attr, void *
     (*start_routine)(void *), void * arg);


struct log_data_t
{
  char *dest_ip;
  unsigned short dest_port; 
  char *local_dir;
};
******/
}

int main2(int argc, char *argv[])
{
  int sockfd;
  struct sockaddr_in dest_addr;   /* will hold the destination addr */

  printf("Max fds: %d\n", getdtablesize());
  sockfd = socket(AF_INET, SOCK_STREAM, 0); /* do some error checking! */
  if (sockfd < 0)
  {
    perror("Can't create socket");
    exit(0);
  }

  struct timeval tv;
  tv.tv_sec = 30;
  tv.tv_usec = 0;
  int err = setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv)); 
  if (err != 0)
  {
    perror("setsockopt");
  }
  err = setsockopt(sockfd, SOL_SOCKET, SO_SNDTIMEO, &tv, sizeof(tv)); 
  if (err != 0)
  {
    perror("setsockopt");
  }

  dest_addr.sin_family = AF_INET;        /* host byte order */
  dest_addr.sin_port = htons(DEST_PORT); /* short, network byte order */
  dest_addr.sin_addr.s_addr = inet_addr(DEST_IP);
  printf("Using %s\n", DEST_IP);
  memset(&(dest_addr.sin_zero), 8, 0);       /* zero the rest of the struct */

  printf("Connecting on %d\n", DEST_PORT); 
  int n = connect(sockfd, (struct sockaddr *)&dest_addr, 
		  sizeof(struct sockaddr));
  if (n < 0)
  {
    perror("Can't connect to port");
    exit(0);
  }
  printf("Connected!: %d\n", DEST_PORT); 
  time_t last_time = time(NULL);
  long last_seq_num = open_evlog_files();
  if (last_seq_num == -1)
  {
    last_time = 0;
  }
  send_time_seq_num(sockfd, last_time, last_seq_num);

  while (true)
  {
    crossing_event_data_t ev;
    char recv_buffer[5 * sizeof(long)];
    int nbytes = recv(sockfd, recv_buffer, sizeof(recv_buffer),  MSG_WAITALL);
    if (nbytes != sizeof(recv_buffer))
    {
      perror("recv");
      time_t now = time(NULL);
      evlog_print_log(EV_NETDN, "NETDN", now, now, -1, -1, -1);
      sockfd = reconnect(sockfd, dest_addr, last_time, last_seq_num);
      now = time(NULL);
      evlog_print_log(EV_NETUP, "NETUP", now, now, -1, -1, -1);
    }
    else
    {
      train_detector_t::cev_from_net(&ev, recv_buffer, sizeof(recv_buffer));
    
      last_time = ev.start_time;
      last_seq_num = ev.seq_num;
      char buf[100];
      struct tm mytm;
      localtime_r(&ev.start_time, &mytm);
      strftime(buf, sizeof(buf), "%F\t%T", &mytm);
      printf("%ld\t%s\t%s\t", ev.id, get_ev_string(ev.id), buf);
      localtime_r(&ev.end_time, &mytm);
      strftime(buf, sizeof(buf), "%F\t%T", &mytm);
      printf("%s\t%ld\t%ld\n", buf, ev.n_pulses, ev.seq_num);
      evlog_print_log(ev.id, get_ev_string(ev.id), ev.start_time, ev.end_time, ev.n_pulses, ev.seq_num, 0);
    }
  }
  

/*****
void print_log(char the_id, char *tp, time_t start_time, time_t end_time,
                int n_counts, int start_count, int end_count)
******/


}


