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

web.cpp

Member functions for web points.

*************************************************************************/



#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>

#include "rtcommon.h"
#include "db_point.h"
#include "db.h"
#include "arg.h"
#include "config.h"

/**********************************************************************/

static int gen_image_at(char *bp, int bp_size, int top, int left, char *name)
{
  int n = snprintf(bp, bp_size, "<div style=\"position: absolute; top: %dpx; left: %dpx;\"> <img src=\"%s\">\n</div>", 
             top, left, name);
  return n;
}

/**********************************************************************/

static void gen_bar(char *bp, int bp_size, 
             char *base, char *half_on, char *half_off, 
             int top, int left, int length,
             double eu_lo, double eu_hi, double pv) 
{
  double fraction = (pv - eu_lo) / (eu_hi - eu_lo);
  int sz = gen_image_at(bp, bp_size, top, left, base);
  bp += sz;
  bp_size -= sz;
  //printf("%0.2lf ", fraction);
  if (fraction < 0.0)
  {
    fraction = 0.0;
  }
  else if (fraction > 1.0)
  {
    fraction = 1.0;
  }
  //printf("%0.2lf ", fraction);
  int pixels = int(fraction * double(length)); 
  //printf("%d ", pixels);
  if (fraction < 0.5)
  {
    int btop =  top + (length / 2) - pixels; 
    //printf("%d %d %d %d %d\n ", btop, left, top, length, pixels);
    gen_image_at(bp, bp_size, btop, left, half_off);
  }
  else 
  {
    int btop =  top + length - pixels;
    //printf("%d %d\n ", btop, left);
    gen_image_at(bp, bp_size, btop, left, half_on);
  }
}

/**********************************************************************/

void web_point_t::update(void)
{
  /* Update the web point. */

  double now = db->get_time();
  if ((now - last_time) >= update_interval)
  {
    last_time = now;
    //printf("Updating %s ..........\n", this->tag);
    this->update_file_and_write();
  }
}

/*********************************************************************/

web_point_t **web_point_t::read(int *cnt, const char *home_dir)
{
  int max_points = 20;
  web_point_t **web_points =
	(web_point_t **) malloc(max_points * sizeof(web_point_t*));
  MALLOC_CHECK(web_points);

  *cnt = 0;

  char path[200];
  safe_strcpy(path, home_dir, sizeof(path));
  safe_strcat(path, "/dbfiles/web.dat", sizeof(path));
  FILE *fp = fopen(path, "r");
  if (fp == NULL)
  {
    printf("Can't open: %s\n", path);
    return NULL;
  }
  char line[300];
  int count = 0;

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
    else if (argc != 5)
    {
      printf("%s: wrong number of args for web point: line %d\n", path, i+1);
      continue;
    }

    //printf(line);
    web_point_t *p = new web_point_t;

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

    p->update_interval = atof(argv[2]);

    const char *html_home = react_config->get_config("htmlhome");
    const char *template_home = react_config->get_config("templatehome");
    if (html_home == NULL)
    {
      //p->dest_file = strdup(argv[4]);
      safe_strcpy(p->dest_file, argv[4], sizeof(p->dest_file));
    }
    else
    {
      char stmp[200];
      snprintf(stmp, sizeof(stmp), "%s/%s", html_home, argv[4]);
      //p->dest_file = strdup(stmp);
      safe_strcpy(p->dest_file, stmp, sizeof(p->dest_file));
    }

    if (template_home == NULL)
    {
      //p->source_file = strdup(argv[3]);
      safe_strcpy(p->source_file, argv[3], sizeof(p->source_file));
    }
    else
    {
      char stmp[200];
      snprintf(stmp, sizeof(stmp), "%s/%s",template_home, argv[3]);
      //p->source_file = strdup(stmp);
      safe_strcpy(p->source_file, stmp, sizeof(p->source_file));
    }

    p->pv = false;
    safe_strcpy(p->hi_desc, "True", sizeof(p->hi_desc));
    safe_strcpy(p->lo_desc, "False", sizeof(p->lo_desc));
    p->pv_string = p->lo_desc;
    p->last_time = 0.0;

    p->read_file();

    web_points[count] = p;
    count++;
    if (count > max_points)
    {
      max_points += 10;
      web_points = (web_point_t **) realloc(web_points,
		  max_points * sizeof(web_point_t*));
      MALLOC_CHECK(web_points);
    }
  }

  *cnt = count;

  if (*cnt == 0)
  {
    free(web_points);
    return NULL;
  }
  return web_points;
}

/*************************************************************************/

void web_point_t::read_file(void)
{
  FILE *fp_html = fopen(source_file, "r");
  if (fp_html == NULL)
  {
    printf("Can't open file: %s\n", source_file);
    drefs = NULL;
    irefs = NULL;
    file_buf = NULL;
    file_size = 0;
    return;
  }
  struct stat buf;
  //int fstat(int filedes, struct stat *buf);
  fstat(fileno(fp_html), &buf);

  file_size = buf.st_size;
  file_buf = (char *) malloc(file_size+1);
  // size_t fread(void *ptr, size_t size, size_t nmemb, FILE *stream);
  fread(file_buf, file_size, 1, fp_html);
  fclose(fp_html);
  file_buf[file_size] = '\0';

  drefs = NULL;
  arefs = NULL;
  irefs = NULL;

  for (int i=0; i < file_size; i++)
  {
    if (0 == strncasecmp(&file_buf[i], "$$$$TIME(", 9))
    {
      time_location = &file_buf[i];
      int j;
      for (j = i; file_buf[j] != ')'; j++);
      time_size = j - i + 1;
      //char *time_location;
      //int time_size;
    }
    else if (0 == strncasecmp(&file_buf[i], "$$$$INT(", 8))
    {
      int_ref_t *newiref = new int_ref_t;
      newiref->next = irefs;
      irefs = newiref;
      newiref->str = &file_buf[i];
      int j;
      for (j = i; file_buf[j] != ')'; j++);
      newiref->size = j - i + 1;
      int len = newiref->size-8;
      char *tmp = new char[len+1];
      strncpy(tmp, file_buf + i + 8, len);
      tmp[len] = '\0';
      int argc;
      char *argv[20];
      argc = get_delim_args(tmp, argv, '|', 20);
      for (int m=0; m < argc; m++)
      {
        printf("argv[%d]: %s\n", m, argv[m]);
      }
      rtrim(argv[0]);
      ltrim(argv[0]);
      rtrim(argv[1]);
      ltrim(argv[1]);
      db_point_t *db_point;
      db_point = db->get_db_point(argv[0]);
      if (db_point == NULL)
      {
        newiref->ip = NULL;
        newiref->nvals = 0;
        printf("%s - NOT a database point: %s\n", source_file, argv[0]);
        continue;
      }
      else if (db_point->pv_type() != INTEGER_VALUE)
      {
        newiref->ip = NULL;
        newiref->nvals = 0;
        printf("%s - NOT an integer point: %s\n", source_file, argv[0]);
        continue;
      }
      else
      {
        newiref->ip = (int_t *) db_point;
      }
      newiref->default_str = argv[1];
      newiref->nvals = 0;
      for (int k=2; k < (argc - 1); k += 2)
      {
        rtrim(argv[k]);
        ltrim(argv[k]);
        rtrim(argv[k+1]);
        ltrim(argv[k+1]);
        if (argv[k+1][strlen(argv[k+1])-1] == ')')
        {
          argv[k+1][strlen(argv[k+1])-1] = '\0';
        }
        rtrim(argv[k+1]);
        newiref->values[newiref->nvals] = atol(argv[k]);
        newiref->strings[newiref->nvals] = argv[k+1];
        newiref->nvals++;
      }

      printf("Integer point: %s, default value: %s\n",
           newiref->ip->tag, newiref->default_str);
      for (int k=0; k < newiref->nvals; k++)
      {
        printf("Value: %d, string: %s\n",
            newiref->values[k], newiref->strings[k]);
      }
    }
    else if (0 == strncasecmp(&file_buf[i], "$$$$DI(", 7))
    {
      discrete_ref_t *r = new discrete_ref_t;
      r->next = drefs;
      drefs = r;
      r->str = &file_buf[i];
      int j;
      for (j = i; file_buf[j] != ')'; j++);
      r->size = j - i + 1;
      int len = r->size-7;
      char *tmp = new char[len+1];
      strncpy(tmp, file_buf + i + 7, len);
      tmp[len] = '\0';
      int argc;
      char *argv[10];
      argc = get_delim_args(tmp, argv, '|', 10);
      if (argc == 3)
      {
        rtrim(argv[0]);
        ltrim(argv[0]);
        rtrim(argv[1]);
        ltrim(argv[1]);
        rtrim(argv[2]);
        ltrim(argv[2]);
        if (argv[2][strlen(argv[2])-1] == ')')
        {
          argv[2][strlen(argv[2])-1] = '\0';
        }
        rtrim(argv[2]);
        r->true_string = argv[1];
        r->false_string = argv[2];
        //r->dp = NULL;

        db_point_t *db_point;
        db_point = db->get_db_point(argv[0]);
        if (db_point == NULL)
        {
          r->dp = NULL;
          printf("%s - NOT a database point: %s\n", source_file, argv[0]);
        }
        else if (db_point->pv_type() != DISCRETE_VALUE)
        {
          r->dp = NULL;
          printf("%s - NOT a discrete point: %s\n", source_file, argv[0]);
        }
        else
        {
          r->dp = (discrete_point_t *) db_point;
        }
      }
      else
      {
        r->true_string = "255,255,153";
        r->false_string = "192,192,192";
        r->dp = NULL;
      }
    }
    else if (0 == strncasecmp(&file_buf[i], "$$$$AI(", 7))
    {
      analog_ref_t *r = new analog_ref_t;
      r->next = arefs;
      arefs = r;
      r->str = &file_buf[i];
      r->is_bar = false;
      int j;
      for (j = i; file_buf[j] != ')'; j++);
      r->size = j - i + 1;
      int len = r->size-7;
      char *tmp = new char[len+1];
      strncpy(tmp, file_buf + i + 7, len);
      tmp[len] = '\0';
      int argc;
      char *argv[10];
      printf("%s\n", tmp);
      argc = get_delim_args(tmp, argv, '|', 10);
      if ((argc == 1) || (argc == 4))
      {
        rtrim(argv[0]);
        ltrim(argv[0]);
        if (argv[argc-1][strlen(argv[argc-1])-1] == ')')
        {
          argv[argc-1][strlen(argv[argc-1])-1] = '\0';
        }
        rtrim(argv[0]);

        db_point_t *db_point;
        db_point = db->get_db_point(argv[0]);
        if (db_point == NULL)
        {
          r->ap = NULL;
          printf("%s - NOT a database point: %s\n", source_file, argv[0]);
        }
        else if (db_point->pv_type() != ANALOG_VALUE)
        {
          r->ap = NULL;
          printf("%s - NOT an analog point: %s\n", source_file, argv[0]);
        }
        else
        {
          r->ap = (analog_point_t *) db_point;
          if (argc == 4)
          {
            rtrim(argv[1]);
            ltrim(argv[1]);
            rtrim(argv[2]);
            ltrim(argv[2]);
            rtrim(argv[3]);
            ltrim(argv[3]);
            r->bar = strdup(argv[1]);
            r->half_off = strdup(argv[2]);
            r->half_on = strdup(argv[3]);
            r->is_bar = true;
            printf("Bar: %s, %s, %s\n", r->bar, r->half_off, r->half_on);
          }
        }
      }
      else
      {
        r->ap = NULL;
      }
    }
  }
}

/********************************************************************/

void web_point_t::subst_string(char *sdest, char *snew, int len)
{
  /* This function copies at most len characters into sdest from snew, and then space pads */
  bool end = false;
  for (int i=0; i < len; i++)
  {
    if (end)
    {
      /* ok, space pad if we have extra space */
      sdest[i] = ' ';
      continue;
    }
    if (snew[i] == '\0')
    {
      end = true;
      sdest[i] = ' ';
      continue;
    }
    sdest[i] = snew[i];
  }
}

/***
static bool test[12] =
  {false, false, false, false,false, false, false, false,false, false, false, false};
static int tcount = 0;
***/

/********************************************************************/

void web_point_t::update_file_and_write(void)
{

  if (time_location != NULL)
  {
    char datestr[100];
    time_t t;
    t = time(NULL);
    struct tm *mytm;
    mytm = localtime(&t);
    strftime(datestr, sizeof(datestr), "%d/%m/%y %T", mytm);
    //strftime(timestr, sizeof(timestr), "%T", mytm);

    subst_string(time_location, datestr, time_size);
  }

  int_ref_t *ir;
  ir = irefs;
  while (ir != NULL)
  {
    int intval = 0;
    if (ir->ip != NULL)
    {
      intval = ir->ip->get_pv();
    }
    else
    {
      intval = 0;
    }
    char *src = NULL;
    for (int i=0; i < ir->nvals; i++)
    {
      if (intval == ir->values[i])
      {
        src = ir->strings[i];
      }
    }
    if (src == NULL)
    {
      src = ir->default_str;
    }
    subst_string(ir->str, src, ir->size);
    ir = ir->next;
  }

  discrete_ref_t *dr;
  dr = drefs;
  while (dr != NULL)
  {
    bool current_val;
    if (dr->dp != NULL)
    {
      //printf("val = %s\n", (current_val)?"----T----":"F");
      current_val = dr->dp->get_pv();
    }
    else
    {
      current_val = false;
    }
    char *src;
    if (current_val)
    {
      src = dr->true_string;
    }
    else
    {
      src = dr->false_string;
    }
    subst_string(dr->str, src, dr->size);
    dr = dr->next;
  }

  analog_ref_t *ar;
  ar = arefs;
  while (ar != NULL)
  {
    double current_aval;
    if (ar->ap != NULL)
    {
      //printf("val = %s\n", (current_aval)?"----T----":"F");
      current_aval = ar->ap->get_pv();
    }
    else
    {
      current_aval = 0.0;
    }
    if (ar->is_bar)
    {
      char buf[220];
      int top = 30;
      int left = 710;
      int length = 400;
      gen_bar(buf, sizeof(buf), 
             ar->bar, ar->half_on, ar->half_off, 
             top, left, length,
             0, 500, current_aval); 
      //printf("%s\n", buf);
      subst_string(ar->str, buf, ar->size);
    }
    else
    {
      char abuf[15];
      snprintf(abuf, sizeof(abuf), "%0.2lf", current_aval);
      subst_string(ar->str, abuf, ar->size);
    }
    
    ar = ar->next;
  }


  FILE *fp = fopen(dest_file, "w");
  if (fp == NULL)
  {
    printf("Can't open file: %s\n", dest_file);
    return;
  }
  fwrite(file_buf, file_size, 1, fp);
  fclose(fp);
}

/********************************************************************/
