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

discrete_logger.cpp

Member functions for discrete logger point. This logger will log all
changes to discrete points given in the list.


*************************************************************************/



#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>

#include "rtcommon.h"
#include "db_point.h"
#include "db.h"
#include "arg.h"
#include "ap_config.h"

/**********************************************************************/

static bool day_changed(time_t t1, time_t t2)
{
  struct tm mytm1;
  struct tm mytm2;
  localtime_r(&t1, &mytm1);
  localtime_r(&t2, &mytm2);
  if (mytm1.tm_yday == mytm2.tm_yday)
  {
    return false;
  }
  else 
  {
    printf("********* Day Changed\n");
    return true;
  }
}

/**********************************************************************/

static FILE *open_day_history_file(const char * pre, const char *post, FILE *fp)
{
  char fname[500];
  if (fp != NULL)
  {
    fclose(fp);
  }
  const char *loghome = ap_config.get_config("LogHome");
  if (loghome == NULL)
  {
    loghome = "./";
    printf("Log home not specified, using: %s\n", loghome);
  }
  if (pre == NULL)
  {
    pre = "";
  }
  if (post == NULL)
  {
    post = "";
  }
  time_t now = time(NULL);
  char buf1[30];
  struct tm mytm;
  localtime_r(&now, &mytm);
  strftime(buf1, sizeof(buf1), "%Y%m%d", &mytm);
  snprintf(fname, sizeof(fname), "%s/%s%s%s", loghome, pre, buf1, post);
  printf("Opening %s\n", fname);
  fp = fopen(fname, "a");
  if (fp == NULL)
  {
    printf("**** Error Opening %s\n", fname);
  }
  return fp;
}

/**********************************************************************/ 

void discrete_logger_t::update(void)
{
  //fprintf(instantaneous_fp, "data:");
  if (!collecting)
  {
    fprintf(stdout, " **** NOT collecting\n");
    return;
  }
  time_t now = time(NULL);

  char buf[30];
  struct tm mytm;
  localtime_r(&now, &mytm);
  strftime(buf, sizeof(buf), "%F\t%T", &mytm);

  bool day_change = day_changed(now, last_log_time);

  if (day_change)
  {
    instantaneous_fp = open_day_history_file(base_name, 
                         ".txt", instantaneous_fp);
  }
  int n_changes = 0;
  for (int i=0; i < num_points; i++)
  {
    if (discrete_points[i] == NULL) continue;
    bool vnow = discrete_points[i]->get_pv();
    if (vnow != last_discrete_vals[i])
    {
      const char *tg = discrete_points[i]->get_tag();
      const char *snow;
      const char *slast;
      //printf("Tag changed: %s\n", tg);
      if (vnow) 
      {
        snow = discrete_points[i]->hi_desc;
        slast = discrete_points[i]->lo_desc;
      }
      else
      {
        snow = discrete_points[i]->lo_desc;
        slast = discrete_points[i]->hi_desc;
      }
      fprintf(instantaneous_fp, "%s\t%s\t%s --> %s\n", 
            buf, tg, slast, snow);
      last_discrete_vals[i] = vnow;
      n_changes++;
    }
  }
  if (n_changes > 0)
  {
    fflush(instantaneous_fp);
  }
  last_log_time = now;
}

/**********************************************************************/

discrete_logger_t **discrete_logger_t::read(int *cnt, const char *home_dir)
{
  int max_points = 50;
  discrete_logger_t **logger_points =
	(discrete_logger_t **) malloc((max_points + 1) * sizeof(discrete_logger_t*));
  MALLOC_CHECK(logger_points);

  *cnt = 0;
  int count = 0;

  char path[200];
  safe_strcpy(path, home_dir, sizeof(path));
  safe_strcat(path, "/dbfiles/discrete_logger.dat", sizeof(path));
  FILE *fp = fopen(path, "r");
  if (fp == NULL)
  {
    printf("Can't open: %s\n", path);
    return NULL;
  }
  char line[300];

  for (int i=0; NULL != fgets(line, sizeof(line), fp); i++)
  {
    char tmp[300];
    int argc;
    char *argv[25];
    //DI1|Discrete Input 1|0|0|1|HI|LO|N|N|
    safe_strcpy(tmp, (const char*) line, sizeof(tmp));
    argc = get_delim_args(tmp, argv, '|', 25);
    if (argc == 0)
    {
      continue;
    }
    else if (argv[0][0] == '#')
    {
      continue;
    }
    else if (argc < 4)
    {
      printf("%s: Wrong number of args (minimum 4), line %d\n", path, i+1);
      continue;
    }

    printf("%s", line);
    discrete_logger_t *p = new discrete_logger_t;

    safe_strcpy(p->tag, (const char*) argv[0], sizeof(p->tag));
    safe_strcpy(p->description, (const char*) argv[1], sizeof(p->description));
    safe_strcpy(p->base_name, (const char*) argv[2], sizeof(p->base_name));
    p->collecting = true;
    //p->instantaneous_fp = fopen(p->base_name, "w");
    p->instantaneous_fp = open_day_history_file(p->base_name, ".txt", NULL);
    if (p->instantaneous_fp == NULL)
    {
      perror(p->base_name);
      p->collecting = false;
    } 
    if (p->collecting)
    {
      printf("collection is on for %s\n", p->tag);
    }

    p->num_points = argc - 3;
    p->discrete_points = new discrete_point_t *[p->num_points + 1];
    p->last_discrete_vals = new bool[p->num_points + 1];

    for (int j=3; j < argc; j++)
    {
      char temp_tag[50];
      db_point_t *db_point;
      safe_strcpy(temp_tag, (const char*) argv[j], sizeof(temp_tag));
      rtrim(temp_tag);
      db_point = db->get_db_point(temp_tag);
      if ((db_point == NULL) || (db_point->pv_type() != DISCRETE_VALUE))
      {
        p->discrete_points[j-3] = NULL;
        printf("Bad discrete point: %s\n", temp_tag);
      }
      else
      {
        p->discrete_points[j-3] = (discrete_point_t *) db_point;
      }
      printf("discrete point %d: %s\n", j-3, temp_tag);
      p->last_discrete_vals[j-3] = false;
    }
    p->last_log_time = time(NULL);

    logger_points[count] = p;
    count++;
    if (count > max_points)
    {
      max_points += 10;
      logger_points = (discrete_logger_t **) realloc(logger_points,
		  max_points * sizeof(discrete_logger_t*));
      MALLOC_CHECK(logger_points);
    }
  }

  *cnt = count;
  if (count == 0)
  {
    free(logger_points);
    return NULL;
  }
  return logger_points;
}

/*************************************************************************/

