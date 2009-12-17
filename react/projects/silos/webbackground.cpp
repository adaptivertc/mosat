
#include <stdio.h>
#include <time.h>
#include <stdint.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define interval_mins (1) // 15 minutes.


float temps[100];
float hums[100];

/****************************************************************************************/

int write_float_to_web(const char *url, const char *tag, float value, time_t the_time, const char *key)
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


/****************************************************************************************/

int write_bool_to_web(const char *url, const char *tag, bool value, time_t the_time, const char *key)
{
  char cmd[500];
  snprintf(cmd, sizeof(cmd), 
     "curl -d \"tagname=%s&value=%c&time=%ld&api_key=%s\" %sfloat_add",
       tag, value ? '1' : '0', the_time, key, url);
  printf("%s\n", cmd);
  printf("result is: ");
  fflush(stdout);
  int retval = system(cmd);
  printf("\nsystem returned: %d\n", retval);
  printf("\n");
  return retval;
}

/****************************************************************************************/


/****************************************************************************************/

void open_log(void)
{
}

/****************************************************************************************/

void save_log_time(time_t the_time)
{
  FILE *last_fp;
  last_fp = fopen("web_last_write.txt", "w");
  if (last_fp == NULL) {perror("web_last_write.txt"); return;}
  fprintf(last_fp, "%ld", the_time);
  fclose(last_fp);
}

/****************************************************************************************/

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
/****************************************************************************************/

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

/****************************************************************************************/

int read_log_line(const char *line, char *tag, int tag_max, char *type, int type_max, char *value, int value_max, time_t *the_time, char *key, int key_max)
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

/****************************************************************************************/

int main(int argc, char *argv[])
{
  time_t now;
  //struct tm nowtm;
  //struct tm nexttm;

  FILE *log_fp = NULL;
  log_fp = fopen("web_write_log.txt", "r");
  if (log_fp == NULL) perror("web_write_log.txt");

  now = time(NULL);
  char tag[50];
  char type[50];
  char value[50];
  time_t the_time;
  char key[50];

  const char *url = "http://adaptivertc.pablasso.com/api/";
  int rv;
  char line[300];

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
      rv = write_bool_to_web(url, tag, value[0] == 1, the_time, key);
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



