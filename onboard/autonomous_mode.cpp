

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <sys/time.h>

#include <string.h>

#include "rtcommon.h"

#include "utimer.h"

#include "onboard.h"

#include "spd_comm.h"

#include "ob_config.h"

#define MAX_MAINTENANCE_SPEED (35)
#define MAX_TIME_AT_MAINTENANCE (18)
#define MAX_TIME_AT_ZERO (18)
#define RECORD_SIZE (300)

enum service_mode_t
{
  MAINTENANCE_STOPPED,
  MAINTENANCE_MOVING,
  IN_SERVICE,
};

struct record_data_t
{
  time_t time;
  double speed;
  double distance;
  bool door_open;
};

#define TV_ELAPSED_US(x, y)     ((((x).tv_sec - (y).tv_sec) * 1000000) + \
        ((x).tv_usec - (y).tv_usec))

onboard_config_t *onboard_config = NULL;

/**
void connect_modbus(void);
void get_actual_speed_dist(int section, int t, double *dist, double *speed, spd_discrete_t *discretes);
void init_io(void);
***/
static double speeds[] = 
          {0.0, 5.0, 10.0, 15.0, 20.0, 25.0, 30.0, 40.0, 45.0, 50.0, 
             -1.0, 50, 40, 30, 20, 10, -1, 0, -1};
static int speeds_sz = sizeof(speeds) / sizeof(speeds[0]);
static int sidx = 0;
static double last;
static bool maintain_on = false;
static int maintain_counter = 0;

const char *get_mode_text(service_mode_t service_mode)
{
  switch (service_mode)
  {
    case MAINTENANCE_STOPPED:
      return "MAINTENANCE_STOPPED";
    case MAINTENANCE_MOVING:
      return "MAINTENANCE_MOVING";
    case IN_SERVICE:
      return "IN_SERVICE";
  }
  return "";
}

void xxget_actual_speed_dist(int a, int b, 
  double *distance, double *actual_speed, spd_discrete_t *discretes)
{
  *distance = 0.0;
  if (maintain_on)
  {
    maintain_counter++;
    if (maintain_counter > 20)
    {
      maintain_on = false;
    }
    *actual_speed = last;
    return;
  }
  else if (speeds[sidx] < 0.0)
  {
    maintain_on = true;
    maintain_counter = 0;
    sidx = (sidx + 1) % speeds_sz;
    *actual_speed = last;
    return;
  }  
  last = speeds[sidx]; 
  sidx = (sidx + 1) % speeds_sz;
  *actual_speed = last;
  return;
}

/*********************************************************************/

static int file_number = 0;

FILE *open_next_file(char *dir)
{
  char fname[250];
  snprintf(fname, sizeof(fname), "%s/p%02d.txt", dir, file_number++);
  printf("Opening File: %s\n", fname);
  FILE *fp = fopen(fname, "w");
  return fp;
}

/*********************************************************************/

void write_past_data(FILE *fp, record_data_t *data, int position, int n)
{
  if (fp == NULL) return;
  int idx = position;
  printf("Writing past data, position = %d . . . \n", position);
  for(int i=0; i < (n-1); i ++)
  {
    if (data[idx].speed == 0.0)
    {
      break; 
    }
    idx--;
    if (idx < 0) idx = n - 1; 
  }
  printf("Found zero at %d\n", idx);
  for (; true; idx++)
  {
    if (idx >= n) idx = 0;

    struct tm mytm;
    char buf[20];
    localtime_r(&data[idx].time, &mytm);
    strftime(buf, sizeof(buf), "%T", &mytm);
    char linebuf[200];
    snprintf(linebuf, sizeof(linebuf), "%s\t%lf\t%lf\t%s\n", 
        buf, data[idx].distance, data[idx].speed, 
        data[idx].door_open ? "1" : "0");
    printf("%s", linebuf);
    fprintf(fp, "%s", linebuf);

    if (idx == position) break;
  }
}

/*********************************************************************/

int main(int argc, char *argv[]) 
{
  const char *config_file = "onboard_config.txt";

  int current_arg;
  for (current_arg=1; current_arg < argc; current_arg++)
  {
    if (0 == strcasecmp(argv[current_arg], "-c"))
    {
      if (argc > (current_arg + 1))
      {
        current_arg++;
        config_file = argv[current_arg];
        printf("Setting the config file name to %s \n", config_file);
      }
      else
      {
        printf("Can't read the config file name, not enough args\n");
      }
    }
  }

  onboard_config = new onboard_config_t();
  onboard_config->read_file(config_file);

  utimer_t utimer;
  utimer.set_busy_wait(false);
  utimer.set_interval(1000000);
  utimer.set_start_time();
  spd_discrete_t discretes;
  double distance;
  double actual_speed; 
  discretes.doors_open = false;

  int time_below_speed = 0;
  int time_at_zero = 0;;

  char dirname[200];

  create_profile_dir("./raw_profiles/", dirname, sizeof(dirname));


  //init_io();
  //reset_distance(0);

  time_t the_time = time(NULL);
  record_data_t record_data[RECORD_SIZE];
  for (int i=0; i < RECORD_SIZE; i++)
  {
    record_data[i].time = the_time;
    record_data[i].speed = 0.0;
    record_data[i].distance = 0.0;
    record_data[i].door_open = false;
  }
  int record_pos = 0;
  FILE *pfp = NULL;

  service_mode_t service_mode = MAINTENANCE_STOPPED;
  printf("Starting state is MAINTENANCE STOPPED\n"); 
  while (1)
  {
    utimer.wait_next();
    the_time++;
 
    xxget_actual_speed_dist(0, 0, &distance, &actual_speed, &discretes); 
    printf("%d: Distance: %lf, Speed: %lf, %s\n", record_pos, distance, actual_speed, get_mode_text(service_mode));

    record_data[record_pos].time = the_time;
    record_data[record_pos].speed = actual_speed;
    record_data[record_pos].distance = distance;
    record_data[record_pos].door_open = discretes.doors_open;

    if ((service_mode == IN_SERVICE) && (pfp != NULL))
    {
      struct tm mytm;
      char buf[20];
      localtime_r(&the_time, &mytm);
      strftime(buf, sizeof(buf), "%T", &mytm);
      fprintf(pfp, "%s\t%lf\t%lf\t%s\n", buf, distance, actual_speed, discretes.doors_open ? "1" : "0");
    }


    switch(service_mode)
    {
      case MAINTENANCE_STOPPED:
        if (actual_speed > 0.0)
        {
          service_mode = MAINTENANCE_MOVING;
          printf("Switching to MAINTENANCE MOVING\n"); 
          time_at_zero = 0;
        }
        break;
      case MAINTENANCE_MOVING:
        if (actual_speed > 35.0)
        {
          service_mode = IN_SERVICE; 
          printf("Switching to IN SERVICE\n");
          pfp = open_next_file(dirname);
          write_past_data(pfp, record_data, record_pos, RECORD_SIZE);
          time_below_speed = 0;
        }
        else if (time_at_zero > MAX_TIME_AT_ZERO)
        {
          printf("Switching BACK to MAINTENANCE STOPPED\n"); 
          service_mode = MAINTENANCE_STOPPED;
        }
        else if (actual_speed ==  0.0)
        {
          time_at_zero++;
        }
        break;
      case IN_SERVICE:
        if (actual_speed <  MAX_MAINTENANCE_SPEED)
        {
          time_below_speed++;
        }
        else
        {
          time_below_speed = 0;
        }

        if (actual_speed ==  0.0)
        {
          time_at_zero++;
        }
        else
        {
          time_at_zero = 0;
        }

        if (time_below_speed > MAX_TIME_AT_MAINTENANCE)
        {
          printf("Switching BACK to MAINTENANCE MOVING - closing log file\n"); 
          fclose(pfp);
          service_mode = MAINTENANCE_MOVING;
        }
        break;
    } 
    record_pos = (record_pos + 1) % RECORD_SIZE;
  }
}

/******************************************************************/

