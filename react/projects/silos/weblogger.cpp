
#include <stdio.h>
#include <time.h>
#include <stdint.h>
#include <unistd.h>
#include <stdlib.h>

#define interval_mins (15) // 15 minutes.

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


FILE *log_fp = NULL;
FILE *last_fp = NULL;

/****************************************************************************************/

void open_log(void)
{
  log_fp = fopen("web_write_log.txt", "a+");
  if (log_fp == NULL) perror("web_write_log.txt");
}

/****************************************************************************************/

void save_log_time(time_t the_time)
{
  last_fp = fopen("web_last_write.txt", "w");
  if (last_fp == NULL) {perror("web_last_write.txt"); return;}
  fprintf(last_fp, "%ld", the_time);
  fclose(last_fp);
}

/****************************************************************************************/

int flush_web_log(void)
{
  if (log_fp == NULL) return -1;
  return fflush(log_fp);
}

/****************************************************************************************/

int write_float_to_log(const char *tag, float value, time_t the_time, const char *key)
{
  if (log_fp == NULL) return -1;
  fprintf(log_fp, "%s,float,%0.1f,%ld,%s\n", tag, value, the_time, key); 
  return 0;
}

/****************************************************************************************/

int write_bool_to_log(const char *tag, bool value, time_t the_time, const char *key)
{
  if (log_fp == NULL) return -1;
  fprintf(log_fp, "%s,bool,%c,%ld,%s\n", tag, value ? '1' : '0', the_time, key); 
  return 0;
}

/****************************************************************************************/

int main(int argc, char *argv[])
{
  time_t now;
  struct tm nowtm;
  struct tm nexttm;

  open_log();

  FILE *fp1= fopen("out_temp_hum_sim.txt", "r");

  for (int i=0; i < 96; i++)
  {
    int n;
    char line[300];
    fgets(line, sizeof(line), fp1);
    sscanf(line, "%d\t%f\t%f", &n, &temps[i], &hums[i]);
    printf("------- %d\t%0.1f\t%0.1f\n", n, temps[i], hums[i]);
  }

  now = time(NULL);

  int next_secs = 0;
  
  localtime_r(&now, &nowtm); 
  printf("%s%ju secs since the Epoch\n", asctime(&nowtm), (uintmax_t)now); 
  int secs_after_hour = nowtm.tm_sec + (nowtm.tm_min * 60);
  printf("Secs after hour: %d\n", secs_after_hour);

  for (int i=0; ((i * interval_mins * 60) < (secs_after_hour)); i++)
  {
    printf("%d: %d\n", i, i * interval_mins * 60);   
    next_secs = (i + 1) * interval_mins * 60;   
    printf("next: %d\n", next_secs);   
  }  
  nowtm.tm_min = 0;
  nowtm.tm_sec = 0;
  time_t next_time =  mktime(&nowtm);
  next_time += next_secs;
  localtime_r(&next_time, &nowtm); 
  
  int sim_idx = (nowtm.tm_hour * 4) + (nowtm.tm_min / 15);

  sim_idx = sim_idx % 96; // make sure it is less than 96!
  
  printf("next: %s, idx = %d, next_t = %0.1f, next_h = %0.1f\n", 
       asctime(&nowtm), sim_idx, temps[sim_idx], hums[sim_idx]); 


  bool fans_on = ((hums[sim_idx] > 72) && (hums[sim_idx] < 78));
  printf("To start, fans are %s\n", fans_on ? "ON" : "OFF");

  const char *url = "http://adaptivertc.pablasso.com/api/";
  const char *key = "examplesilokey";
  int rv1;
  int rv2;

  while (true)
  {
    now = time(NULL);
    if (now >= next_time)
    {
      localtime_r(&now, &nowtm); 
      printf("logged at %s, idx = %d, temp = %0.1f, hum = %0.1f\n", 
             asctime(&nowtm), sim_idx, temps[sim_idx], hums[sim_idx]); 

      write_float_to_log("temp_amb", temps[sim_idx], now, key);
      write_float_to_log("hum_rel", hums[sim_idx], now, key);
      flush_web_log();

      rv1 = write_float_to_web(url, "temp_amb", temps[sim_idx], now, key);
      rv2 = write_float_to_web(url, "hum_rel", hums[sim_idx], now, key);

        /*******
      char cmd[500];
      snprintf(cmd, sizeof(cmd), 
         "curl -d \"tagname=temp_amb&value=%0.1f&time=%ld&api_key=examplesilokey\" http://adaptivertc.pablasso.com/api/float_add",
            temps[sim_idx], now);
      printf("%s\n", cmd);
      printf("result is: ");
      fflush(stdout);
      system(cmd);
      printf("\n");
      snprintf(cmd, sizeof(cmd), 
         "curl -d \"tagname=hum_rel&value=%0.1f&time=%ld&api_key=examplesilokey\" http://adaptivertc.pablasso.com/api/float_add",
            hums[sim_idx], now);
      printf("%s\n", cmd);
      printf("result is: ");
      fflush(stdout);
      system(cmd);
      printf("\n");
       ********/

      bool fans_test = ((hums[sim_idx] > 72.0) && (hums[sim_idx] < 78.0));
      printf("Fans are now %s\n", fans_test ? "ON" : "OFF");
      if (fans_test != fans_on) // The state of the fans changed
      {
        printf("------------ Fans just turned %s!! ---------------\n", fans_test ?"ON":"OFF");
        fans_on = fans_test; 

        write_bool_to_log("ventilador", fans_on, now, key);
        rv1 = write_bool_to_web(url, "ventilador", fans_on, now, key);
        flush_web_log();
        /*********
        snprintf(cmd, sizeof(cmd), 
         "curl -d \"tagname=ventilador&value=%c&time=%ld&api_key=examplesilokey\" http://adaptivertc.pablasso.com/api/bool_add",
            fans_on ? '1':'0', now);
        printf("%s\n", cmd);
        printf("result is: ");
        fflush(stdout);
        int rv = system(cmd);
        printf("\nReturned: %d\n", rv);
        *********/
        printf("-------------------------------------------\n");
        
      }

      next_time += interval_mins * 60;
      localtime_r(&next_time, &nexttm); 
      printf("next: %s\n", asctime(&nexttm)); 
      sim_idx = (sim_idx + 1) % 96;
    }
    sleep(1);
  }


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



