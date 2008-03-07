
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>

//#include <stdio.h>
//#include <stdlib.h>
//#include <unistd.h>
#include <string.h>

//#include "rtcommon.h"

#include "utimer.h"

#include "frtimer.h"

enum service_mode_t
{
  MAINTENANCE_STOPPED,
  MAINTENANCE_MOVING,
  IN_SERVICE_MOVING,
  IN_SERVICE_AT_STATION 
};

static const int MAX_STATIONS = 30;
static const int N_LINES = 2;
static const int N_STATIONS[N_LINES] = {19, 10}; 
int n_seconds[N_LINES][MAX_STATIONS];

//Adeline 12:30 pm
//Ricardo 2:00 pm

/******************************************************************/

static unsigned char *page_start;

static unsigned char *xdio_0_start;
static unsigned char *xdio_0_r0;
static unsigned char *xdio_0_r1;
static unsigned char *xdio_0_r2;
static unsigned char *xdio_0_r3;

static unsigned char *xdio_1_start;
static unsigned char *xdio_1_r0;
static unsigned char *xdio_1_r1;
static unsigned char *xdio_1_r2;
static unsigned char *xdio_1_r3;

struct record_data_t
{
  time_t time;
  unsigned short count;
  double speed;
  double distance;
};

static int record_pos = 0;

static record_data_t record_data[60];

static unsigned long long  fr_lastcount;
static unsigned long long  fr_currentcount;

static const double circ = 0.691 * M_PI; //  la motriz M033 se reperfilo el 08/Nov/2005,
// se reperfila cada 50 mil a 60 mil kms (de 5 a 6 meses).
//const double meters_per_pulse = (circ / 110.0);
const double meters_per_pulse = (circ / 220.0); // this box counts edges!


#define TV_ELAPSED_US(x, y)     ((((x).tv_sec - (y).tv_sec) * 1000000) + \
        ((x).tv_usec - (y).tv_usec))

/*********************************************************************/

long calc_dif(long count, long last)
{
  if (last > count)
  {
    // Ok, we rolled over
    return count + 65536 - last;
  }
  else
  {
    return count - last;
  }
}


/*********************************************************************/

unsigned short tsarm_read_count(long *usecs)
{
  unsigned char byte3 = *xdio_0_r3;
  unsigned char byte2 = *xdio_0_r2;
  unsigned char byte3_2 = *xdio_0_r3;
  unsigned char byte2_2 = *xdio_0_r2;
  if (byte3 != byte3_2) // rollover use second readings 
  {
    byte3 = byte3_2;
    byte2 = byte2_2;
  }
  fr_currentcount = GetFRTimer();

  long elapsed_us =  frt_us_diff(fr_currentcount, fr_lastcount);

  unsigned short count16;
  count16 = ((unsigned short) byte3) << 8;
  count16 = count16 + ((unsigned short) byte2);

  fr_lastcount = fr_currentcount;
  if (usecs != NULL)
  {
    *usecs = elapsed_us;
  }
  return (count16);
}

/*********************************************************************/

void init_io(void)
{
  printf("Initializing tsarm to read high speed counter\n");
  int fd = open("/dev/mem", O_RDWR|O_SYNC);
  //xdio at 0x12c00000-0x12c00003
  page_start = (unsigned char *) mmap(0, getpagesize(), PROT_READ|PROT_WRITE, MAP_SHARED, fd,   0x72000000);
  //page_start = (unsigned char *) mmap(0, getpagesize(), PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0x12C00000);

  xdio_0_start = page_start + 0x40;
  xdio_1_start = page_start + 0x44;

  xdio_0_r0 = (unsigned char *) (xdio_0_start + 0);
  xdio_0_r1 = (unsigned char *) (xdio_0_start + 1);
  xdio_0_r2 = (unsigned char *) (xdio_0_start + 2);
  xdio_0_r3 = (unsigned char *) (xdio_0_start + 3);

  xdio_1_r0 = (unsigned char *) (xdio_1_start + 0);
  xdio_1_r1 = (unsigned char *) (xdio_1_start + 1);
  xdio_1_r2 = (unsigned char *) (xdio_1_start + 2);
  xdio_1_r3 = (unsigned char *) (xdio_1_start + 3);

  // Set xdio core #0 as a counter input
  *xdio_0_r0 = 0x41;
  *xdio_0_r1 = 0x00;

  // Set xdio core #1 as a PWM output
  *xdio_1_r0 = 0x00;
  *xdio_1_r1 = 0xff;
  *xdio_1_r0 = 0xC0;
  *xdio_1_r1 = 0x00;
  *xdio_1_r2 = 0x0F; // 0x0f = 15 + 2 = 17 ticks
  *xdio_1_r3 = 0x0F; // 0x0f = 15 + 2 = 17 ticks
  *xdio_1_r0 = 0xC3; // 32.768 khz clock.

  InitFRTimer();
  ResetFRTimer();
  fr_lastcount = GetFRTimer();
} 

/*********************************************************************/

int main(int argc, char *argv[]) 
{

/***/
  utimer_t utimer;
  utimer.set_busy_wait(false);
  utimer.set_interval(1000000);
  utimer.set_start_time();
/****/

  service_mode_t service_mode = MAINTENANCE_STOPPED;
  int line_number = -1;
  int section_number= -1;
  bool door_is_open = false;
  double speed_kmh = 0.0;
  double distance_km = 0.0;

  unsigned short count16;
  unsigned short last_count;
  init_io();
  last_count = tsarm_read_count(NULL);


  while (1)
  {
    utimer.wait_next();

    long usecs;

    count16 = tsarm_read_count(&usecs);

    long n_pulses = calc_dif((long) count16, (long) last_count);

    double frequency = ((double) n_pulses) / (((double) usecs) / 500000.0);
    
    distance_km += ((double) n_pulses) * meters_per_pulse;

    speed_kmh = frequency * meters_per_pulse * 3.6;

    printf("Speed: %5.1lf, Distance: %5.0lf\n", speed_kmh, distance_km);
    //printf("--- %02hhx %02hhx %02hhx %02hhx -  %hx %hu  %ld %ld %0.1lf\n", 
    //     byte0, byte1, byte2, byte3, count16, count16, n_pulses, elapsed_us, frequency);


    record_data[record_pos].time = time(NULL);
    record_data[record_pos].count = count16;
    record_data[record_pos].speed = speed_kmh;
    record_data[record_pos].distance = distance_km;

    switch(service_mode)
    {
      case MAINTENANCE_STOPPED:
        if (speed_kmh > 0.0)
        {
          service_mode = MAINTENANCE_MOVING;
        }
        break;
      case MAINTENANCE_MOVING:
        if (speed_kmh > 35.0)
        {
          service_mode = IN_SERVICE_MOVING; 
          section_number = 0;
          line_number = -1;
        }
        break;
      case IN_SERVICE_MOVING:
        if ((speed_kmh == 0.0) && (door_is_open))
        {
          service_mode = IN_SERVICE_AT_STATION;
          if (section_number == 0)
          {
             // Determine which line by the distance to the first station.
             
          }
          int last_station = 36; // should be calculated based on line!!
          if (section_number == last_station) // 
          {
            // close out files for rount trip here
            service_mode = MAINTENANCE_STOPPED;
          }
        }
        break;
      case IN_SERVICE_AT_STATION:
        if ((speed_kmh > 0.0) && (!door_is_open))
        {
          service_mode = IN_SERVICE_MOVING;
        }
        break;
    } 
    
    fr_lastcount = fr_currentcount;
    last_count = count16;
    //last_time = now;

  }
}

/******************************************************************/

