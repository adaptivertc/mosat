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

#include <errno.h>
#include <math.h>

#include "rtcommon.h"
#include "utimer.h"
#include "spd_comm.h"
#include "ob_config.h"

#include "odo_mode.h"

onboard_config_t *onboard_config = NULL;

/*********************************************************************/

int main(int argc, char *argv[])
{
  const char *config_file = "onboard_config.txt";

  onboard_config = new onboard_config_t();
  onboard_config->read_file(config_file);

  spd_discrete_t discretes;
  double speed;
  double dist;

  utimer_t utimer;
  utimer.set_busy_wait(false);
  utimer.set_interval(1000000);

  const char *fname = "sample_out.txt";
  FILE *fp = fopen(fname, "w");
  if (fp == NULL)
  {
    perror(fname);
    exit(0);
  }

  init_io();
  reset_distance(0);
  odo_init_screen();

  utimer.set_start_time();
  int sample_n = 1;
  for (int i=0; i < 30; i++)
  {
    get_actual_speed_dist(0, 0, &dist, &speed, &discretes);
    //odo_print_current(double(i*10), double(i*9), i+22872); 
    odo_print_current(speed, dist, discretes.total_count, discretes.current_count); 
    #ifdef USE_PRINTF
    printf("\n\n\n");
    printf("Speed: %12.1lf\n", speed);
    printf("Dist.: %12.1lf\n", dist);
    printf("Count: %10ld\n", discretes.total_count);
    printf("Count: %10ld\n", discretes.current_count);
    #endif
    int key = odo_getch();
    if (key == 's')
    {
      fprintf(fp, "%d\t%lf\t%ld\t%ld\n", 
          sample_n, dist, discretes.total_count, discretes.current_count);
      #ifdef USE_PRINTF
      printf("SAVED: %d\t%lf\t%ld\t%ld\n", 
          sample_n, dist, discretes.total_count, discretes.current_count);
      #else
      #endif
      
      sample_n++;
    }
    utimer.wait_next();
  }
  odo_endwin();
}


