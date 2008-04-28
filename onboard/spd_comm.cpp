
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ncurses.h>
#include <string.h>
#include <stdint.h>

#include <math.h>

#include "rtmodbus.h"

#include "spd_comm.h"

/*********************************************************************/

//static mod_tcpip_client_t *mtp;
static rtmodbus_t *modc;
//static MODSerial *modc;
static double total_distance = 0.0;
//static const double circ = 0.694 * M_PI;
//static const double circ = 0.695 * M_PI;
static const double circ = 0.665 * M_PI; //  la motriz M033 se reperfilo el 08/Nov/2005,
// se reperfila cada 50 mil a 60 mil kms (de 5 a 6 meses).
const double meters_per_pulse = (circ / 110.0);
//static const double meters_per_pulse = 10.0;
static const int count_channel = 8;
static const  int speed_channel = 0;

static long last_count;
static int count_history[3];
//static struct timeval time_history[3]; 
static int hindex;
static const int hsize = sizeof(count_history) / sizeof(count_history[0]);

//static FILE *fp = NULL;

//#define DEST_IP   "10.25.50.27"
//#define DEST_IP   "10.25.50.46"
//#define DEST_IP   "172.16.115.27"
//#define DEST_IP   "172.16.115.99"
#define DEST_IP   "10.1.0.5"
//#define DEST_IP   "127.0.0.1"
//#define DEST_IP   "216.109.112.135"

//#define DEST_PORT 3490
#define DEST_PORT 502
//#define DEST_PORT 80

double time_dif(struct timeval tv1, struct timeval atv)
{
  /*
   * This returns the difference in seconds between two
   * timeval structures.
   */

  long secdif = tv1.tv_sec - atv.tv_sec;
  long usecdif = tv1.tv_usec - atv.tv_usec;
  return (double) secdif + ((double) usecdif / 1000000);
}


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

void reset_distance(int section)
{
  total_distance = 0.0;
  uint16_t uvals[20];
  modc->read_ai(0, 16, uvals);
  last_count = uvals[count_channel];
  for (int i=0; i < hsize; i++) count_history[i] = last_count;
  hindex = 0;
}

/*********************************************************************/

void connect_modbus(void)
{

  modc = rt_create_modbus("172.16.115.99");
  modc->set_debug_level(10);
  modc->set_address(0);

  //fp = fopen("readings.txt", "w");
  reset_distance(0);
}

/***********************************************************************/

void get_actual_speed_dist(int section, int t, double *dist, double *speed)
{
  int current_count;
  int speed_count;
  uint16_t uvals[20];
  bool dvals[12];
  char buf[100];


  modc->read_di(0, 12, dvals);
  modc->read_ai(0, 16, uvals);

  current_count = uvals[count_channel];
  speed_count = uvals[speed_channel]; 
  for (int i=0; i < 12; i++)
  {
    buf[i] = dvals[i] ? '1' : '0';
    buf[i+1] = '\0';
  }
  mvprintw(17,2,"%s", buf);

  /***
  for (int i=0; i < 16; i++)
  {
    printf("%d ", (int) uvals[i]);
  }
  fprintf(fp, "%s", buf);
  ***/
  /**
  for (int i=0; i < nt; i++)
  {
    count[i] = uvals[ch[i]];
    //fprintf(fp, "\t%d", (int) count[i]);
    //td[i].update(uvals[ch[i]], dvals[dch[i]]);
  }
  **/
  double ai_speed = (((double) speed_count) / 6553.5) * 0.5; /* This gives you the value in volts */
  ai_speed *= 10.0; /* Now 1 volt = 10 km/h */
  total_distance += calc_dif(current_count, last_count) * meters_per_pulse;
  double fdistance = calc_dif(current_count, count_history[hindex]) * meters_per_pulse;
  double fspeed = ((fdistance / (double) hsize) * 3600.0) / 1000.0; 
  //fprintf(fp, "\t%lf\t%lf", total_distance, speed);
  //fprintf(fp, "\n");
  //fflush(fp);
  last_count = current_count;
  count_history[hindex] = current_count;
  hindex = (hindex + 1) % hsize;
  *dist = total_distance;
  *speed = fspeed; //ai_speed;
}

/*********************************************************************/

void init_io(void)
{
  connect_modbus();
}

/*********************************************************************/

void spd_set_key(int ch)
{
}

/*********************************************************************/
