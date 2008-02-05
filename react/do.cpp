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

Do.c

Member functions for discrete output points.

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

void do_point_t::send(bool val)
{
  /* Send the discrete output. */

  tdo_on = false; // if tdo is on, turn it off;
  db->send_do(driver, card, channel, val);
  pv = val ? DISCRETE_HI: DISCRETE_LO;
  switch (pv)
  {
    case DISCRETE_HI:
      pv_string = hi_desc;
      //pv_attr = NORMAL_PV_ATTR;
      break;
    case DISCRETE_LO:
      pv_string = lo_desc;
      //pv_attr = NORMAL_PV_ATTR;
      break;
    case DISCRETE_FAILED:
      pv_string = "Failed";
      point_state = STATE_FAILED;
      //pv_attr = FAILED_ATTR;
      break;
    default:
      pv_string = "*******";
      break;
  }
  display_pv();
}

/********************************************************************/

void do_point_t::tdo(double val)
{
  this->send(true);
  double t = db->get_time();
  tdo_end_time = t + val;
  tdo_on = true;
}


/********************************************************************/

void do_point_t::update(void)
{
  if (!tdo_on)
  {
    return;
  }
  double t = db->get_time();
  if (t > tdo_end_time)
  {
    this->send(false);
    tdo_on = false;
  }
}


/********************************************************************/

do_point_t **do_point_t::read(int *cnt, const char *home_dir)
{
  /*************/

  int max_points = 50;
  do_point_t **do_points =
	(do_point_t **) malloc(max_points * sizeof(do_point_t*));
  MALLOC_CHECK(do_points);

  *cnt = 0;


  char path[200];
  safe_strcpy(path, home_dir, sizeof(path));
  safe_strcat(path, "/dbfiles/do.dat", sizeof(path));
  FILE *fp = fopen(path, "r");
  if (fp == NULL)
  {
    printf("Can't open file: %s\n", path);
    return NULL;
  }
  char line[300];

  for (int i=0; NULL != fgets(line, sizeof(line), fp); i++)
  {
    char tmp[300];
    int argc;
    char *argv[25];
    //DO|Discrete Output 1|0|0|1|HI|LO|
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
    else if (argc != 7)
    {
      printf("%s: Wrong number of args, line %d", path, i+1);
      continue;
    }
    printf(line);
    do_point_t *p = new do_point_t;

    /*****
    Tag
    Description
    Driver Number
    Card
    Channel
    Low Description
    High Description
    ******/

    safe_strcpy(p->tag, (const char*) argv[0], sizeof(p->tag));
    safe_strcpy(p->description, (const char*) argv[1], sizeof(p->description));

    p->driver = atol(argv[2]);
    p->card = atol(argv[3]);
    p->channel = atol(argv[4]);

    safe_strcpy(p->lo_desc, (const char*) argv[5], sizeof(p->lo_desc));
    safe_strcpy(p->hi_desc, (const char*) argv[6], sizeof(p->hi_desc));

    p->pv = DISCRETE_LO;
    p->pv_string = p->lo_desc;
    //p->pv_attr = NORMAL_PV_ATTR;

    do_points[*cnt] = p;
    (*cnt)++;
    if (*cnt > max_points)
    {
      max_points += 5;
      do_points = (do_point_t **) realloc(do_points,
	    max_points * sizeof(do_point_t*));
      MALLOC_CHECK(do_points);
    }
  }
  if (*cnt == 0)
  {
    free(do_points);
    return NULL;
  }
  return do_points;
}

/************************************************************************/
