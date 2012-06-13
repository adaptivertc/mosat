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

/*********
Ai.c

Code for the analog input member functions.

*************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include <ncurses.h>
#include <math.h>
#include <malloc.h>
#include <float.h>

#include "rtcommon.h"
#include "reactpoint.h"
#include "db_point.h"
#include "db.h"
#include "arg.h"

/***
class ai_point_factory_t : public db_point_factory_t
{
public:
  db_point_t *new_point (int argc, char *argv[], char *err, int esz)
    {return ai_point_t::read_one(argc, argv, err, esz);};
  void delete_point(db_point_t *dbp) {delete (ai_point_t *)dbp;};
  const char *get_config_name(void) {return "ai.dat";};
  virtual const char * long_description(void) {return "Analog Input";};
  virtual const char * short_description(void) {return "Analog Input";};
  virtual const char * abbreviation(void) {return "AI";};
  virtual bool is_analog_input() {return true;};
  virtual bool is_discrete_input() {return false;};
  virtual bool is_integer_input() {return false;};
  virtual bool is_analog_output() {return false;};
  virtual bool is_discrete_output() {return false;};
  virtual bool is_integer_output() {return false;};
  virtual bool needs_update() {return false;};
  virtual bool has_analog_val() {return true;};
  virtual bool has_discrete_val() {return false;};
  virtual bool has_int_val() {return false;};
};
**/

/********************************************************************/

/**
extern "C" db_point_factory_t *get_db_factory(react_base_t *r)
{
  return new ai_point_factory_t;
}
**/

/*********************************************************************/

void ai_point_t::update(double new_raw_value)
{
  /* This routine updates the analog input point using the given raw
     value. */
  point_lock_t l(&this->point_lock, tag);
  //printf("Entering ai update\n");

  raw_value = new_raw_value;
  pv_last = pv;

  if (raw_value == FLT_MAX)
  {
    pv = FLT_MAX;
  }
  else
  {
    pv = ((raw_value - raw_lo) * conversion_factor) + eu_lo;

    //printf("%s, pv = %lf\n", tag, pv);
    if (pv <= zero_cutoff)
    {
      pv = 0.0;
    }
  //printf("pv = %lf\n", pv);
  }
  //printf("%s, raw val = %lf, pv = %lf\n", tag, new_raw_value, pv);
  check_alarms();
  display_pv();
  //printf("%s.pv = %lf\n", tag, pv);
  //printf("leaving ai update\n");
}

/*************************************************************************/

void ai_point_t::set_conversion(double rl, double rh, double eul, double euh)
{

  /* This sets up the conversion for the given analog input point
     using the parameters passed */

  raw_lo = rl;
  raw_hi = rh;
  eu_lo = eul;
  eu_hi = euh;
  set_conversion();
}

/*************************************************************************/

void ai_point_t::set_conversion(void)
{
  /* This procedure sets up the conversion for the given analog input
     point using the internal conversion values. */

  if ((raw_hi - raw_lo) == 0)
  {
    conversion_factor = 0.0;
  }
  else
  {
    conversion_factor = (eu_hi - eu_lo) / (raw_hi - raw_lo);
  }
}

/************************************************************************/

void ai_point_t::init_values(void)
{
  lo_alarm_shutdown = false;
  hi_alarm_shutdown = false;
  set_conversion();
  set_format();
  pv = 0;
  json_str = NULL;
}

/************************************************************************/

const char *ai_point_t::get_config_json(void)
{
  point_lock_t l(&this->point_lock, tag);

  if (json_str == NULL)
  {
     asprintf(&json_str, "{\"tag\":\"%s\",\"description\":\"%s\",\"eu\":\"%s\",\"driver\":%d,\"card\":%d,\"channel\":%d,\"eu_lo\":%lf,\"eu_hi\":%lf,\"raw_lo\":%lf,\"raw_hi\":%lf,\"decimal_places\":%d,\"zero_cutoff\":%lf,\"lo_alarm\":%lf,\"lo_caution\":%lf,\"hi_caution\":%lf,\"hi_alarm\":%lf,\"deadband\":%lf,\"lo_alarm_enable\":%s,\"lo_caution_enable\":%s,\"hi_caution_enable\":%s,\"hi_alarm_enable\":%s,\"scale_lo\":%lf,\"scale_hi\":%lf}",
         this->tag, 
         this->description, 
         this->eu, 
         this->driver,
         this->card,
         this->channel,
         this->eu_lo,
         this->eu_hi,
         this->raw_lo,
         this->raw_hi, 
         this->decimal_places,
         this->zero_cutoff,
         this->lo_alarm,
         this->lo_caution,
         this->hi_caution,
         this->hi_alarm,
         this->deadband,
         this->lo_alarm_enable?"true":"false",
         this->lo_caution_enable?"true":"false",
         this->hi_caution_enable?"true":"false",
         this->hi_alarm_enable?"true":"false",
         this->scale_lo,
         this->scale_hi
      );  
           /**
Tag: LevelDescription: Level of first tankEU Label: cm.Driver: 1Card: 0Channel: 231EU Low: 0EU High: 600Raw Low: 0Raw High: 10Decimal Places: 0Zero Cutoff: 2
***/
  }
  return json_str;
}

/************************************************************************/

db_point_t *ai_point_t::read_one(int argc, char *argv[], char *err, int esz)
{
  if (argc != 23)
  {
    snprintf(err, esz, "Wrong number of args for ai");
    return NULL;
  }
  ai_point_t *ai = new ai_point_t;
  safe_strcpy(ai->tag, argv[0], sizeof(ai->tag));
  //printf("Tag = %s\n", ai->tag);
  safe_strcpy(ai->description, argv[1], sizeof(ai->description));
  safe_strcpy(ai->eu, argv[2], sizeof(ai->eu));
  ai->driver = atol(argv[3]);
  ai->card = atol(argv[4]);
  ai->channel = atol(argv[5]);
  ai->eu_lo = atof(argv[6]);
  ai->eu_hi = atof(argv[7]);
  ai->raw_lo = atof(argv[8]);
  ai->raw_hi = atof(argv[9]);
  ai->decimal_places = atol(argv[10]);
  ai->zero_cutoff = atof(argv[11]);
  ai->lo_alarm = atof(argv[12]);
  ai->lo_caution = atof(argv[13]);
  ai->hi_caution = atof(argv[14]);
  ai->hi_alarm = atof(argv[15]);
  ai->deadband = atof(argv[16]);
  ai->lo_alarm_enable = (argv[17][0] == '1');
  ai->lo_caution_enable = (argv[18][0] == '1');
  ai->hi_caution_enable = (argv[19][0] == '1');
  ai->hi_alarm_enable = (argv[20][0] == '1');
  ai->scale_lo = atof(argv[21]);
  ai->scale_hi = atof(argv[22]);

  ai->init_values();

  return ai;
}

/************************************************************************

//ai_point_t **read_ai_points(int *cnt, const char *home_dir)
ai_point_t **ai_point_t::read(int *cnt, const char *home_dir)
{
  int max_points = 50;
  ai_point_t **ai_points =
	(ai_point_t **) malloc(max_points * sizeof(ai_point_t*));
  MALLOC_CHECK(ai_points);

  *cnt = 0;
  char path[200];
  safe_strcpy(path, home_dir, sizeof(path));
  safe_strcat(path, "/dbfiles/ai.dat", sizeof(path));
  FILE *fp = fopen(path, "r");
  if (fp == NULL)
  {
    printf("Can't open %s", path);
    return NULL;
  }
  char line[300];

  for (int i=0; NULL != fgets(line, sizeof(line), fp); i++)
  {
    char tmp[300];
    int argc;
    char *argv[25];

    //safe_strcpy(line, "AI1|Analog Input 1|PSI|0|0|1|0|100|0|4095|1|5|10|20|80|90|5|0|0|0|0|");
    safe_strcpy(tmp, line, sizeof(tmp));
    argc = get_delim_args(tmp, argv, '|', 25);
    if (argc == 0)
    {
      continue;
    }
    else if (argv[0][0] == '#')
    {
      continue;
    }
    //else if (argc != 21)
    //{
     // printf("%s: Wrong number of args, line %d", path, i+1);
      //continue;
    //}
    //printf(line);
    //ai_point_t *ai = new ai_point_t;
    //(*cnt)++;

    //AI1|Analog Input 1|PSI|0|0|0|0|100|0|4095|5|0|10|20|80|90|5|1|1|1|1|
    //Tag
    //Description
    //Engineering Units Label
    //Driver Number
    //Card
    //Channel
    //Engineering Units Low
    //Engineering Units High
    //Raw Value Low
    //Raw Value High
    //Decimal Places
    //Zero Cutoff
    //Alarm Low
    //Caution Low
    //Caution High
    //Alarm High
    //Alarm Deadband
    //Alarm Low Enable
    //Caution Low Enable
    //Caution High Enable
    //Alarm High Enable

    char errbuf[100];
    ai_points[*cnt] = ai_point_t::read_one(argc, argv, errbuf, sizeof(errbuf));
    if (ai_points[*cnt] == NULL)
    {
      printf("%s:%d %s\n", path, i+1, errbuf);
      continue;
    }
    printf("AI[%d].tag = '%s'\n", *cnt, ai_points[*cnt]->tag);
    printf(line);
    (*cnt)++;
    if (*cnt >= max_points)
    {
      max_points += 25;
      ai_points = (ai_point_t **) realloc(ai_points,
	      max_points * sizeof(ai_point_t*));
      MALLOC_CHECK(ai_points);
    }
    //printf("AI[%d].tag = '%s'\n", i, ai->tag);
  }
  if (*cnt == 0)
  {
    free(ai_points);
    return NULL;
  }
  return ai_points;
}

************************************************************************/

ai_point_t *ai_point_t::create_one(int argc, char *argv[], char *err, int esz)
{
  ai_point_t *objp;
  objp = new ai_point_t;
  if (objp == NULL)
  {
    perror("new ai_point_t");
    snprintf(err, esz, "call to 'new' failed");
    return NULL;
  }
  if (argc != 23)
  {
    snprintf(err, esz, "Wrong number of args for ai: %d, should be 23", argc);
    return NULL;
  }
  snprintf(objp->tag, sizeof(objp->tag), "%s", argv[0]);
  snprintf(objp->description, sizeof(objp->description), "%s", argv[1]);
  snprintf(objp->eu, sizeof(objp->eu), "%s", argv[2]);
  objp->driver = atoi(argv[3]);
  objp->card = atoi(argv[4]);
  objp->channel = atoi(argv[5]);
  objp->eu_lo = atof(argv[6]);
  objp->eu_hi = atof(argv[7]);
  objp->raw_lo = atof(argv[8]);
  objp->raw_hi = atof(argv[9]);
  objp->decimal_places = atoi(argv[10]);
  objp->zero_cutoff = atof(argv[11]);
  objp->lo_alarm = atof(argv[12]);
  objp->lo_caution = atof(argv[13]);
  objp->hi_caution = atof(argv[14]);
  objp->hi_alarm = atof(argv[15]);
  objp->deadband = atof(argv[16]);
  objp->lo_alarm_enable = (argv[17][0] == '1') ||
             (argv[17][0] == 'T') ||
             (argv[17][0] == 't');
  objp->lo_caution_enable = (argv[18][0] == '1') ||
             (argv[18][0] == 'T') ||
             (argv[18][0] == 't');
  objp->hi_caution_enable = (argv[19][0] == '1') ||
             (argv[19][0] == 'T') ||
             (argv[19][0] == 't');
  objp->hi_alarm_enable = (argv[20][0] == '1') ||
             (argv[20][0] == 'T') ||
             (argv[20][0] == 't');
  objp->scale_lo = atof(argv[21]);
  objp->scale_hi = atof(argv[22]);
  return objp;
}
ai_point_t **ai_point_t::read(int *cnt, const char *home_dir)
{
 int max_points = 100;
 ai_point_t **dbps =
 (ai_point_t **) malloc(max_points * sizeof(ai_point_t*));
 MALLOC_CHECK(dbps);
 *cnt = 0;
 int count = 0;
 char path[500];
 snprintf(path, sizeof(path), "%s/dbfiles/ai.dat", home_dir);
 FILE *fp = fopen(path, "r");
 if (fp == NULL)
 {
   logfile->perror(path);
   logfile->vprint("Can't open %s", path);
   return NULL;
 }
 char line[500];
 for (int i=0; NULL != fgets(line, sizeof(line), fp); i++)
 {
   char tmp[500];
   int argc;
   char *argv[50];
   ltrim(line);
   rtrim(line);
   snprintf(tmp, sizeof(tmp), "%s", line);
   argc = get_delim_args(tmp, argv, '|', 50);
   if (argc == 0)
   {
     continue;
   }
   else if (argv[0][0] == '#')
   {
     continue;
   }
   if (count >= max_points)
   {
     max_points += 50;
     int new_size = max_points * sizeof(ai_point_t*);
     logfile->vprint("Reallocating: new size = %d\n", new_size);
     dbps = (ai_point_t **) realloc(dbps, new_size);
     MALLOC_CHECK(dbps);
   }
   char errbuf[100];
   dbps[count] = ai_point_t::create_one(argc, argv, errbuf, sizeof(errbuf));
   if (dbps[count] == NULL)
   {
     logfile->vprint("%s:%d\n", path, i+1);
     logfile->vprint("%s\n", errbuf);
     continue;
   }
   logfile->vprint("%s\n", line);
   count++;
 }
 if (count == 0)
 {
   free(dbps);
   return NULL;
 }
 *cnt = count;
 return dbps;
}


/***************************/

