
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

/*************************************************************************/

void spd_create_image(const char *base_name, const char *gtitle, bool window)
{
  const char *fname = "gnuplotoptions.txt";
  FILE *fp = fopen(fname, "w");
  if (fp == NULL)
  {
    perror(fname);
    return;
  }
  fprintf(fp, "set   autoscale\n");
  fprintf(fp, "unset log\n");
  fprintf(fp, "unset label\n");
  fprintf(fp, "set xtic auto\n");
  fprintf(fp, "set ytic auto\n");
  fprintf(fp, "set grid\n");
  fprintf(fp, "set xlabel \"fecha-tiempo\"\n");
  fprintf(fp, "set ylabel \"%%humedad y temp\"\n");

  fprintf(fp, "set title \"%s\"\n", gtitle);

  // The following line is basic black and white
  //fprintf(fp, "set terminal png size 800,600 xffffff x000000 x000000 x000000\n");
  // The following is default, probably black and white with red data
  //fprintf(fp, "set terminal png size 800,600\n");
  //The follow line is light blue background, blue font, magenta data line
  if (!window)
  {
    fprintf(fp, "set terminal png size 1000,720\n");
                    //xadd8e6 x0000ff xdda0dd x9500d3\n");
    //fprintf(fp, "set terminal png small color\n");
  }
  fprintf(fp, "%s\n", "set output\n");
  fprintf(fp, "set xdata time\n");
  //fprintf(fp, "set timefmt \"%%s\"\n");
  fprintf(fp, "set timefmt \"%%Y/%%m/%%d-%%H:%%M:%%S\"\n");
  fprintf(fp, "plot \"%s.txt\" using 1:2 with lines lw 2 title \"temperatura\"", base_name);
  fprintf(fp, ", \"%s.txt\" using 1:3 with lines lw 2 title \"humedad\"", base_name);
  //fprintf(fp, ", \"%s.txt\" using 1:3 with lines lw 2 title \"low\"", base_name);
  //fprintf(fp, ", \"%s.txt\" using 1:5 with lines lw 2 title \"high\"", base_name);
  //fprintf(fp, ", \"%s.txt\" using 1:6 with lines lw 2 title \"vhigh\"", base_name);
  fprintf(fp, "\n");

  fclose(fp);
  char command[500];
  if (window)
  {
    snprintf(command, sizeof(command), "gnuplot -persist %s", fname);
  }
  else
  {
    snprintf(command, sizeof(command), "gnuplot %s > %s.png", fname, base_name);
  }

  printf("Cmd: %s", command);

  system(command);

}

/*********************************************************/

void copy_to_delim(char *str_to, int str_to_size, const char *str_from, char delim)
{
  for (int i=0; i < str_to_size; i++)
  {
    if ((str_from[i] == delim) || (str_from[i] == '\n') || (str_from[i] == '\r') ||  (str_from[i] == '\0'))
    {
      str_to[i] = '\0';
      //printf("\n");
      return;
    }
    //printf("'%c' ", str_from[i]);
    str_to[i] = str_from[i];
  }
  //printf("\n");
  str_to[str_to_size -1] = '\0';
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

int read_dat_line(const char *line, char *tag, int tag_max,
      char *value, int value_max,
      time_t *the_time)
{
  const char *p = line;

  copy_to_delim(tag, tag_max, p, ':');

  p = next_field(p, ':');
  if (p == NULL) return -1;

  copy_to_delim(value, value_max, p, ',');

  p = next_field(p, ',');
  if (p == NULL) return -1;

  *the_time = atol(p);

  return 0;
}

/*************************************************************************/

int main(int argc, char *argv[])
{

  const char *input_file = "poncitlan_dat.txt";

  if (argc > 1)
  {
    input_file = argv[1];
  }

  log_fp = fopen(input_file, "r");
  if (log_fp == NULL) 
  {
    perror(input_file);
    exit(1);
  }

  FILE *out_fp = fopen("poncitlan_out.txt", "w");
  if (out_fp == NULL) 
  {
    perror("poncitlan_out.txt");
    exit(1);
  }

  char line[300];
  char tag[50];
  char value1[50];
  char value2[50];
  time_t the_time1;
  time_t the_time2;
  time_t last_time = 0;
  int rv;
  int line_num = 0;
  int n_errors = 0;
  int last_gap =0;
  struct tm mytm;
  

//temp_amb:26,1261249200
//hum_rel:24,1261249200

  for (int i=0; NULL !=  fgets(line, sizeof(line), log_fp); i++)
  {
     //int read_dat_line(const char *line, char *tag, int tag_max,
           //char *value, int value_max,
           //time_t *the_time)

    line_num++;
    printf("-------------\n");
    read_dat_line(line, tag, sizeof(tag), value1, sizeof(value1), &the_time1);
    printf("tag: %s\nvalue1: %s\ntime1: %ld\n", tag, value1, the_time1);
    if (0 != strcmp(tag, "temp_amb")) 
    {
      printf("Line %d: @@@@@@@@@@@@@@@@@@@@@@ NOT temp_amb: %s\n", line_num, tag);
      n_errors++;
    }
    if (NULL !=  fgets(line, sizeof(line), log_fp))
    {
      line_num++;
      read_dat_line(line, tag, sizeof(tag), value2, sizeof(value2), &the_time2);
      printf("tag: %s\nvalue2: %s\ntime2: %ld\n", tag, value2, the_time2);
      if (0 != strcmp(tag, "hum_rel")) 
      {
        printf("Line %d: &&&&&&&&&&&&&&&&&&&& NOT hum_rel: %s\n", line_num, tag);
        n_errors++;
      }
    }
    if (the_time1 != the_time2) 
    {
      printf("Line %d: ^^^^^^^^^^^^^^^^^^^ Bad times\n", line_num);
      n_errors++;
    }
    if ((last_time != 0) && ((the_time1 - last_time) > 1000))
    {
      printf("Line %d: $$$$$$$$$$$$$$ Big gap in time\n", line_num);
      last_gap = line_num;
    }
    last_time = the_time1;

    //struct tm *localtime_r(const time_t *timep, struct tm *result);
    char time_str[100];

    localtime_r(&the_time1, &mytm);

    strftime(time_str, sizeof(time_str), "%Y/%m/%d-%H:%M:%S", &mytm);
    printf("Time: %s\n", time_str);


    fprintf(out_fp, "%s\t%s\t%s\n", time_str, value1, value2);

    

  }
  fclose(out_fp);

  spd_create_image("poncitlan_out", "Humedad Temperatura", true);
  spd_create_image("poncitlan_out", "Humedad Temperatura", false);

  printf("**********\n");
  printf("%d errors\n", n_errors);
  printf("%d last gap\n", last_gap);
  printf("**********\n");
    
    /****
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
    ***/
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



