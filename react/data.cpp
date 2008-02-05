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

/**********************************************************************/

void data_point_t::write_to_file(void)
{
  if (count == 0)
  {
    return;
  }
  char fname[200];
  snprintf(fname, sizeof(fname), "%s/dbout/%s", db->get_home_dir(), file_name);
  FILE *fp = fopen(fname, "w");
  if (fp == NULL)
  {
    printf("Can not open output file: %s\n", fname);
    return;
  }
  printf("Writing: %s . . .\n", fname);
  for (int i=0; i < count; i++)
  {
    fprintf(fp, "%lf", double(i) / db->get_sample_rate());
    for (int j=0; j < num_points; j++)
    {
      fprintf(fp, "\t%lf", data[j][i]);
    }
    fprintf(fp, "\n");
  }
  fclose(fp);
}

/**********************************************************************/

void data_point_t::update(void)
{
  if (!collecting)
  {
    return;
  }

  if (count >= max_samples)
  {
    collecting = false;
    return;
  }

  for (int i=0; i < num_points; i++)
  {
    if (analog_points[i] != NULL)
    {
      //printf("%s %d, %d %d\n", __FILE__, __LINE__, i, count);
      data[i][count] = analog_points[i]->get_pv();
      //printf("%s %d\n", __FILE__, __LINE__);
    }
    else
    {
      data[count][i] = 0.0;
    }
  }
  count++;
}

/**********************************************************************/

void data_point_t::start(void)
{
  collecting = true;
  count = 0;
}

/**********************************************************************/

void data_point_t::stop(void)
{
  collecting = false;
}

/*********************************************************************/

data_point_t **data_point_t::read(int *cnt, const char *home_dir)
{
  int max_points = 50;
  data_point_t **data_points =
	(data_point_t **) malloc((max_points + 1) * sizeof(data_point_t*));
  MALLOC_CHECK(data_points);

  *cnt = 0;
  int count = 0;

  char path[200];
  safe_strcpy(path, home_dir, sizeof(path));
  safe_strcat(path, "/dbfiles/data.dat", sizeof(path));
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
    else if (argc < 5)
    {
      printf("%s: Wrong number of args, line %d\n", path, i+1);
      continue;
    }

    printf(line);
    data_point_t *p = new data_point_t;

    /*****
    Tag
    Description
    File Name
    Max Time
    Tag 1
    Tag 2
    . . .
    Tag N
    ******/

    safe_strcpy(p->tag, (const char*) argv[0], sizeof(p->tag));
    safe_strcpy(p->description, (const char*) argv[1], sizeof(p->description));
    safe_strcpy(p->file_name, (const char*) argv[2], sizeof(p->file_name));
    p->max_time = atol(argv[3]);
    p->max_samples = int(p->max_time * db->get_sample_rate());
    printf("MaxSamples = %d MaxTime = %d\n", p->max_samples, p->max_time);
    p->num_points = argc - 4;

    p->analog_points = new analog_point_t *[p->num_points + 1];
    //(analog_point_t **) malloc(p->num_points * sizeof(analog_point_t*));
    p->data = new double *[p->num_points + 1];
    //(analog_point_t **) malloc(p->num_points * sizeof(analog_point_t*));
    //MALLOC_CHECK(p->data);

    for (int j=4; j < argc; j++)
    {
      char temp_tag[50];
      db_point_t *db_point;
      safe_strcpy(temp_tag, (const char*) argv[j], sizeof(temp_tag));
      rtrim(temp_tag);
      db_point = db->get_db_point(temp_tag);
      if ((db_point == NULL) || (db_point->pv_type() != ANALOG_VALUE))
      {
        p->analog_points[j-4] = NULL;
        printf("Bad analog point: %s\n", temp_tag);
      }
      else
      {
        p->analog_points[j-4] = (analog_point_t *) db_point;
      }
      printf("allocating space for  %d samples\n", p->max_samples);
      p->data[j-4] = new double[p->max_samples + 1];
      MALLOC_CHECK(p->data[j-4]);
    }


    p->collecting = false;
    p->count = 0;
    data_points[count] = p;
    count++;
    if (count > max_points)
    {
      max_points += 10;
      data_points = (data_point_t **) realloc(data_points,
		  max_points * sizeof(data_point_t*));
      MALLOC_CHECK(data_points);
    }
  }

  if (count == 0)
  {
    free(data_points);
    return NULL;
  }
  *cnt = count;
  return data_points;

}

/*************************************************************************/

