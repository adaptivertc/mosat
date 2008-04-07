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

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "rtcommon.h"
#include "arg.h"
#include "tpconfig.h"

tp_config_t tpconfig;

/**********************************************************************/

tp_config_t::tp_config_t(void)
{
  n_keys = 0;
  cur_key = 0;
  printf("Init done, %d, %d\n", n_keys, cur_key);
}

/**********************************************************************/

int tp_config_t::get_int(const char *aKey)
{
  for (int i=0; i < n_keys; i++)
  {
    if (0 == strcasecmp(aKey, key[i]))
    {
      return atol(val[i]);
    }
  }
  printf("****** Error: key not found: %s\n", aKey);
  return -1;
}

/**********************************************************************/

double tp_config_t::get_double(const char *aKey)
{
  for (int i=0; i < n_keys; i++)
  {
    if (0 == strcasecmp(aKey, key[i]))
    {
      return atof(val[i]);
    }
  }
  printf("****** Error: key not found: %s\n", aKey);
  return 0.0;
}

/**********************************************************************/

const char *tp_config_t::get_config(const char *aKey)
{
  for (int i=0; i < n_keys; i++)
  {
    if (0 == strcasecmp(aKey, key[i]))
    {
      return val[i];
    }
  }
  printf("****** Error: key not found: %s\n", aKey);
  return NULL;
}

/**********************************************************************/

void tp_config_t::read_file(const char *fname)
{
  printf("Reading config file: %s\n", fname);
  FILE *fp = fopen(fname, "r");
  if (fp == NULL)
  {
    printf("Can't open %s\n", fname);
    return;
  }
  char line[300];

  for (int i=0; NULL != fgets(line, sizeof(line), fp); i++)
  {
    char tmp[300];
    int argc;
    char *argv[25];

    safe_strcpy(tmp, (const char*) line, sizeof(tmp));
    argc = get_delim_args(tmp, argv, ',', 25);
    if (argc == 0)
    {
      continue;
    }
    else if (argv[0][0] == '#')
    {
      continue;
    }
    else if (argc != 2)
    {
      printf("Error in %s: %s\n", fname, line);
      continue;
    }
    rtrim(argv[1]);
    key[n_keys] = strdup(argv[0]);
    val[n_keys] = strdup(argv[1]);
    printf("key: %s, val: %s\n", key[n_keys], val[n_keys]);
    n_keys++;
  }
}

/**********************************************************************/

/***
main(int argc, char *argv[])
{
  tp_config_t cfg;
  cfg.read_file("tpconfig.txt");
  const char *val;
  val = cfg.get_config("SECTIONS_FILE");
  if (val != NULL)
  {
    printf("found it: %s\n", val);
  }
  val = cfg.get_config("TIMETABLE_FILE");
  if (val != NULL)
  {
    printf("found it: %s\n", val);
  }
}

**/
