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

valve.cpp

Member functions for a valve monitor.

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

/********************************************************************/

void valve_point_t::open(void)
{
  point_lock_t l(&this->point_lock, tag);
  this->in_transition = true; 
  this->start_time = db->get_time();
  this->pv = true;
  this->do_point->send(true);
  this->pv_string = this->hi_desc;
}

/********************************************************************/

void valve_point_t::close(void)
{
  point_lock_t l(&this->point_lock, tag);
  this->in_transition = true; 
  this->start_time = db->get_time();
  this->pv = false;
  this->do_point->send(false);
  this->pv_string = this->lo_desc;
}

/********************************************************************/

void valve_point_t::update(void)
{
  point_lock_t l(&this->point_lock, tag);

  bool open = open_point->get_pv();
  bool closed = closed_point->get_pv();
  double tnow = db->get_time();
  bool valid = (!pv && !open && closed) || (pv && open && !closed);
  
  if (this->in_transition)
  {
    if ((tnow - this->start_time) > this->delay)
    {
      this->in_transition = false; 
    }
    else
    {
      return; 
    }
  }

  if (!valid)
  {
  }
}


/********************************************************************/

void valve_point_t::init_values(void)
{
  pv = false;
  snprintf(lo_desc, sizeof(lo_desc), "CLOSED");
  snprintf(hi_desc, sizeof(hi_desc), "OPEN");
  pv_string = lo_desc;

  db_point_t *db_point;

  ltrim(do_tag);
  rtrim(do_tag);
  db_point = db->get_db_point(do_tag);

  if (db_point == NULL)
  {
    logfile->vprint("%s - bad TAGNAME: %s\n", tag, do_tag);
    do_point = NULL;
  }
  else
  {
    do_point = dynamic_cast <do_point_t *> (db_point);
    if (do_point == NULL)
    {
      logfile->vprint("%s - bad discrete output point: %s\n", tag, do_tag);
    }
  }

  ltrim(closed_tag);
  rtrim(closed_tag);
  db_point = db->get_db_point(closed_tag);

  if (db_point == NULL)
  {
    logfile->vprint("%s - bad TAGNAME: %s\n", tag, closed_tag);
    closed_point = NULL;
  }
  else
  {
    closed_point = dynamic_cast <discrete_point_t *> (db_point);
    if (closed_point == NULL)
    {
      logfile->vprint("%s - bad discrete tag for closed: %s\n", tag, closed_tag);
    }
  }

  ltrim(open_tag);
  rtrim(open_tag);
  db_point = db->get_db_point(open_tag);

  if (db_point == NULL)
  {
    logfile->vprint("%s - bad TAGNAME: %s\n", tag, open_tag);
    open_point = NULL;
  }
  else
  {
    open_point = dynamic_cast <discrete_point_t *> (db_point);
    if (open_point == NULL)
    {
      logfile->vprint("%s - bad discrete tag for open: %s\n", tag, open_tag);
    }
  }
}

/********************************************************************/

valve_point_t **valve_point_t::read(int *cnt, const char *home_dir)
{
  int max_points = 200;
  int count = 0;
  valve_point_t **valve_points =
	(valve_point_t **) malloc(max_points * sizeof(valve_point_t*));
  MALLOC_CHECK(valve_points);

  *cnt = 0;

  char path[200];
  safe_strcpy(path, home_dir, sizeof(path));
  safe_strcat(path, "/dbfiles/valve.dat", sizeof(path));
  FILE *fp = fopen(path, "r");
  if (fp == NULL)
  {
    logfile->perror(path);
    logfile->vprint("Can't open file: %s\n", path);
    return NULL;
  }
  char line[300];

  for (int i=0; NULL != fgets(line, sizeof(line), fp); i++)
  {
    char tmp[300];
    int argc;
    char *argv[25];
    //DO|Discrete Output 1|0|0|1|HI|LO|
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
    else if (argc != 6)
    {
      logfile->vprint("%s: Wrong number of args, line %d", path, i+1);
      continue;
    }
    logfile->vprint("%s\n", line);
    valve_point_t *p = new valve_point_t;

    /*****
    Tag
    Description
    do_tag
    closed_tag
    open_tag
    delay
    ******/

    safe_strcpy(p->tag, (const char*) argv[0], sizeof(p->tag));
    safe_strcpy(p->description, (const char*) argv[1], sizeof(p->description));

    snprintf(p->do_tag, sizeof(p->do_tag), "%s", argv[2]);
    snprintf(p->closed_tag, sizeof(p->closed_tag), "%s", argv[3]);
    snprintf(p->open_tag, sizeof(p->open_tag), "%s", argv[4]);

    p->delay = atof(argv[5]);

    p->init_values();

    valve_points[count] = p;
    count++;
    if (count >= max_points)
    {
      max_points += 50;
      valve_points = (valve_point_t **) realloc(valve_points,
	    max_points * sizeof(valve_point_t*));
      MALLOC_CHECK(valve_points);
    }
  }
  *cnt = count;
  if (count == 0)
  {
    free(valve_points);
    return NULL;
  }
  return valve_points;
}

/************************************************************************/
