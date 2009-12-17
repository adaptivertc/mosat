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

data.cpp

Member functions for data collection points.

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

#define WL_MIN_ARGS (8)

/**********************************************************************/

void web_logger_t::update(void)
{
}

/**********************************************************************/

web_logger_t **web_logger_t::read(int *cnt, const char *home_dir)
{
  int max_points = 50;
  web_logger_t **logger_points =
	(web_logger_t **) malloc((max_points + 1) * sizeof(web_logger_t*));
  MALLOC_CHECK(logger_points);

  *cnt = 0;
  int count = 0;

  char path[200];
  safe_strcpy(path, home_dir, sizeof(path));
  safe_strcat(path, "/dbfiles/web_logger.dat", sizeof(path));
  FILE *fp = fopen(path, "r");
  if (fp == NULL)
  {
    logfile->perror(path);
    logfile->vprint("Can't open: %s\n", path);
    return NULL;
  }
  char line[300];

  for (int i=0; NULL != fgets(line, sizeof(line), fp); i++)
  {
    char tmp[300];
    int argc;
    char *argv[25];
    ltrim(line);
    rtrim(line);

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
    else if (argc < WL_MIN_ARGS)
    {
      logfile->vprint("%s: Wrong number of args (minimum %d), line %d\n", 
          path, WL_MIN_ARGS, i+1);
      continue;
    }
    if (0 != ((argc - 5) % 3)) 
    {
      logfile->vprint(
       "%s: Wrong number of args (%d) (must be 5 + multiple of 3 ), line %d\n", 
         path, argc, i+1);
      continue;
    }

    logfile->vprint("%s\n", line);
    web_logger_t *p = new web_logger_t;

    safe_strcpy(p->tag, (const char*) argv[0], sizeof(p->tag));
    safe_strcpy(p->description, (const char*) argv[1], sizeof(p->description));
    p->sample_interval = atol(argv[2]);
    p->url = strdup(argv[3]);
    p->key = strdup(argv[4]);

    p->num_analog = 0;
    p->num_discrete = 0;
    int num_points = (argc - (WL_MIN_ARGS - 5)) / 3;
    int start;
    for (int j=0; j < num_points; j++)
    {
      start = 5 + (j * 3);
      if (0 == strcasecmp(argv[start+2], "bool"))
      {
        p->num_discrete++;
      }
      else if (0 == strcasecmp(argv[start+2], "float"))
      {
        p->num_analog++;
      }
      else
      {
        logfile->vprint(
         "%s: Unknown value type: %s, must be 'bool' or 'float', line %d\n", 
          path, argv[start+2], i+1);
      }
    } 

    logfile->vprint("%d analog points\n", p->num_analog);
    logfile->vprint("%d discrete points\n", p->num_discrete);

    // Allocate space
    p->analog_points = new analog_point_t *[p->num_analog + 1];
    p->discrete_points = new discrete_point_t *[p->num_discrete + 1];
    p->analog_web_tags = new char *[p->num_analog + 1];
    p->discrete_web_tags = new char *[p->num_discrete + 1];

    p->num_analog = 0;
    p->num_discrete = 0;
    for (int j=0; j < num_points; j++)
    {
      start = 5 + (j * 3);
      char temp_tag[50];
      db_point_t *db_point;
      safe_strcpy(temp_tag, (const char*) argv[start], sizeof(temp_tag));
      ltrim(temp_tag);
      rtrim(temp_tag);
      logfile->vprint("Tag = %s\n", temp_tag);

      if (0 == strcasecmp(argv[start+2], "float"))
      {
        p->analog_web_tags[p->num_analog] = strdup(argv[start+1]);
        db_point = db->get_db_point(temp_tag);
        if ((db_point == NULL) || (db_point->pv_type() != ANALOG_VALUE))
        {
          p->analog_points[p->num_analog] = NULL;
          logfile->vprint("Bad analog point: %s\n", temp_tag);
        }
        else
	{
          p->analog_points[p->num_analog] = (analog_point_t *) db_point;
          logfile->vprint("analog point %d: %s\n", p->num_analog, temp_tag);
        }
        logfile->vprint("Web Tag: %s\n", p->analog_web_tags[p->num_analog]);
        p->num_analog++;
      }
      else if (0 == strcasecmp(argv[start+2], "bool"))
      {
        p->discrete_web_tags[p->num_discrete] = strdup(argv[start+1]);
        db_point = db->get_db_point(temp_tag);
        if ((db_point == NULL) || (db_point->pv_type() != DISCRETE_VALUE))
        {
          p->discrete_points[p->num_discrete] = NULL;
          logfile->vprint("Bad discrete point: %s\n", temp_tag);
        }
        else
	{
          p->analog_points[p->num_analog] = (analog_point_t *) db_point;
          logfile->vprint("discrete point %d: %s\n", p->num_discrete, temp_tag);
        }
        logfile->vprint("Web Tag: %s\n", p->discrete_web_tags[p->num_discrete]);
        p->num_discrete++;
      }
      else
      {
      }
    } 

    //p->hour_enable = (argv[6][0] == '1');

    logger_points[count] = p;
    count++;
    if (count > max_points)
    {
      max_points += 10;
      logger_points = (web_logger_t **) realloc(logger_points,
		  max_points * sizeof(web_logger_t*));
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

