/***************************************************************************
Copyright (c) 2002,2003,2004 Donald Wayne Carr
                                                                                                                             
Permission is hereby granted, free of charge, to any person obtaining a
copy of this software and associated documentation files (the "Software"),
to deal in the Software without restriction, including without limitation
the rights to use, copy, modify, merge, publish, distribute, sublicense,
and/or sell copies of the Software, and to permit persons to whom the
Software is furnished to do so, subject to the following conditions:
                                                                                                                             
The above copyright notice and this permission notice shall be included
in all copies or substantial portions of the Software.
                                                                                                                             
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
DEALINGS IN THE SOFTWARE.
*************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <errno.h>
#include <math.h>

#include <curses.h>

#include "rtcommon.h"
#include "utimer.h"

#include "onboard.h"
#include "spd_algo.h"
#include "spd_algo_DC.h"
#include "spd_algo_RD.h"
#include "spd_algo_VV.h"

#include "spd_display.h"
#include "spd_comm.h"

#include "profile_reader.h"
#include "sim_reader.h"
                                                                                
#define DIF_LEN (100)
#define DRIVER_DELAY (10)
#define RT_FACTOR (100)

#include "arg.h"

static int n_sections;
static int the_line = 1;
static bool all_profiles = false;

/*******************************************************************/

void dispatch_algorithms(time_t now, int command, double speed, double distance)
{

}

/**********************************************************************/

int main(int argc, char *argv[])
{
  FILE *pfp = NULL;
  bool sim_mode;
  char base_name[100];
  int start = 0;

  //print_profile();
  //react_trace.set_level(5);
  speed_algorithm_DC_t spd_DC;
  speed_algorithm_RD_t spd_RD;
  speed_algorithm_VV_t spd_VV;

  sim_mode = false;
  bool pick_mode = false;
  bool free_running_mode = false;
  int current_arg;
  bool create_profiles = false;
  sim_reader_t sreader;
  sreader.set_line(1);
  profile_reader_t preader;
  preader.set_all(false);
  preader.set_line(1);
  for (current_arg=1; current_arg < argc; current_arg++)
  {
    if (0 == strcasecmp(argv[current_arg], "-s"))
    {
      printf("Setting simulation mode . . \n");
      sim_mode = true;
    } 
    else if (0 == strcasecmp(argv[current_arg], "-p"))
    {
      printf("Setting pick mode . . \n");
      pick_mode = true;
    } 
    else if (0 == strcasecmp(argv[current_arg], "-l"))
    {
      printf("Setting free running mode (libre) . . \n");
      free_running_mode = true;
    } 
    else if (0 == strcasecmp(argv[current_arg], "-L1"))
    {
      printf("Setting to line 1 . . \n");
      preader.set_line(1);
      sreader.set_line(1);
      the_line = 1;
    } 
    else if (0 == strcasecmp(argv[current_arg], "-L2"))
    {
      printf("Setting to line 2 . . \n");
      preader.set_line(2);
      sreader.set_line(2);
      the_line = 2;
    } 
    else if (0 == strcasecmp(argv[current_arg], "-f"))
    {
      if (argc > (current_arg + 1))
      {
        current_arg++;
        start = atol(argv[current_arg]);
        printf("Setting to first section to %d \n", start);
      }
      else
      {
        printf("Can't set first section, not enough args\n");
      }
    }
    else if (0 == strcasecmp(argv[current_arg], "-cp"))
    {
      create_profiles = true;
      printf("Create profiles ON\n");
    }
    else if (0 == strcasecmp(argv[current_arg], "-a"))
    {
      all_profiles = true;
      preader.set_all(true);
      printf("All profiles ON\n");
    }
  }

  //spd_init_screen();
  /**
  if (pick_mode)
  {
    printf("Picking line . . .\n");
    char *line_list[2] = {"Linea 1", "Linea 2"};
    int opt = select_from_list(2, line_list, "Selecionar la Linea");
    the_line = opt + 1;
    preader.set_line(opt+1);
    sreader.set_line(opt+1);
    if (the_line == 1)
    {
      int ns = sizeof(sim_names1) / sizeof(sim_names1[0]);
      int opt = select_from_list(ns, sim_names1, "Selecionar la estacion");
      start = opt;
    }
    else
    {
      int ns = sizeof(sim_names2) / sizeof(sim_names2[0]);
      int opt = select_from_list(ns, sim_names2, "Selecionar la estacion");
      start = opt;
    }
  }
  */


//  int n_seg = read_profile(vel_profile, 
 //          sizeof(vel_profile) / sizeof(vel_profile[0]));

  preader.read_profiles();
  int n_seg = preader.get_n_sections();

  if (!sim_mode)
  {
    init_io();
  }
  else
  {
    sreader.read_sim_data();
  }

  spd_init_screen();

  bool done = false;
  bool auto_mode = false;
  bool fast = false;
  bool continue_mode = false;

  utimer_t utimer;
  utimer.set_busy_wait(false);
  utimer.set_interval(1000000);

  //FILE *fp = fopen("out.txt", "w");
  char dirname[100];
  dirname[0] = '\0';
  for (int j=start; true; j = (j+1) % n_seg)
  {
    int next = (j + 1) % n_seg;
    int dif_index = 0; 
    int type = 1;
    bool warn = false;
    //int nseg = profile[j].n;

    double total_distance;
    double total_time;

    total_distance = preader.get_total_distance(j); //get_total_dist(j);
    total_time = preader.get_total_time(j);

    //mvprintw(20,2,"Cargando y Descargando Pasajeros"); 
    //refresh();

    if (!continue_mode)
    {
      if (auto_mode)
      {
        spd_show_loading(fast?2:4);
      }
      else
      {
        spd_wait_key("Pres. tecla para salir"); 
      }
      //mvprintw(20,2,"%-36s", " "); 
    }
    continue_mode = false;
    //spd_init_segment(vel_profile[j].st1, vel_profile[next].st1); 
    spd_init_segment(preader.get_station_name(j), preader.get_station_name(next)); 
    //beep();
    //mvprintw(18,2,"%d: total dist = %lf", j, total_distance); 
    //refresh();
    double distance = 0.0;
    int time_in_section = 0;
    if (!sim_mode) reset_distance(j);

    /***/
    if (create_profiles)
    {

      char fname[100];
    
      if ((j == 0) || (dirname[0] == '\0'))
      {
      	if (!dir_exists("profiles"))
      		system("mkdir profiles");
        for (int k=1; true; k++)
        {
          snprintf(dirname, sizeof(dirname), "profiles/profile%d", k);
          if (!dir_exists(dirname)) 
          {
            char cmd[100];
            snprintf(cmd, sizeof(cmd), "mkdir %s", dirname);
            system(cmd);
            snprintf(cmd, sizeof(cmd), "date >%s/date.txt", dirname);
            system(cmd);
            mvprintw(25,2,"Using: %s", dirname);
            break;
          }
        }
      }

      snprintf(fname, sizeof(fname), "%s/p%02d.txt", dirname, j);
      snprintf(base_name, sizeof(base_name), "%s/p%02d", dirname, j);
      mvprintw(26,2,"File: %s %s", fname, base_name);
      pfp = fopen(fname, "w");

    }
    else
    {
      pfp = NULL;
    }


    utimer.set_start_time();
    for (int i=0; true; i++)
    {
      if (!free_running_mode)
      {
        utimer.wait_next();
        if (utimer.late_time() > 1.0)
        {
          utimer.set_start_time();
        }
      }
      double now = (double) i / 1.0;
     
      double actual = 0;
      double desired = 0;

      if (sim_mode)
      {
        //mvprintw(22,2,"Sim Mode"); 
        sreader.get_sim_speed_dist(j, time_in_section, &distance, &actual);
      } 
      else
      {
        //mvprintw(22,2,"Live Mode"); 
        get_actual_speed_dist(j, time_in_section, &distance, &actual);
      } 
      
      //dispatch_algorithms(time_t now, int command, double speed, double distance)

      struct current_speed_limits_t limits;
      //calc_desired(j, actual, distance, &limits, &type, &warn);
      preader.calc_desired(j,  actual, distance, &limits, &type, &warn);
 
      desired = limits.desired;

      speed_results_t results;
      spd_DC.evaluate((time_t) now, actual, distance, limits, &results);  
      spd_RD.evaluate((time_t) now, actual, distance, limits, &results);  
      spd_VV.evaluate((time_t) now, actual, distance, limits, &results);  

      time_in_section++;


      if (pfp != NULL)
      {
        fprintf(pfp, "%lf %lf %lf %lf %lf %lf\n", 
          distance, actual, desired, limits.low, limits.high, limits.very_high);
      }

      //mvprintw(21,2,"Distance: %5.1lf, Speed: %4.0lf", distance, actual); 

      spd_print_current(desired, actual, type, warn, 
        100.0 * distance / total_distance, total_distance,
           now, total_time);

      /*****
      double left = total_distance - distance;
      if (left < 1.0) left = 0.0;
      mvprintw(12,2,"Tiempo: %5.1lf, Distancia: %4.0lf", now, distance); 
      if (COLS < 70) mvprintw(13,2, "");
      else printw(", ");
      printw("Falta: %4.0lf, Porciento: %3.0lf%%", left, (distance / total_distance) * 100.0);
      refresh();
      ****/
      if (((distance / total_distance) > 0.99) && (actual == 0))
      {
        break;
      }
      int ch = spd_getch();
      if (ch == 'f') {fast = true; utimer.set_interval(200000);}
      else if (ch == 's') {fast = false; utimer.set_interval(1000000);}
      else if (ch == 'c') {continue_mode = false; break;} 
      else if (ch == 'q') {done = true; break;} 
      else if (ch == 'a') {auto_mode = true; spd_print_auto(true);} 
      else if (ch == 'm') {auto_mode = false; spd_print_auto(false);} 
      //if (!fast) usleep(1000000);
      //else usleep(200000);
      dif_index++;
    }

    if (pfp != NULL)
    {
      fclose(pfp);
      pfp = NULL;
      char gtitle[50];
      snprintf(gtitle, sizeof(gtitle), "%s-%s", 
           preader.get_station_name(j), preader.get_station_name(next));
           //vel_profile[j].st1, vel_profile[next].st1);   
      mvprintw(23,2,"graph: %s %s", base_name, gtitle);
      spd_create_image(base_name, gtitle, false);
      spd_create_image(base_name, gtitle, true);
    }
    else
    {
      mvprintw(23,2,"graph: pfp is NULL");
    }

    if (done) break;
  }
  sleep(1);
  spd_wait_key("Hit any key to exit"); 
  spd_endwin();
}

/***********************************************************************************************************/
