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

/*** 

Di.c

Member functions for discrete input points.

*************************************************************************/



#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>

#include "rtcommon.h"
#include "reactpoint.h"
#include "db_point.h"
#include "db.h"
#include "arg.h"

/**********************************************************************/

void di_point_t::update(bool value)
{
  point_lock_t l(&point_lock, tag);
  //printf("entering update for %s\n", tag);
  /* Update the discrete input point. */

  //printf("@@ tag: %s, value: %s, invert: %s\n", tag, value ? "T":"F", invert_pv ? "T":"F");
  
  if (invert_pv) value = not value;
  pv_last = pv;
  pv  = value;

  display_pv();
  check_alarms();
  //printf("exiting update for %s\n", tag);

}

/**********************************************************************/

void di_point_t::init_values(void)
{
  pv_string = lo_desc;
  pv = false;
  json_str = NULL;
}

/**********************************************************************/
static const char *get_json_dstate(int state)
{
  switch (state) 
  {
    case DISCRETE_LO:
      return "LOW";
      break;
    case DISCRETE_HI:
      return "HIGH";
      break;
    case DISCRETE_NONE:
    default:
      return "NONE";
      break;
  }
}
/**********************************************************************/

const char *di_point_t::get_config_json(void)
{
  point_lock_t l(&this->point_lock, tag);
  if (json_str == NULL)
  {
     asprintf(&json_str, "{\"tag\":\"%s\",\"description\":\"%s\",\"driver\":%d,\"card\":%d,\"channel\":%d, \"lo_desc\":\"%s\",\"hi_desc\":\"%s\", \"alarm_state\":\"%s\",\"shutdown_state\":\"%s\"}",
         this->tag,
         this->description,
         this->driver,
         this->card,
         this->channel,
         this->lo_desc,
         this->hi_desc,
         get_json_dstate(this->alarm_state),
         get_json_dstate(this->shutdown_state)
      );
  }
  return json_str;
}
/**
string|Tag|tag|25|
string|Description|description|50|
int|Driver|driver|25|
int|Card|card|25|
int|Channel|channel|
string|Low Description|lo_desc|12|
string|High Description|hi_desc|12|
Separator|Alarms|
dropdown|Alarm State|alarm_state|High,Low,None|H,L,N|
dropdown|Shutdown State|shutdown_state|High,Low,None|H,L,N|
**/


/*********************************************************************/

di_point_t **di_point_t::read(int *cnt, const char *home_dir)
{
  int max_points = 200;
  di_point_t **di_points =
	(di_point_t **) malloc(max_points * sizeof(di_point_t*));
  MALLOC_CHECK(di_points);

  *cnt = 0;
  int count = 0;

  char path[200];
  safe_strcpy(path, home_dir, sizeof(path));
  safe_strcat(path, "/dbfiles/di.dat", sizeof(path));
  FILE *fp = fopen(path, "r");
  if (fp == NULL)
  {
    logfile->perror(path);
    logfile->vprint("Can't open %s\n", path);
    return NULL;
  }
  char line[300];

  for (int i=0; NULL != fgets(line, sizeof(line), fp); i++)
  {
    char tmp[300];
    int argc;
    char *argv[25];
    //DI1|Discrete Input 1|0|0|1|HI|LO|N|N|
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

    else if (argc != 10)
    {
      logfile->vprint("%s: Wrong number of args, line %d\n", path, i+1);
      continue;
    }

    di_point_t *p = new di_point_t;
    logfile->vprint("%s\n", line);

    /*****
    Tag
    Description
    Driver Number
    Card
    Channel
    Low Description
    High Description
    Alarm State
    Shutdown State
    ******/

    safe_strcpy(p->tag, (const char*) argv[0], sizeof(p->tag));
    safe_strcpy(p->description, (const char*) argv[1], sizeof(p->description));

    p->driver = atol(argv[2]);
    p->card = atol(argv[3]);
    p->channel = atol(argv[4]);

    safe_strcpy(p->lo_desc, (const char*) argv[5], sizeof(p->lo_desc));
    safe_strcpy(p->hi_desc, (const char*) argv[6], sizeof(p->hi_desc));

    switch (argv[7][0])
    {
      case 'H':
      case 'h':
	p->alarm_state = DISCRETE_HI;
	break;
      case 'L':
      case 'l':
	p->alarm_state = DISCRETE_LO;
	break;
      case 'N':
      case 'n':
	p->alarm_state = DISCRETE_NONE;
	break;
      default:
	p->alarm_state = DISCRETE_NONE;
	break;
    }

    switch (argv[8][0])
    {
      case 'H':
      case 'h':
	p->shutdown_state = DISCRETE_HI;
	break;
      case 'L':
      case 'l':
	p->shutdown_state = DISCRETE_LO;
	break;
      case 'N':
      case 'n':
	p->shutdown_state = DISCRETE_NONE;
	break;
      default:
	p->shutdown_state = DISCRETE_NONE;
	break;
    }
    p->invert_pv = (argv[9][0] == '1');

    p->init_values();

    di_points[count] = p;
    count++;
    if (count >= max_points)
    {
      max_points += 100;
      di_points = (di_point_t **) realloc(di_points,
		  max_points * sizeof(di_point_t*));
      MALLOC_CHECK(di_points);
    }
  }

  if (count == 0)
  {
    free(di_points);
    return NULL;
  }
  *cnt = count;
  return di_points;

}

/*************************************************************************/

