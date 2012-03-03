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
  pid control

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

/********************************************************************/

void remote_pid_t::change_setpoint(double val, double ramp_time)
{
  /* change the set point for the given control point. */

  if ((ai_point == NULL) || (ao_point == NULL))
  {
    return;
  }

  if (!control_enabled)
  {
    this->start_control();
  }

  set_ramp(val, ramp_time);
  pv = setpoint;
  display_pv();
}

/******************************************************************/

void remote_pid_t::start_control(void)
{
  /* Start control for the given control point. */

  if ((ai_point == NULL) || (ao_point == NULL) || (enable_point == NULL))
  {
    return;
  }
  control_enabled = true;
  reset_alarms();
  enable_point->send(true);
}

/******************************************************************/

void remote_pid_t::stop_control(void)
{
  /* Stop control for the given control point. */

  control_enabled = false;
  point_state = STATE_NORMAL;
  reset_alarms();
  enable_point->send(false);
}

/******************************************************************/

void remote_pid_t::get_gains(double *p, double *i, double *d)
{
  *p = this->p_gain;
  *i = this->i_time;
  *d = this->d_time;
}

/******************************************************************/

void remote_pid_t::send_gains(double p, double i, double d)
{
  if ((p_point == NULL) || (i_point == NULL) || (d_point == NULL))
  {
    return;
  }
  this->p_gain = p;
  this->i_time = i;
  this->d_time = d;
  p_point->send(p);
  i_point->send(i);
  d_point->send(d);
  
}

/******************************************************************/

void remote_pid_t::update(void)
{
  /* Update the given control point. */

  if (!control_enabled)
  {
    return;
  }
  update_ramp();

    /* Now do some alarm detection */
  check_alarms();
}

/******************************************************************/

void remote_pid_t::init_values(void)
{
  db_point_t *db_point;

  ltrim(ai_tag);
  rtrim(ai_tag);
  db_point = db->get_db_point(ai_tag);

  if (db_point == NULL)
  {
    logfile->vprint("%s - bad TAGNAME: %s\n", tag, ai_tag);
    ai_point = NULL;
  }
  else
  {
    ai_point = dynamic_cast <analog_point_t *> (db_point);
    if (ai_point == NULL)
    {
      logfile->vprint("%s - bad analog input point: %s\n", tag, ai_tag);
    }
  }

  ltrim(ao_tag);
  rtrim(ao_tag);
  db_point = db->get_db_point(ao_tag);

  if (db_point == NULL)
  {
    logfile->vprint("%s - bad TAGNAME: %s\n", tag, ao_tag);
    ao_point = NULL;
  }
  else
  {
    ao_point = dynamic_cast <ao_point_t *> (db_point);
    if (ao_point == NULL)
    {
      logfile->vprint("%s - bad analog output point: %s\n", tag, ao_tag);
    }
  }

  ltrim(enable_tag);
  rtrim(enable_tag);
  if (0 == strlen(enable_tag))
  {
    enable_point = NULL; 
  }
  else
  {
    db_point = db->get_db_point(enable_tag);
    if (db_point == NULL)
    {
      logfile->vprint("%s - bad enable TAGNAME: %s\n", tag, enable_tag);
      enable_point = NULL;
    }
    else
    {
      enable_point = dynamic_cast <do_point_t *> (db_point);
      if (enable_point == NULL)
      {
        logfile->vprint("%s - NOT a discrete output point: %s\n", tag, enable_tag);
      }
    }
  }

  ltrim(p_tag);
  rtrim(p_tag);
  if (0 == strlen(p_tag))
  {
    p_point = NULL; 
  }
  else
  {
    db_point = db->get_db_point(p_tag);
    if (db_point == NULL)
    {
      logfile->vprint("%s - bad proportional gain TAGNAME: %s\n", tag, p_tag);
      p_point = NULL;
    }
    else
    {
      p_point = dynamic_cast <ao_point_t *> (db_point);
      if (p_point == NULL)
      {
        logfile->vprint("%s - NOT an analog output point: %s\n", tag, p_tag);
      }
    }
  }

  ltrim(i_tag);
  rtrim(i_tag);
  if (0 == strlen(i_tag))
  {
    i_point = NULL; 
  }
  else
  {
    db_point = db->get_db_point(i_tag);
    if (db_point == NULL)
    {
      logfile->vprint("%s - bad integral time TAG: %s\n", tag, i_tag);
      i_point = NULL;
    }
    else
    {
      i_point = dynamic_cast <ao_point_t *> (db_point);
      if (i_point == NULL)
      {
        logfile->vprint("%s - NOT an analog output point: %s\n", tag, i_tag);
      }
    }
  }

  ltrim(d_tag);
  rtrim(d_tag);
  if (0 == strlen(d_tag))
  {
    d_point = NULL; 
  }
  else
  {
    db_point = db->get_db_point(d_tag);
    if (db_point == NULL)
    {
      logfile->vprint("%s - bad derivative time TAG: %s\n", tag, d_tag);
      d_point = NULL;
    }
    else
    {
      d_point = dynamic_cast <ao_point_t *> (db_point);
      if (d_point == NULL)
      {
        logfile->vprint("%s - NOT an analog output point: %s\n", tag, d_tag);
      }
    }
  }

  set_format();
  pv = setpoint = 0.0;
  point_state = STATE_NORMAL;
  dev_caut_detect_time = -1;
  dev_caut_detect_time = -1;
  control_enabled = false;
  ramp_is_on = false;
  display_pv();
  reset_alarms();
}

/********************************************************************/

remote_pid_t **remote_pid_t::read(int *cnt, const char *home_dir)
{
  /* Read the pid point from the database. */

  int max_points = 20;
  remote_pid_t **remote_pids =
	(remote_pid_t **) malloc(max_points * sizeof(remote_pid_t*));
  MALLOC_CHECK(remote_pids);

  int count = 0;


  char path[200];
  safe_strcpy(path, home_dir, sizeof(path));
  safe_strcat(path, "/dbfiles/remote_pid.dat", sizeof(path));
  FILE *fp = fopen(path, "r");
  if (fp == NULL)
  {
    logfile->perror(path);
    logfile->vprint("Can't open %s\n", path);
    *cnt = 0;
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
    else if (argc != 23)
    {
      logfile->vprint("%s: Wrong number of args, line %d\n", path, i+1);
      continue;
    }
    logfile->vprint("%s\n", line);
    remote_pid_t *rpid = new remote_pid_t;

    safe_strcpy(rpid->tag, (const char*) argv[0], sizeof(rpid->tag));
    safe_strcpy(rpid->description, (const char*) argv[1], sizeof(rpid->description));
    safe_strcpy(rpid->eu, (const char*) argv[2], sizeof(rpid->eu));
    rpid->decimal_places = atol(argv[3]);


    rpid->setpoint_limit_hi = atof(argv[4]);

    rpid->setpoint_limit_lo = atof(argv[5]);

    rpid->p_gain = atof(argv[6]);
    rpid->i_time = atof(argv[7]);
    rpid->d_time = atof(argv[8]);

    snprintf(rpid->ai_tag, sizeof(rpid->ai_tag), "%s", argv[9]);
    snprintf(rpid->ao_tag, sizeof(rpid->ao_tag), "%s", argv[10]);
    snprintf(rpid->enable_tag, sizeof(rpid->enable_tag), "%s", argv[11]);
    snprintf(rpid->p_tag, sizeof(rpid->p_tag), "%s", argv[12]);
    snprintf(rpid->i_tag, sizeof(rpid->i_tag), "%s", argv[13]);
    snprintf(rpid->d_tag, sizeof(rpid->d_tag), "%s", argv[14]);

    rpid->deviation_alarm = atof(argv[15]);
    rpid->deviation_caution = atof(argv[16]);
    rpid->deviation_delay = atof(argv[17]);
    rpid->dev_alarm_enable = (argv[18][0] == '1');
    rpid->dev_alarm_shutdown = (argv[19][0] == '1');
    rpid->dev_caution_enable = (argv[20][0] == '1');
    rpid->scale_lo = atof(argv[21]);
    rpid->scale_hi = atof(argv[22]);


    rpid->init_values();

    remote_pids[count] = rpid;
    count++;
    if (count >= max_points)
    {
      max_points += 10;
      remote_pids = (remote_pid_t **) realloc(remote_pids,
	       max_points * sizeof(remote_pid_t*));
      MALLOC_CHECK(remote_pids);
    }
  }

  *cnt = count;
  return remote_pids;
}

/******************************************************************/

