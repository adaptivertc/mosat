/************************************************************************
This is software to monitor and/or control trains.
Copyright (C) 2005,2006 Donald Wayne Carr 

This program is free software; you can redistribute it and/or modify 
it under the terms of the GNU General Public License as published by 
the Free Software Foundation; either version 2 of the License, or 
(at your option) any later version.

This program is distributed in the hope that it will be useful, 
but WITHOUT ANY WARRANTY; without even the implied warranty of 
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU 
General Public License for more details.

You should have received a copy of the GNU General Public License along 
with this program; if not, write to the Free Software Foundation, Inc., 
59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
***********************************************************************/

#define _XOPEN_SOURCE /* glibc2 needs this */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#include "rtcommon.h"
#include "arg.h"
#include "train_sim.h"
#include "msg_queue.h"
#include "tcontrol.h"
#include "event_alg.h"

#include "tpconfig.h"

const double driver_factors[10] = {1.02, 0.99, 0.98, 1.005, 1.02, 1.02, 0.99, 1.01, 0.98, 0.995};
static int factor_loc = 0;


/********************************************************/

void train_sim_t::next_day(void)
{
  for (int i=0; i < n_times; i++)
  {
    this->times[i] += 24 * 60 * 60;
  }
  next_entry = 0;
}

/********************************************************/

train_sim_t::train_sim_t(sim_ev_notify_t *nobj)
{
  n_trains = 0;
  n_sections = 0;
  n_times = 0;
  next_entry = 0;
  notify_obj = nobj;
}

/********************************************************/

void train_sim_t::read_sections(const char *fname)
{
  int max = 50;
  char line[300];

  FILE *fp = fopen(fname, "r");
  if (fp == NULL)
  {
    perror(fname);
    exit(0);
  }

  int n_lines = 0;
  for (int i=0; NULL != fgets(line, sizeof(line), fp); i++)
  {
    char tmp[300];
    int argc;
    char *argv[25];

    safe_strcpy(tmp, line, sizeof(tmp));
    argc = get_delim_args(tmp, argv, '\t', 4);
    if (argc != 4)
    {
      continue;
    }
    sections[n_lines].section_time = atol(argv[1]);
    sections[n_lines].departure_sensor_loc = atol(argv[2]);
    sections[n_lines].arival_sensor_loc = atol(argv[3]);
    printf("%d: %d %d %d\n", n_lines,
      sections[n_lines].section_time,
      sections[n_lines].departure_sensor_loc,
      sections[n_lines].arival_sensor_loc );
    n_lines++;
    if (n_lines >= max)
    {
      break;
    }
  }
  n_sections = n_lines;
}

/********************************************************/

time_t train_sim_t::read_day(void)
{
  int max = 500;
  time_t today = time(NULL);
  char line[300];
  
  const char *fname = tpconfig.get_config("TIMETABLE_FILE");
  if (fname == NULL)
  {
    fname = "timetable.txt";
  } 
  struct tm mytm;
  localtime_r(&today, &mytm);
  week_day = mytm.tm_wday;

  FILE *fp = fopen(fname, "r");
  if (fp == NULL)
  {
    perror(fname);
    exit(0);
  }

  int n_lines = 0;
  for (int i=0; NULL != fgets(line, sizeof(line), fp); i++)
  {
    char tmp[300];
    int argc;
    char *argv[25];

    safe_strcpy(tmp, line, sizeof(tmp));
    argc = get_delim_args(tmp, argv, '\t', 2);
    if (argc == 0)
    {
      continue;
    }

    localtime_r(&today, &mytm);

    if (NULL == strptime(argv[1], "%T", &mytm))
    {
      perror(argv[1]);
      exit(0);
    }

    printf("---%d: %s, %s---\n", n_lines, argv[0], argv[1]);
    times[n_lines] = mktime(&mytm);
    safe_strcpy(train_id[n_lines], argv[0], sizeof(train_id[n_lines])); 
    n_lines++;
    if (n_lines >= max)
    {
      printf("Error: Maximum reached\n");
      break;
    }
  }
  next_entry = 0;
  next_entry_time = times[0];
  n_times = n_lines;
  return times[0];
}

/********************************************************/

void train_sim_t::update(time_t now)
{
  if (next_entry < n_times)
  {
    if (now >= next_entry_time)
    {
      this->add_train(now);
      next_entry++;
      if (next_entry >= n_times)
      {
        next_day();
      }
      if (next_entry < n_times)
      {
        double d = ((double) random() / (double) RAND_MAX) * 12.0;
        int secs_dif = int(d) - 6; // This give a random value +/- 6 seconds
        next_entry_time = times[next_entry] + secs_dif; 
        printf("====== Random dif = %d\n", secs_dif);
      }
    }
  }
  else
  {
    // Now check to see if we are after midnight
    // and read in the new day if needed.
    struct tm mytm;
    localtime_r(&now, &mytm);
    if ( mytm.tm_wday != week_day)
    {
      this->read_day();
    }
  }

  for (int i=0; i < n_trains; i++)
  {
    this->update_train(i, now);
  }
}

/********************************************************/

void train_sim_t::add_train(time_t now)
{
  for (int i=(n_trains - 1); i >= 0; i--)
  {
    trains[i+1] = trains[i];
  }
  trains[0].driver_factor = driver_factors[factor_loc];
  factor_loc = (factor_loc + 1) % 10;
  trains[0].section = 0;
  trains[0].next_departure = times[next_entry] + sections[0].departure_sensor_loc;
  trains[0].next_arival = times[next_entry] +
   int (trains[0].driver_factor * double(sections[0].arival_sensor_loc));
  trains[0].departure_triggered = false;
  char buf[30];
  struct tm mytm;
  localtime_r(&now, &mytm);
  strftime(buf, sizeof(buf), "%T", &mytm);
  printf("+++++++++++ train entered service (%s) %d\n", buf, n_trains);
  n_trains++;

/**
  double driver_factor; // factor for this driver in calculating times
  int section; // section where the train is
  time_t next_departure; // time that the next event is triggered
  time_t next_arival; // time that the next event is triggered

  int section_time;
  int departure_sensor_loc;
**/
}

/********************************************************/

void train_sim_t::update_train(int n, time_t now)
{
  if (trains[n].departure_triggered)
  {
    if (now >= trains[n].next_arival)
    {
      trains[n].section++;
      notify_obj->trigger_arrival(trains[n].section, now);
      if (trains[n].section >= n_sections)
      {
        // ok, the train arrived back at the starting station, out of service
        char buf[30];
        struct tm mytm;
        localtime_r(&now, &mytm);
        strftime(buf, sizeof(buf), "%T", &mytm);
        printf("******* train left service (%s) %d\n", buf, n_trains);
        n_trains--;
      }
      else
      {
        int s = trains[n].section;
        int diff = sections[s].section_time - sections[s].arival_sensor_loc;
        trains[n].departure_triggered = false;
        trains[n].next_departure = now + sections[s].departure_sensor_loc + diff + RT_DWELL_TIME;
        double total_to_sensor =
         double(sections[s].arival_sensor_loc + diff + RT_DWELL_TIME);
        total_to_sensor *= trains[n].driver_factor;
        trains[n].next_arival = now + int(total_to_sensor);
/**
           (trains[n].driver_factor *
         (sections[s].arival_sensor_loc + diff + RT_DWELL_TIME));
        trains[n].driver_factor;
***/
      }
    }
  }
  else
  {
    if (now >= trains[n].next_departure)
    {
      trains[n].departure_triggered = true;
      notify_obj->trigger_departure(trains[n].section, now);
    }
  }
}

/********************************************************/

