
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#ifndef ARM
#include <ncurses.h>
#endif

#include <string.h>
#include <stdint.h>

#include <math.h>

#include "rtmodbus.h"

#include "spd_comm.h"

#include "ob_config.h"

/*********************************************************************/

//static mod_tcpip_client_t *mtp;
static rtmodbus_t *modc;
//static MODSerial *modc;
static double total_distance = 0.0;
//static const double circ = 0.694 * M_PI;
//static const double circ = 0.695 * M_PI;
//  Verificamos 12 de Mayo 2008 que es 0.665 en la motriz de pruebas
static double wheel_diameter = 0.665;
static double circ = wheel_diameter * M_PI; //  la motriz M038? 
// se reperfila cada 50 mil a 60 mil kms (de 5 a 6 meses).
static double meters_per_pulse = (circ / 110.0);
//static const double meters_per_pulse = 10.0;
static int count_channel = 8;
static int door_channel = 2;
static bool door_invert = false;
static const  int speed_channel = 0;

static long last_count;
static int count_history[3];
//static struct timeval time_history[3]; 
static int hindex;
static const int hsize = sizeof(count_history) / sizeof(count_history[0]);

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

void set_calculation_information(double the_diameter, int the_count_channel)
{
  wheel_diameter = the_diameter;
  circ = wheel_diameter * M_PI;
  meters_per_pulse = (circ / 110.0);
  count_channel = the_count_channel;
}

/*********************************************************************/

void connect_modbus(void)
{
  const char *ip_address = onboard_config->get_config("modbus_ip");
  if (ip_address == NULL)
  {
    ip_address = "172.16.115.99";
    printf("No ip address specified, using: %s\n", ip_address);
  }
  else
  {
    printf("ip_address specified: %s\n", ip_address);
  }
  const char *modbus_address = onboard_config->get_config("modbus_address");
  if (modbus_address == NULL)
  {
    modbus_address = "0";
    printf("No modbus address specified, using: %s\n", modbus_address);
  }
  else
  {
    printf("modbus address specfied: %s\n", modbus_address);
  }
  const char *diameter = onboard_config->get_config("diameter");
  if (diameter == NULL)
  {
    diameter = "0.665";
    printf("No diameter specified, using: %s\n", diameter);
  }
  else
  {
    printf("diameter specified: %s\n", diameter);
  }

  const char *cnt = onboard_config->get_config("count_channel");
  if (cnt == NULL)
  {
    cnt = "8";
    printf("No count channel specified, using: %s\n", cnt);
  }
  else
  {
    printf("Count channel specified: %s\n", cnt);
  }
  set_calculation_information(atof(diameter), atol(cnt));

  const char *door_ch = onboard_config->get_config("door_channel");
  if (door_ch == NULL)
  {
    door_ch = "2";
    printf("No door channel specified, using: %s\n", door_ch);
  }
  else
  {
    printf("Door channel specified: %s\n", door_ch);
  }
  door_channel = atol(door_ch);

  const char *door_inv = onboard_config->get_config("door_invert");
  if (door_ch == NULL)
  {
    door_invert = false;
    printf("No door invert specified, using positive logic");
  }
  else
  {
    door_invert = (door_inv[0] == '1') || (door_inv[0] == 't') || (door_inv[0] == 'T') || (door_inv[0] == 'Y') || (door_inv[0] == 'y');
    printf("Door inver specified: %s, %s logic\n", door_inv, door_invert ? "negative": "positive");
  }

  door_channel = atol(door_ch);
  modc = rt_create_modbus(ip_address);
  modc->set_debug_level(10);
  modc->set_address(atol(modbus_address));
  reset_distance(0);
}

/***********************************************************************/

void get_actual_speed_dist(int section, int t, double *dist, double *speed, spd_discrete_t *descretes)
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
  #ifndef ARM
  mvprintw(17,2,"%s %s", buf, dvals[door_channel] ? "OPEN" : "CLOSED");
  #endif
  descretes->doors_open = dvals[door_channel];
  if (door_invert)  descretes->doors_open = !descretes->doors_open;
  // When we have the inputs, we need to fix the following:
  descretes->left_open = false;
  descretes->right_open = false;
  descretes->master = false;
  

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
