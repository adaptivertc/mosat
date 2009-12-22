
#include <stdio.h>
#include <time.h>
#include <stdint.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include <fcntl.h>
#include <sys/stat.h>
#include <mqueue.h>

#include "../../silodata.h"

FILE *log_fp = NULL;
FILE *last_fp = NULL;

const char *log_dir = "/var/flash";
const char *url = "http://adaptivertc.pablasso.com/api/";
const char *qname = "/adaptivertc.react.weblog1";

#define interval_mins (1) // 15 minutes.

mqd_t mq_fd;

float temps[100];
float hums[100];

/*************************************************************************/

static bool day_changed(time_t t1, time_t t2)
{
  struct tm mytm1;
  struct tm mytm2;
  localtime_r(&t1, &mytm1);
  localtime_r(&t2, &mytm2);
  if (mytm1.tm_yday == mytm2.tm_yday)
  {
    return false;
  }
  else
  {
    printf("********* Day Changed\n");
    return true;
  }
}

/*************************************************************************/

void make_day_file_name(time_t the_time, 
      char *fname, int size_fname, const char *pre, const char *post)
{
  const char *loghome = log_dir; 
  if (loghome == NULL)
  {
    loghome = "./";
    printf("Log home not specified, using: %s\n", loghome);
  }

  if (pre == NULL)
  {
    pre = "";
  }
  if (post == NULL)
  {
    post = "";
  }
  char buf1[30];
  struct tm mytm;
  localtime_r(&the_time, &mytm);
  strftime(buf1, sizeof(buf1), "%Y%m%d", &mytm);
  snprintf(fname, size_fname, "%s/%s%s%s", loghome, pre, buf1, post);
}
   

/*************************************************************************/

int write_float_to_web(const char *url, const char *tag, 
                  float value, time_t the_time, const char *key)
{
  char cmd[500];
  snprintf(cmd, sizeof(cmd), 
     "curl -d \"tagname=%s&value=%0.1f&time=%ld&api_key=%s\" %sfloat_add",
       tag, value,the_time, key, url);
  printf("%s\n", cmd);
  printf("result is: ");
  fflush(stdout);
  int retval = system(cmd);
  printf("\nsystem returned: %d\n", retval);
  printf("\n");
  return retval;
}


/*************************************************************************/

int write_bool_to_web(const char *url, const char *tag, 
              bool value, time_t the_time, const char *key)
{
  char cmd[500];
  snprintf(cmd, sizeof(cmd), 
     "curl -d \"tagname=%s&value=%c&time=%ld&api_key=%s\" %sbool_add",
       tag, value ? '1' : '0', the_time, key, url);
  printf("%s\n", cmd);
  printf("result is: ");
  fflush(stdout);
  int retval = system(cmd);
  printf("\nsystem returned: %d\n", retval);
  printf("\n");
  return retval;
}

/*************************************************************************/

int write_float_to_log(const char *tag, float value, time_t the_time, const char *key)
{
  printf("-------------- Writing %s float to log\n", tag);
  if (log_fp == NULL) 
  {
    printf("******* log file fp is NULL\n");
    return -1;
  }
  return fprintf(log_fp, "%s,float,%0.1f,%ld,%s\n", tag, value, the_time, key);
}

/*************************************************************************/

int write_bool_to_log(const char *tag, bool value, time_t the_time, const char *key)
{
  printf("-------------- Writing %s bool to log\n", tag);
  if (log_fp == NULL) 
  {
    return -1;
    printf("******* log file fp is NULL\n");
  }
  return fprintf(log_fp, "%s,bool,%c,%ld,%s\n", 
        tag, value ? '1' : '0', the_time, key); 

}

/*************************************************************************/


void save_log_time(time_t the_time)
{
  FILE *last_fp;
  last_fp = fopen("web_last_write.txt", "w");
  if (last_fp == NULL) {perror("web_last_write.txt"); return;}
  fprintf(last_fp, "%ld", the_time);
  fclose(last_fp);
}

/*************************************************************************/

const char *next_field(const char *str, char delim)
{
  const char *p = str;
  while (*p != '\0')
  {
    if (*p == delim)
    {
      return (p+1);
    }
    p++;
  }
  return NULL;
}

/*************************************************************************/

void copy_to_delim(char *str_to, int str_to_size, const char *str_from, char delim)
{
  for (int i=0; i < str_to_size; i++)
  {
    if ((str_from[i] == delim) || (str_from[i] == '\n') || (str_from[i] == '\r') ||  (str_from[i] == '\0'))
    {
      str_to[i] = '\0';
      printf("\n");
      return;
    }
    printf("'%c' ", str_from[i]);
    str_to[i] = str_from[i];
  }
  printf("\n");
  str_to[str_to_size -1] = '\0';
} 

/*************************************************************************/

int read_log_line(const char *line, char *tag, int tag_max, 
      char *type, int type_max, char *value, int value_max, 
      time_t *the_time, char *key, int key_max)
{
  const char *p = line;

  copy_to_delim(tag, tag_max, p, ','); 

  p = next_field(p, ',');
  if (p == NULL) return -1; 

  copy_to_delim(type, type_max, p, ','); 

  p = next_field(p, ',');
  if (p == NULL) return -1; 

  copy_to_delim(value, value_max, p, ','); 

  p = next_field(p, ',');
  if (p == NULL) return -1; 

  *the_time = atol(p);

  p = next_field(p, ',');
  if (p == NULL) return -1; 

  copy_to_delim(key, key_max, p, ','); 
  
  return 0;
}

/*************************************************************************/

static FILE *open_day_file(const char * pre, const char *post, FILE *fp)
{
  char fname[500];
  if (fp != NULL)
  {
    fclose(fp);
  }
  time_t now = time(NULL);
  make_day_file_name(now, fname, sizeof(fname), pre, post);

  //printf("Opening %s\n", fname);
  fp = fopen(fname, "a+");
  if (fp == NULL)
  {
    perror(fname);
    printf("**** Error Opening %s\n", fname);
  }
  return fp;
}

/*************************************************************************/

void open_log_append(void)
{
  log_fp = open_day_file("webev", ".txt", log_fp);
  //log_fp = fopen("web_write_log.txt", "a+");
}

/*************************************************************************/

void open_log_read(void)
{
  log_fp = fopen("web_write_log.txt", "r");
  if (log_fp == NULL) 
  {
    perror("web_write_log.txt");
  }
}

int flush_web_log(void)
{
  if (log_fp == NULL) return -1;
  return fflush(log_fp);
}


/*************************************************************************/

int main(int argc, char *argv[])
{
  time_t now;

  if (argc > 1)
  {
    log_dir = argv[1];    
  }

  if (argc > 2)
  {
    url = argv[2];
  }

  if (argc > 3)
  {
    qname = argv[3];
  }
  printf("Logging to: %s\n", log_dir);
  printf("url: %s\n", url);
  printf("queue: %s\n", qname);

  mq_fd = mq_open(qname, O_RDWR | O_CREAT, 0755, NULL);
  if (mq_fd == ((mqd_t) -1))
  {
    perror("mq_open");
  }

  open_log_append();


  now = time(NULL);

  int rv;

  time_t last_time = now;

  while (true)
  {
    unsigned prio;
    char msgbuf[8192];
    silodata_t sdata;
    printf("Waiting for next message . . . \n");
    fflush(stdout);
    int rval = mq_receive(mq_fd, msgbuf, sizeof(msgbuf), &prio);
    if (rval == -1)
    {
      perror("mq_recieve");
    }
    memcpy(&sdata, msgbuf, sizeof(sdata));
    printf("Tag: %s\n", sdata.tag);
    printf("Type: %s\n", sdata.type);
    printf("Key: %s\n", sdata.key);
    printf("Value: %s\n", sdata.value);
    printf("Time: %ld\n", sdata.the_time);

    if (day_changed(last_time, sdata.the_time))
    {
      open_log_append();
      last_time = sdata.the_time;
    }
    

    if (0 == strncasecmp("float", sdata.type, 5))
    {
      write_float_to_log(sdata.tag, 
           atof(sdata.value), sdata.the_time, sdata.key);
      flush_web_log();
      rv = write_float_to_web(url, sdata.tag, 
          atof(sdata.value), sdata.the_time, sdata.key);
      if (rv != 0)
      {
         perror("write_float_to_web\n");
         printf("Return Value: %d\n", rv);
      }
    }
    else if (0 == strncasecmp("bool", sdata.type, 4))
    {
      write_bool_to_log(sdata.tag, 
          sdata.value[0] != 0, sdata.the_time, sdata.key);
      flush_web_log();
      rv = write_bool_to_web(url, sdata.tag, 
          sdata.value[0] != '0', sdata.the_time, sdata.key);
      if (rv != 0)
      {
         perror("write_bool_to_web\n");
         printf("Return Value: %d\n", rv);
      }
    }
    else
    {
      printf("****** Bad type in message: %s\n", sdata.type);
    }

  }

  /*******
  char line[300];
  char tag[50];
  char type[50];
  char value[50];
  time_t the_time;
  char key[50];

  for (int i=0; NULL !=  fgets(line, sizeof(line), log_fp); i++)
  {
    printf("-------------\n");
    read_log_line(line, tag, sizeof(tag), type, sizeof(type), value, sizeof(value), &the_time, key, sizeof(key));
    printf("tag: %s\ntype: %s\nvalue: %s\ntime: %ld\nkey %s\n", tag, type, value, the_time, key);
    if (0 == strncasecmp("float", type, 5))
    {
      rv = write_float_to_web(url, tag, atof(value), the_time, key);
      if (rv != 0)
      {
         perror("write_float_to_web\n");
         printf("Return Value: %d\n", rv);
      }
    }
    else if (0 == strncasecmp("bool", type, 4))
    {
      rv = write_bool_to_web(url, tag, value[0] == '1', the_time, key);
      if (rv != 0)
      {
         perror("write_float_to_web\n");
         printf("Return Value: %d\n", rv);
      }
    }
    else
    {
      printf("****** Line %d, Bad type: %s\n", i, type);
    }
  }
      //rv1 = write_float_to_web(url, "temp_amb", temps[sim_idx], now, key);
      //rv2 = write_float_to_web(url, "hum_rel", hums[sim_idx], now, key);
      //rv1 = write_bool_to_web(url, "ventilador", fans_on, now, key);
**********/

  return(0);
}


//       struct tm *localtime_r(const time_t *timep, struct tm *result);
//       time_t mktime(struct tm *tm);
//
 //          struct tm {
 //              int tm_sec;         /* seconds */
 //              int tm_min;         /* minutes */
 //              int tm_hour;        /* hours */
 //              int tm_mday;        /* day of the month */
 //              int tm_mon;         /* month */
 //              int tm_year;        /* year */
 //              int tm_wday;        /* day of the week */
 //              int tm_yday;        /* day in the year */
 //              int tm_isdst;       /* daylight saving time */
 //          };



