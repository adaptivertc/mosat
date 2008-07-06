/************************************************************************
This software is part of React, a control engine
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

/******
  ac point

*************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <malloc.h>
#include <string.h>

#include "rtcommon.h"
#include "reactpoint.h"
#include "db_point.h"
#include "db.h"
#include "arg.h"
#include "ap_config.h"

static FILE *ac_fp = NULL;
static char ac_log_home[200];

/********************************************************************/

static FILE *open_day_history_file(const char *home, const char *post, FILE *fp) {
  char fname[500];
  if (fp != NULL)
  {
    fclose(fp);
  }
  time_t now = time(NULL);
  char buf1[30];
  struct tm mytm;
  localtime_r(&now, &mytm);
  strftime(buf1, sizeof(buf1), "%Y%m%d", &mytm);
  snprintf(fname, sizeof(fname), "%s/%s%s", home, buf1, post);
  printf("Opening %s\n", fname);
  fp = fopen(fname, "a");
  if (fp == NULL)
  {
    printf("**** Error Opening: %s\n", fname);
  }
  return fp;
}

/********************************************************************/

static bool hour_changed(time_t t1, time_t t2)
{
  struct tm mytm1;
  struct tm mytm2;
  localtime_r(&t1, &mytm1);
  localtime_r(&t2, &mytm2);
  return (mytm1.tm_hour != mytm2.tm_hour);
}

/********************************************************************/

static bool day_changed(time_t t1, time_t t2)
{
  struct tm mytm1;
  struct tm mytm2;
  localtime_r(&t1, &mytm1);
  localtime_r(&t2, &mytm2);
  return (mytm1.tm_yday != mytm2.tm_yday);
}

/********************************************************************/

void ac_point_t::update(void)
{
  /* Update the given ac point. */
  //double amps;
  bool state;
  time_t now;
  double level;

  double cold_temp;
  double hot_temp;
  bool unit_running;

/**
  ai_point_t *cold_temp_point;
  ai_point_t *hot_temp_point;
  di_point_t *unit_running_point;
  do_point_t *unit_disable_point;
***/

  if ((ac_fp == NULL) || (cold_temp_point == NULL) || 
     (unit_running_point == NULL) || (unit_disable_point == NULL))
  {
    return;
  }

  now = time(NULL);
  //amps = ai_point->get_pv();
  //state = di_point->get_pv();

  cold_temp = cold_temp_point->get_pv();
  hot_temp = hot_temp_point->get_pv();
  state = unit_running_point->get_pv();

  if (hour_changed(now, this_hour))
  {
    struct tm mytm;
    localtime_r(&this_hour, &mytm);
    double avg_cold = hour_total_cold / double(hour_num_temp_readings);
    double avg_hot = hour_total_hot / double(hour_num_temp_readings);
    double fraction_on = (double(hour_num_on_readings) / 
         (double(hour_num_on_readings) + double(hour_num_off_readings)));
    fprintf(history_fp, "%d\t%0.2lf\t%0.2lf\t%0.2lf\n", mytm.tm_hour, 
            100.0 * fraction_on, avg_cold, avg_hot); 

    fflush(history_fp);
    //hour_total_amps = 0.0;
    hour_total_cold = 0.0;
    hour_total_hot = 0.0;
    hour_num_on_readings = 0;
    hour_num_off_readings = 0;
    this_hour =  now;
  }

  if (day_changed(now, this_day))
  {
    double fraction_on = (double(day_num_on_readings) / 
         (double(day_num_on_readings) + double(day_num_off_readings)));
    double avg_cold = day_total_cold / double(day_num_temp_readings);
    double avg_hot = day_total_hot / double(day_num_temp_readings);
    fprintf(history_fp, "%s\t%0.2lf\t%0.2lf\t%0.2lf\n", "dia", 
            100.0 * fraction_on, avg_cold, avg_hot); 

    fflush(history_fp);
    //day_total_amps = 0.0;
    day_num_on_readings = 0;
    day_num_off_readings = 0;
    this_day = now;
    ac_fp = open_day_history_file(ac_log_home, "_ac_log.txt", NULL);
    char tbuf[50];
    snprintf(tbuf, sizeof(tbuf), "_%s.txt", this->tag);
    history_fp = open_day_history_file(ac_log_home, tbuf, NULL);
  }


  // Below is a simple method for caluclating % on.
  // - just count on readings vs off readings. Close enough! 
  if (state)
  { 
    hour_num_off_readings++;
    day_num_off_readings++;
  }
  else
  {
    if (delay_elapsed)
    {
      hour_total_cold += cold_temp;
      hour_total_hot += hot_temp;
      day_total_cold += cold_temp;
      day_total_hot += hot_temp;
    }
    //hour_total_amps += amps;
    //day_total_amps += amps;
    hour_num_on_readings++;
    day_num_on_readings++;
  }

  //printf("PUMP UPDATE: %s\n", tag);
  if (change_started)
  {
    time_t now = time(NULL);
    if (now > (last_change_time + int(delay) + 1))
    {
      //printf("%s", change_start_line);
      //printf("%0.3lf\n", amps);
      fprintf(ac_fp, "%s", change_start_line);
      //fprintf(ac_fp, "%0.3lf\n", amps);
      fflush(ac_fp);
      change_started = false;
    }
  }

  if (state != last_state_at_change)
  {
    char datestr[30];
    time_t now = time(NULL); 
    struct tm mytm;
    localtime_r(&now, &mytm);
    strftime(datestr, sizeof(datestr), "%F\t%T", &mytm);
 
    snprintf(change_start_line, sizeof(change_start_line), 
            "%s\t%s\t%s\t%0.2lf\t%0.3lf\t", datestr, tag, 
             unit_running_point->pv_string, level, 0.0);
    last_state_at_change = state;
    last_change_time = now;
    change_started = true;
  }
  
}

/********************************************************************/

ac_point_t **ac_point_t::read(int *cnt, const char *home_dir)
{
  /* Read the ac point from the database. */
  db_point_t *db_point;

  int max_points = 20;
  ac_point_t **ac_points =
	(ac_point_t **) malloc(max_points * sizeof(ac_point_t*));
  MALLOC_CHECK(ac_points);

  int count = 0;


  char path[200];
  safe_strcpy(path, home_dir, sizeof(path));
  safe_strcat(path, "/dbfiles/ac.dat", sizeof(path));
  FILE *fp = fopen(path, "r");
  if (fp == NULL)
  {
    printf("Can't open %s\n", path);
    *cnt = 0;
    return NULL;
  }
  char line[300];

//const char *html_home = ap_config.get_config("htmlhome");

  for (int i=0; NULL != fgets(line, sizeof(line), fp); i++)
  {
    char tmp[300];
    int argc;
    char *argv[25];
    safe_strcpy(tmp, (const char*)line, sizeof(tmp));
    argc = get_delim_args(tmp, argv, '|', 25);
    if (argc == 0)
    {
      continue;
    }
    else if (argv[0][0] == '#')
    {
      continue;
    }
    else if (argc != 7)
    {
      printf("%s: Wrong number of args, line %d\n", path, i+1);
      continue;
    }
    printf("%s", line);
    ac_point_t *ac = new ac_point_t;

    safe_strcpy(ac->tag, (const char*) argv[0], sizeof(ac->tag));
    safe_strcpy(ac->description, (const char*) argv[1], sizeof(ac->description));

    char temp_tag[30];


    safe_strcpy(temp_tag, (const char*) argv[2], sizeof(temp_tag));
    rtrim(temp_tag);
    db_point = db->get_db_point(temp_tag);
    if ((db_point == NULL) || (db_point->point_type() != ANALOG_INPUT))
    {
      ac->cold_temp_point = NULL;
      printf("%s - bad analog input point: %s\n", ac->tag, temp_tag);
    }
    else
    {
      ac->cold_temp_point = (ai_point_t *) db_point;
    }

    safe_strcpy(temp_tag, (const char*) argv[3], sizeof(temp_tag));
    rtrim(temp_tag);
    db_point = db->get_db_point(temp_tag);
    if ((db_point == NULL) || (db_point->point_type() != ANALOG_INPUT))
    {
      ac->hot_temp_point = NULL;
      printf("%s - bad analog input point: %s\n", ac->tag, temp_tag);
    }
    else
    {
      ac->hot_temp_point = (ai_point_t *) db_point;
    }

    safe_strcpy(temp_tag, (const char*) argv[4], sizeof(temp_tag));
    rtrim(temp_tag);
    db_point = db->get_db_point(temp_tag);
    if ((db_point == NULL) || (db_point->point_type() != DISCRETE_INPUT))
    {
      ac->unit_running_point = NULL;
      printf("%s - bad discrete input point: %s\n", ac->tag, temp_tag);
    }
    else
    {
      ac->unit_running_point = (di_point_t *) db_point;
    }

    safe_strcpy(temp_tag, (const char*) argv[5], sizeof(temp_tag));
    rtrim(temp_tag);
    db_point = db->get_db_point(temp_tag);
    if ((db_point == NULL) || (db_point->point_type() != DISCRETE_OUTPUT))
    {
      ac->unit_disable_point = NULL;
      printf("%s - bad discrete output point: %s\n", ac->tag, temp_tag);
    }
    else
    {
      ac->unit_disable_point = (do_point_t *) db_point;
    }

    ac->delay = atof(argv[6]);

    ac->last_state_at_change = true;
    ac->last_change_time = time(NULL);
    ac->last_current = 0.0;
    ac->change_started = false;
    ac->this_hour = ac->this_day = time(NULL);
    ac->hour_num_on_readings = 0;
    ac->hour_num_off_readings = 0;
    ac->hour_num_temp_readings = 0;

    ac->day_num_on_readings = 0;
    ac->day_num_off_readings = 0;
    ac->day_num_temp_readings = 0;


    ac->hour_total_cold = 0.0;
    ac->day_total_cold = 0.0;
    ac->hour_total_hot = 0.0;
    ac->day_total_hot = 0.0;

    /***
    ac->hour_total_amps = 0.0;
    ac->day_total_amps = 0.0;
    ac->min_amps = atof(argv[5]);
    ***/

    const char *html_home = ap_config.get_config("htmlhome");
    if (html_home == NULL)
    {
      printf("Warning: htmlhome variable not set\n");
      snprintf(ac_log_home, sizeof(ac_log_home), "./log/");
    }
    else
    {
      snprintf(ac_log_home, sizeof(ac_log_home), "%s/", html_home);
    }

    if (ac_fp == NULL)
    {
      ac_fp = open_day_history_file(ac_log_home, "_ac_log.txt", NULL);
    }

    char tbuf[50];
    snprintf(tbuf, sizeof(tbuf), "_%s.txt", ac->tag);
    ac->history_fp = open_day_history_file(ac_log_home, tbuf, NULL);
    
    ac_points[count] = ac;
    count++;
    if (count > max_points)
    {
      max_points += 10;
      ac_points = (ac_point_t **) realloc(ac_points,
	       max_points * sizeof(ac_point_t*));
      MALLOC_CHECK(ac_points);
    }
  }

  *cnt = count;
  return ac_points;
}

/******************************************************************/

