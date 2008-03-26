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

#include "spd_algo.h"
#include "spd_algo_DC.h"
#include "spd_algo_RD.h"
#include "spd_algo_VV.h"

#include "spd_display.h"
#include "spd_comm.h"


                                                                                
#define DIF_LEN (100)
#define DRIVER_DELAY (10)
#define RT_FACTOR (100)
#define MAXS (100)
//#define DISTANCE

#include "arg.h"

char *sim_names2[] = 
{
  "Tetlan-Aurora",
  "Aurora-SanJacinto",
  "SanJacinto-SanAndres",
  "SanAndres-Cristobal",
  "Cristobal-Oblatos",
  "Oblatos-Belisario",
  "Belisario-SnJuan",
  "SnJuan-PUniver",
  "PUniver-Juarez",
  "Juarez-AreaManiobras",
  "AreaManiobras-Juarez",
  "Juarez-PUniver",
  "PUniver-SnJuan",
  "SnJuan-Belisario",
  "Belisario-Oblatos",
  "Oblatos-Cristobal",
  "Cristobal-SanAndres",
  "SanAndres-SanJacinto",
  "SanJacinto-Aurora",
  "Aurora-Tetlan",
  "Tetlan-AreaManiobras"
};

char *sim_names1[] = 
{
  "PerifericoSur-Tesoro",
  "Tesoro-Espana",
  "Espana-PatriaSur",
  "PatriaSur-IslaRaza",
  "IslaRaza-18deMarzo",
  "18deMarzo-Urdaneta",
  "Urdaneta-UnidadDeportivo",
  "UnidadDeportivo-SantaFilomena",
  "SantaFilomena-Washington",
  "Washington-Mexicaltzingo",
  "Mexicaltzingo-Juarez",
  "Juarez-ElRefugio",
  "ElRefugio-Mezquitan",
  "Mezquitan-AvilaComacho",
  "AvilaComacho-DivisionNorte",
  "DivisionNorte-Atemajac",
  "Atemajac-Dermatologico",
  "Dermatologico-PerifericoNorte",
  "PerifericoNorte-Dermatologico",
  "Dermatologico-Atemajac",
  "Atemajac-DivisionNorte",
  "DivisionNorte-AvilaComacho",
  "AvilaComacho-Mezquitan",
  "Mezquitan-ElRefugio",
  "ElRefugio-Juarez",
  "Juarez-Mexicaltzingo",
  "Mexicaltzingo-Washington",
  "Washington-SantaFilomena",
  "SantaFilomena-UnidadDeportivo",
  "UnidadDeportivo-Urdaneta",
  "Urdaneta-18deMarzo",
  "18deMarzo-IslaRaza",
  "IslaRazaPatriaSur",
  "PatriaSur-Espana",
  "Espana-Tesoro",
  "Tesoro-PerifericoSur"
};

struct spd_sim_data_t
{
  int n;
  double speed[500];
  double dist[500];
};

/*****
struct current_speed_limits_t
{
  double desired;
  double low; 
  double high;
  double very_high;
};
***/
typedef struct sdef_t
{
  int n;
  char st1[13];
  double *dist;
  double *speed;
  double *low;
  double *high;
  double *very_high;
  double total_dist;
  double total_time;
};


static sdef_t vel_profile[60];
static int n_sections;
static spd_sim_data_t sim_data[MAXS];
static int the_line = 1;
static bool all_profiles = false;

/*******************************************************************/

double get_total_dist(int section)
{
  int i = sim_data[section].n - 1;
  return sim_data[section].dist[i] - sim_data[section].dist[0];
}

/*********************************************************************/

void get_sim_speed_dist(int section, int t, double *dist, double *speed)
{
  if (t >= sim_data[section].n)
  {
    *dist = sim_data[section].dist[sim_data[section].n-1] - sim_data[section].dist[0];
    *speed = 0.0;
    return;
  }
  *dist = sim_data[section].dist[t] - sim_data[section].dist[0];
  *speed = sim_data[section].speed[t];
}

/*********************************************************************/

void read_sim_data(void)
{
  if (the_line == 1)
  {
    n_sections = sizeof(sim_names1) / sizeof(sim_names1[0]); 
  }
  else
  {
    n_sections = sizeof(sim_names2) / sizeof(sim_names2[0]); 
  }
  delim_file_t df(300, 5, '\t', '#');
  for (int i=0; i < n_sections; i++)
  {
    char **argv;
    int argc, line_num;
    char fname[200];
    if (the_line == 1)
    {
      snprintf(fname, sizeof(fname), "vfiles_line1/%s.txt", sim_names1[i]);
    }
    else
    {
      snprintf(fname, sizeof(fname), "vfiles_line2/%s.txt", sim_names2[i]);
    }
    printf("Reading: %s\n", fname);

    argv = df.first(fname, &argc, &line_num);
    sim_data[i].n = 0; 
    for (int j=0; argv != NULL; j++)
    {
      if (argc == 2)
      {
        sim_data[i].dist[j] = atof(argv[0]); 
        sim_data[i].speed[j] = atof(argv[1]); 
        sim_data[i].n++; 
        //printf("'%lf' '%lf'\n", sim_data[i].dist[j], sim_data[i].speed[j]); 
      }
      else
      {
        printf("Wrong number of args: %d, line %d\n", argc, line_num);
      }
      argv = df.next(&argc, &line_num);
    }
  }
}

/*********************************************************************/


                                                                                
int read_profile(sdef_t the_profile[], int max)
{
  int argc, line_num;
    argc = 7;
    if (argc < 3)
    {
      printf("xWrong number of args: %d\n", argc);
      exit(0);
    }
  delim_file_t df(300, 20, '|', '#');
    if (argc < 3)
    {
      printf("xxWrong number of args: %d\n", argc);
      exit(0);
    }
  char **argv;
  int n_segments = 0;
  if (all_profiles)
  {
    if (the_line == 1)
    {
      argv = df.first("line1_profile_all.txt", &argc, &line_num);
    }
    else
    {
      argv = df.first("line2_profile_all.txt", &argc, &line_num);
    }
  }
  else
  {
    if (the_line == 1)
    {
      argv = df.first("line1_profile.txt", &argc, &line_num);
    }
    else
    {
      argv = df.first("line2_profile.txt", &argc, &line_num);
    }
  }
  if (argc != 2)
  {
    printf("xxxWrong number of args: %d\n", argc);
    exit(0);
  }
  for (int i=0; (argv != NULL) && (i < max); i++)
  {
    if (argc != 2)
    {
      printf("Wrong number of args (first line): %d\n", argc);
      exit(0);
    }
    //printf("%s: %s\n", argv[0], argv[1]);
    safe_strcpy(the_profile[i].st1, argv[0], sizeof(the_profile[i].st1));
    the_profile[i].total_dist = atof(argv[1]);
    argv = df.next(&argc, &line_num);
    //printf("argc = %d, %s, %d\n", argc, __FILE__, __LINE__);
    if (argc < 3)
    {
      printf("Wrong number of args: %d\n", argc);
      exit(0);
    }

    if (argv == NULL)
    {
      printf("No more args after %s\n", argv[0]);
      exit(0);
    }


    if (argc < 3)
    {
      printf("Wrong number of args: %d\n", argc);
      exit(0);
    }
    //printf("argc = %d, %s, %d\n", argc, __FILE__, __LINE__);

    if (argc < 3)
    {
      printf("Wrong number of args: %d\n", argc);
      exit(0);
    }

    the_profile[i].n = argc;
    the_profile[i].dist = new double[argc];
    the_profile[i].speed = new double[argc];
    the_profile[i].low = new double[argc];
    the_profile[i].high = new double[argc];
    the_profile[i].very_high = new double[argc];
    for (int j=0; j < argc; j++)
    {
      the_profile[i].dist[j] = atof(argv[j]);
    } 
    argv = df.next(&argc, &line_num);
    if ((argv == NULL) || (argc != the_profile[i].n))
    {
      printf("Wrong number of args: %d, expected %d\n", argc, the_profile[i].n);
      exit(0);
    }
    for (int j=0; j < argc; j++)
    {
      the_profile[i].speed[j] = atof(argv[j]);
    } 

    if (all_profiles)
    {
      argv = df.next(&argc, &line_num);
      if ((argv == NULL) || (argc != the_profile[i].n))
      {
        printf("Wrong number of args: %d, expected %d\n", argc, the_profile[i].n);
        exit(0);
      }
      for (int j=0; j < argc; j++)
      {
        the_profile[i].low[j] = atof(argv[j]);
      } 
      
      argv = df.next(&argc, &line_num);
      if ((argv == NULL) || (argc != the_profile[i].n))
      {
        printf("Wrong number of args: %d, expected %d\n", argc, the_profile[i].n);
        exit(0);
      }
      for (int j=0; j < argc; j++)
      {
        the_profile[i].high[j] = atof(argv[j]);
      } 
      
      argv = df.next(&argc, &line_num);
      if ((argv == NULL) || (argc != the_profile[i].n))
      {
        printf("Wrong number of args: %d, expected %d\n", argc, the_profile[i].n);
        exit(0);
      }
      for (int j=0; j < argc; j++)
      {
        the_profile[i].very_high[j] = atof(argv[j]);
      } 
      
    }

    argv = df.next(&argc, &line_num);
    n_segments++;
  }
  /****
typedef struct sdef_t
{
  int n;
  char st1[13];
  double *dist;
  double *speed;
  double total_dist;
  double total_time;
};
******/
  double mydiff[4] = {-6, 0, 5, 8};
  FILE *fp = fopen("junk.txt", "w");
  for (int i=0; i < n_segments; i++)
  {
    fprintf(fp, "%s|%0.1lf|\n", the_profile[i].st1, the_profile[i].total_dist);
    for (int j=0; j < the_profile[i].n; j++)
    {
      fprintf(fp, "%0.1lf|", the_profile[i].dist[j]);
    }
    fprintf(fp, "\n");
    for (int cnt=0; cnt < 4; cnt++)
    {
      for (int j=0; j < the_profile[i].n; j++)
      {
        fprintf(fp, "%0.1lf|", the_profile[i].speed[j] + mydiff[cnt]);
      }
      fprintf(fp, "\n");
    }
  }
  return n_segments;
}

/********************************************************************/

void print_profile(int n)
{
  FILE *fp;
  fp = fopen("line1_profile.txt", "w");
  for (int i=0; i < n; i++)
  {
    fprintf(fp, "%s|%0.0lf|\n", vel_profile[i].st1, vel_profile[i].total_dist);
    for (int j=0; j < vel_profile[i].n; j++)
    {
      fprintf(fp, "%0.1lf|", vel_profile[i].dist[j]);
    }
    fprintf(fp, "\n");
    for (int j=0; j < vel_profile[i].n; j++)
    {
      fprintf(fp, "%0.1lf|", vel_profile[i].speed[j]);
    }
    fprintf(fp, "\n");
  }
}

/***********************************************************************************************************/

double calc_speed(double t, double t1, double s1, double t2, double s2)
{
  // Here, we do a simple interpolation between two points.
  double conversion_factor, raw_span;
  raw_span = t2 - t1;
  if (raw_span == 0)
  {
    conversion_factor = 0.0;
  }
  else
  {
    conversion_factor = (s2 - s1) / (t2 - t1);
  }
  return ((t - t1) * conversion_factor) + s1;
}

/***********************************************************************************************************/

static int last_section = -1;
static int my_index = 0;
static int last_type = 1;
static int type = 1;


void calc_desired(int a_section, double a_speed, double a_distance, 
      current_speed_limits_t *limits, int *mode, bool *warn)
{
  if (a_section != last_section)
  {
    my_index = 0;
    last_section = a_section;
    spd_beep();
  }

  //mvprintw(16,2,"ind: %2d, n: %2d, %6.0lf %6.0lf", my_index + 2, profile[a_section].n,
   //                a_distance, profile[a_section].dist[my_index+1]);

  if (a_distance > vel_profile[a_section].dist[my_index+1])
  {
    if ((my_index + 2) < vel_profile[a_section].n)
    {
      my_index++;
      spd_beep();
    }
  } 
  *warn = false;
  double distance_to_next = vel_profile[a_section].dist[my_index+1] - a_distance; 
  double distance_in_5_sec = a_speed * (1.0/3.6) * 5.5;
  if (distance_to_next < distance_in_5_sec) 
  {
    *warn = true;
  }

  if (vel_profile[a_section].speed[my_index+1] < vel_profile[a_section].speed[my_index])
  {
    type = 0;
  }
  else if (vel_profile[a_section].speed[my_index+1] > vel_profile[a_section].speed[my_index])
  {
    type = 2;
  }
  else
  {
    type = 1;
  }

  if (type != 1) 
  {
    *warn = false;
  }

  //mvprintw(15,2,"Section: %2d, index: %2d, spd1: %5.0lf, spd2: %5.1lf, d1: %8.1lf, d2: %8.1lf, n: %d", a_section, my_index, 
   //        profile[a_section].speed[my_index], profile[a_section].speed[my_index + 1],
    //       profile[a_section].dist[my_index], profile[a_section].dist[my_index + 1],
     //                                 profile[a_section].n);
  double spd = calc_speed(a_distance, vel_profile[a_section].dist[my_index],
              vel_profile[a_section].speed[my_index], 
              vel_profile[a_section].dist[my_index+1], 
              vel_profile[a_section].speed[my_index+1]);
  if (spd < 0.0) spd = 0.0;
  double low = calc_speed(a_distance, vel_profile[a_section].dist[my_index],
              vel_profile[a_section].low[my_index], 
              vel_profile[a_section].dist[my_index+1], 
              vel_profile[a_section].low[my_index+1]);
  if (low < -1.0) low = -1.0;
  double high = calc_speed(a_distance, vel_profile[a_section].dist[my_index],
              vel_profile[a_section].high[my_index], 
              vel_profile[a_section].dist[my_index+1], 
              vel_profile[a_section].high[my_index+1]);
  if (high < 5.0) high = 5.0;
  double very_high = calc_speed(a_distance, vel_profile[a_section].dist[my_index],
              vel_profile[a_section].very_high[my_index], 
              vel_profile[a_section].dist[my_index+1], 
              vel_profile[a_section].very_high[my_index+1]);
  if (very_high < 8.0) very_high = 8.0;
  mvprintw(16,2,"%4.1lf < %4.1lf > %4.1lf >> %4.1lf", low, spd, high, very_high); 
  limits->desired = spd;
  limits->low = low;
  limits->high = high;
  limits->very_high = very_high;
  //*speed = spd;
  *mode = type;
  last_type = type;
}

/***********************************************************************************************************/

double past_speed[DRIVER_DELAY];

/**********************************************************************/

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
  //
  speed_algorithm_DC_t spd_DC;
  speed_algorithm_RD_t spd_RD;
  speed_algorithm_VV_t spd_VV;

  sim_mode = false;
  bool pick_mode = false;
  int current_arg;
  bool create_profiles = false;
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
    else if (0 == strcasecmp(argv[current_arg], "-L1"))
    {
      printf("Setting to line 1 . . \n");
      the_line = 1;
    } 
    else if (0 == strcasecmp(argv[current_arg], "-L2"))
    {
      printf("Setting to line 2 . . \n");
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
      printf("All profiles ON\n");
    }
  }

  //spd_init_screen();
  if (pick_mode)
  {
    printf("Picking line . . .\n");
    char *line_list[2] = {"Linea 1", "Linea 2"};
    int opt = select_from_list(2, line_list, "Selecionar la Linea");
    the_line = opt + 1;
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

  int n_seg = read_profile(vel_profile, 
           sizeof(vel_profile) / sizeof(vel_profile[0]));

  if (!sim_mode)
  {
    init_io();
  }
  else
  {
    read_sim_data();
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

    total_distance = vel_profile[j].total_dist;//get_total_dist(j);
    total_time = vel_profile[j].total_time;

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
    spd_init_segment(vel_profile[j].st1, vel_profile[next].st1); 
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
      utimer.wait_next();
      if (utimer.late_time() > 1.0)
      {
        utimer.set_start_time();
      }
      double now = (double) i / 1.0;
     
      double actual = 0;
      double desired = 0;

      if (sim_mode)
      {
        //mvprintw(22,2,"Sim Mode"); 
        get_sim_speed_dist(j, time_in_section, &distance, &actual);
      } 
      else
      {
        //mvprintw(22,2,"Live Mode"); 
        get_actual_speed_dist(j, time_in_section, &distance, &actual);
      } 
      
      //dispatch_algorithms(time_t now, int command, double speed, double distance)

      struct current_speed_limits_t limits;
      calc_desired(j, actual, distance, &limits, &type, &warn);
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
           vel_profile[j].st1, vel_profile[next].st1);   
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
