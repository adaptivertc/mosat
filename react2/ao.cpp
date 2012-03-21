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

/****

Ao.c

Code for the analog output member functions.

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

void ao_point_t::send(double val)
{
  point_lock_t l(&this->point_lock, tag);
  ramp_on = false;
  this->sendit(val);
}

/********************************************************************/

int ao_point_t::get_json(char *attribute, char *buf, int sz)
{
  return analog_point_t::get_json(attribute, buf, sz);
}

/********************************************************************/

int ao_point_t::set_json(char *attribute, char *value)
{
  if (0 == strcmp(attribute, "pv"))
  {
    this->send(atof(value));
    return 0;
  }
  else if (0 == strcmp(attribute, "send"))
  {
    this->send(atof(value));
    return 0;
  }
  else if (0 == strcmp(attribute, "ramp"))
  {
    if (value[0] != '[') return -3; // must be an array
    double ramp_val = atof(value + 1);
    double ramp_time = 0;
    bool found = false;
    for (unsigned int i=0; i < strlen(value); i++)
    {
      if (value[i] == ',')
      {
        found = true;
        ramp_time = atof(&value[i + 1]);
      }
    }
    if (found) 
    {
      this->ramp(ramp_val, ramp_time);
      return 0;
    }
    return -3;
  }
  return -2;
}

/********************************************************************/

void ao_point_t::sendit(double val)
{
  /* Send an analog output. */

  double raw;

  if (val > output_limit_hi)
  {
    val = output_limit_hi;
  }
  if (val < output_limit_lo)
  {
    val = output_limit_lo;
  }
  raw = ((raw_hi - raw_lo) * (val - eu_lo) / (eu_hi - eu_lo)) + raw_lo;
  db->send_ao(driver, card, channel, raw);
  //printf("%s.send(%lf)\n", tag, val);
  pv = val;
  display_pv();
}

/********************************************************************/

void ao_point_t::update(void)
{
  point_lock_t l(&this->point_lock, tag);
  if (!ramp_on)
  {
    return;
  }
  double sendval = pv + ramp_inc;
  if (ramp_inc > 0)
  {
    if (sendval > ramp_val)
    {
      sendval = ramp_val;
      ramp_on = false;
    }
  }
  else
  {
    if (sendval < ramp_val)
    {
      sendval = ramp_val;
      ramp_on = false;
    }
  }
  //printf("%s: ramping: %lf\n", this->tag, sendval);
  this->sendit(sendval);
}

/********************************************************************/

void ao_point_t::ramp(double val, double time)
{
  point_lock_t l(&this->point_lock, tag);
  ramp_on = true;
  ramp_val = val;
  double dif = val - pv;
  int increments = int(time * db->get_sample_rate());
  ramp_inc = dif / increments;
  //printf("ramping to: %lf, ramp increment: %lf", val, ramp_inc);
}

/********************************************************************/

void ao_point_t::init_values(void)
{
    set_format();
    pv = 0.0;
    //pv_attr = NORMAL_PV_ATTR;
}

/********************************************************************/

ao_point_t *ao_point_t::read_one(int argc, char *argv[], char *err, int esz)
{
    ao_point_t *p = new ao_point_t;
    if (argc != 15)
    {
      snprintf(err, esz, "Wrong number of args for ao");
      return NULL;
    }
    safe_strcpy(p->tag, (const char*) argv[0], sizeof(p->tag));
    safe_strcpy(p->description, (const char*) argv[1], sizeof(p->description));
    safe_strcpy(p->eu, (const char*) argv[2], sizeof(p->eu));
    p->driver = atol(argv[3]);
    p->card = atol(argv[4]);
    p->channel = atol(argv[5]);
    p->eu_lo = atof(argv[6]);
    p->eu_hi = atof(argv[7]);
    p->raw_lo = atof(argv[8]);
    p->raw_hi = atof(argv[9]);
    p->decimal_places = atoi(argv[10]);

    p->output_limit_lo = atof(argv[11]);
    p->output_limit_hi = atof(argv[12]);
    p->scale_lo = atof(argv[13]);
    p->scale_hi = atof(argv[14]);


    p->init_values();

    return p;
}
/********************************************************************/

ao_point_t **ao_point_t::read(int *cnt, const char *home_dir)
{
  /*********/

  int max_points = 100;
  ao_point_t **ao_points =
	(ao_point_t **) malloc(max_points * sizeof(ao_point_t*));
  MALLOC_CHECK(ao_points);

  *cnt = 0;
  int count = 0;

  char path[200];
  safe_strcpy(path, home_dir, sizeof(path));
  safe_strcat(path, "/dbfiles/ao.dat", sizeof(path));
  FILE *fp = fopen(path, "r");
  if (fp == NULL)
  {
    logfile->perror(path);
    logfile->vprint("Can't open %s", path);
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
    /********
    else if (argc != 13)
    {
      printf("%s: Wrong number of args, line %d", path, i+1);
      continue;
    }
    *********/

    if (count >= max_points)
    {
      max_points += 50;
      int new_size = max_points * sizeof(ao_point_t*);
      logfile->vprint("Reallocating: new size = %d\n", new_size);
      ao_points = (ao_point_t **) realloc(ao_points, new_size);
      MALLOC_CHECK(ao_points);
    }

    //ao_point_t *p = new ao_point_t;
    //printf(line);
    /*******/
    /******
    AO1|Analog Output 1|PSI|0|0|0|0|100|0|4095|1|0|100|
    Tag
    Description
    Engineering Units Label
    Driver Number
    Card
    Channel
    Engineering Units Low
    Engineering Units High
    Raw Value Low
    Raw Value High
    Decimal Places
    Output Limit Low
    Output Limit High
    ******/
    /******/
    char errbuf[100];
    ao_points[count] = ao_point_t::read_one(argc, argv, errbuf, sizeof(errbuf));
    if (ao_points[count] == NULL)
    {
      logfile->vprint("%s:%d\n", path, i+1);
      logfile->vprint("%s\n", errbuf);
      continue;
    }
    logfile->vprint("%s\n", line);
    count++;
    /**********/

  }

  if (count == 0)
  {
    free(ao_points);
    return NULL;
  }
  *cnt = count;
  return ao_points;
  /***************/
}

/********************************************************************/
