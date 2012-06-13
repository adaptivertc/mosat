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

