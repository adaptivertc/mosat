/************************************************************************
This is software to monitor and/or control trains.
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
#include <time.h>
#include <stdlib.h>
#include <unistd.h>

#include "msg_queue.h"
#include "tcontrol.h"
#include "event_alg.h"
#include "dc_evalg.h"

#include "rtcommon.h"
#include "arg.h"


display_dist_t::display_dist_t(int a_x1, int a_x2,
                               int a_y1, int a_y2, int n_sects)
{
  x1 = a_x1;
  x2 = a_x2;
  y1 = a_y1;
  y2 = a_y2;
  n_sections = n_sects;
  return_start = n_sects / 2;
  n_squares = 5 * return_start + 1;
  total_x_pixels = x2 - x1 + 1;
  //printf("x1 = %d, x2 = %d, y1 = %d, y2 = %d\n", x1, x2, y1, y2 );
  //printf("return_start = %d, n_squares = %d, total_x_pixels = %d\n", return_start, n_squares, total_x_pixels);
}

void display_dist_t::calc_xy(int section, double fraction, int *x, int *y)
{
  int extra;
  int square;

  if (fraction > 0.75)
  {
   extra = 4;
  }
  else if (fraction > 0.50)
  {
    extra = 3;
  }
  else if (fraction > 0.25)
  {
    extra = 2;
  }
  else if (fraction > 0.01)
  {
    extra = 1;
  }
  else
  {
    extra = 0;
  }

  if (section >= return_start)
  {
    *y = y1;
    square = (5 * (section - return_start)) + extra;
    square = n_squares - square - 1;
  }
  else
  {
    square = (5 * section) + extra;
    *y = y2;
  }
  *x = x1 + int(double(total_x_pixels) * (double(square) / double(n_squares-1)));
  //printf("s = %d, fr = %0.2lf, ex = %d, sq = %d, x = %d, y = %d\n",
  //  section, fraction, extra, square, *x, *y);
}

/********************************************************/

void display_alg_t::read_sections(char *fname)
{

// image size
  dd = new display_dist_t(26, 967, 45, 72, 36);
//  dd = new display_dist_t(54, 2001, 94, 153, 36);
  n_trains = 0;
  train_number = 1;
  fp = fopen("line1.html", "w");
  if (fp == NULL)
  {
    perror("line1.html");
    exit(0);
  }
  fd = fileno(fp);

  table_fp = fopen("line1_table.html", "w");
  if (table_fp == NULL)
  {
    perror("line1_table.html");
    exit(0);
  }
  table_fd = fileno(table_fp);


  //display_dist_t(int a_x1, int a_x2, int a_y1, int a_y2, int n_sections);

  int max = 50;

  char line[300];

  FILE *fp = fopen(fname, "r");
  if (fp == NULL)
  {
    perror(fname);
    exit(0);
  }

  int n_lines = 0;
  int total_time = 0;
  for (int i=0; NULL != fgets(line, sizeof(line), fp); i++)
  {
    char tmp[300];
    int argc;
    char *argv[25];

    //printf("--- %s\n", line);
    safe_strcpy(tmp, line, sizeof(tmp));
    argc = get_delim_args(tmp, argv, '\t', 4);
    if (argc != 4)
    {
      printf("Wrong: %s\n", line);
      continue;
    }

    safe_strcpy(sections[n_lines].station, argv[0], sizeof(sections[n_lines].station));
    sections[n_lines].section_time = atol(argv[1]);
    sections[n_lines].departure_sensor_loc = atol(argv[2]);
    sections[n_lines].arival_sensor_loc = atol(argv[3]);
    sections[n_lines].time_to_start = total_time;
    printf("%20s(%d): %d %d %d %d\n",
      sections[n_lines].station,
      n_lines,
      sections[n_lines].section_time,
      sections[n_lines].departure_sensor_loc,
      sections[n_lines].arival_sensor_loc,
      sections[n_lines].time_to_start );
    total_time += sections[n_lines].section_time;
    if (n_lines != 0) total_time += RT_DWELL_TIME;
    n_lines++;
    if (n_lines >= max)
    {
      break;
    }
  }
  n_sections = n_lines;
}

/*********************************************************/

void display_alg_t::add_train(time_t ts)
{
  for (int i=(n_trains - 1); i >= 0; i--)
  {
    trains[i+1] = trains[i];
  }
  trains[0].section = 0;
  trains[0].num = train_number;
  trains[0].arival_time = ts - sections[0].departure_sensor_loc; // really irrelavent, but should be assigned.
  trains[0].departure_time = ts - sections[0].departure_sensor_loc;
  trains[0].departed = true;
  trains[0].service_entry_time = ts - sections[0].departure_sensor_loc;
  trains[0].seconds_in_section = ts - trains[0].arival_time;
  trains[0].seconds_late = 0;
  trains[0].unexpected = false;
  trains[0].line_location = trains[0].seconds_in_section;

  n_trains++;
  train_number++;
  printf("********** Added train, n_trains = %d\n", n_trains);
}

/*********************************************************/

void display_alg_t::gen_alarms(time_t now)
{
  

}

/*********************************************************/

static int n_pass[] = {
57, 88, 105, 165, 198, 234, 278, 299, 333,
345, 321, 302, 281, 234, 185, 163, 111, 63,
69, 113, 158, 185, 204, 251, 282, 301, 338,
331, 304, 288, 251, 201, 185, 172, 144, 78, 
39, 81, 101, 165, 191, 211, 232, 291, 338,
332, 273, 248, 221, 181, 165, 122, 88, 55 
};

void display_alg_t::gen_table(time_t now)
{

  fprintf(table_fp, "<!DOCTYPE html PUBLIC \"-//W3C//DTD HTML 4.01 Transitional//EN\">\n");
  fprintf(table_fp, "<html>\n");
  fprintf(table_fp, "<head>\n");
  fprintf(table_fp, "<META HTTP-EQUIV=\"refresh\" CONTENT=\"1\">\n");
  fprintf(table_fp, "<meta http-equiv=\"pragma\" content=\"no-cache\">\n");
  fprintf(table_fp, "  <meta content=\"text/html; charset=ISO-8859-1\"\n");
  fprintf(table_fp, " http-equiv=\"content-type\">\n");
  fprintf(table_fp, "  <title>Table de Trenes</title>\n");
  fprintf(table_fp, "</head>\n");
  fprintf(table_fp, "<body style=\"color: rgb(0, 0, 0); background-color: rgb(153, 153, 153);\"\n");
  fprintf(table_fp, " alink=\"#000099\" link=\"#000099\" vlink=\"#990099\">\n");
  fprintf(table_fp, "<table\n");
  fprintf(table_fp, " style=\"background-color: rgb(192, 192, 192); width: 100%; text-align: left;\"\n");
  fprintf(table_fp, " border=\"2\" cellpadding=\"2\" cellspacing=\"2\">\n");
  fprintf(table_fp, "  <tbody>\n");
  fprintf(table_fp, "    <tr>\n");
  fprintf(table_fp, "      <td colspan=\"8\" style=\"vertical-align: top;\"><big><big><big>Linea\n");
  char buf1[30];
  struct tm mytm;
  localtime_r(&now, &mytm);
  strftime(buf1, sizeof(buf1), "%Y-%m-%d %T", &mytm);
  //fprintf(table_fp, "<h1>Linea 1 (%s)</h1>\n", buf1);
  fprintf(table_fp, "1 (%s)</big></big></big><br>\n", buf1);
  fprintf(table_fp, "      </td>\n");
  fprintf(table_fp, "    </tr>\n");
  fprintf(table_fp, "    <tr>\n");
  fprintf(table_fp, "      <th style=\"vertical-align: top;\">Tren ID<br>\n");
  fprintf(table_fp, "      </th>\n");
  fprintf(table_fp, "      <th style=\"vertical-align: top;\">Tiempo<br>\n");
  fprintf(table_fp, "en servicio<br>\n");
  fprintf(table_fp, "      </th>\n");
  fprintf(table_fp, "      <th style=\"vertical-align: top;\">Tarde<br>\n");
  fprintf(table_fp, "segundos<br>\n");
  fprintf(table_fp, "      </th>\n");
  fprintf(table_fp, "      <th style=\"vertical-align: top;\">Porciento<br>\n");
  fprintf(table_fp, "      </th>\n");
  fprintf(table_fp, "      <th style=\"vertical-align: top;\">Tiempo a <br>\n");
  fprintf(table_fp, "Siguente<br>\n");
  fprintf(table_fp, "      </th>\n");
  fprintf(table_fp, "      <th style=\"vertical-align: top;\">Ultimo\n");
  fprintf(table_fp, "Salida<br>\n");
  fprintf(table_fp, "      </th>\n");
  fprintf(table_fp, "      <th style=\"vertical-align: top;\">Timpo <br>\n");
  fprintf(table_fp, "desde Salida<br>\n");
  fprintf(table_fp, "      </th>\n");
  fprintf(table_fp, "      <th style=\"vertical-align: top;\">Numero de<br>Pasajeros<br>\n");
  fprintf(table_fp, "      </th>\n");
  fprintf(table_fp, "    </tr>\n");
  fprintf(table_fp, "    <tr>\n");

  for (int i=0; i < n_trains; i++)
  {
    fprintf(table_fp, "      <td style=\"vertical-align: top;\">%02d<br>\n", trains[i].num);
    fprintf(table_fp, "      </td>\n");
    int secs_in_service = now - trains[i].service_entry_time;
    int mins_in_service = secs_in_service / 60;
    secs_in_service %= 60;
    fprintf(table_fp, "      <td style=\"vertical-align: top;\">%02d:%02d<br>\n", mins_in_service, secs_in_service);
    fprintf(table_fp, "      </td>\n");
    if (trains[i].seconds_late < 0)
    {
      fprintf(table_fp, "      <td style=\"vertical-align: top;\">(%d)<br>\n", -trains[i].seconds_late);
    }
    else
    {
      fprintf(table_fp, "      <td style=\"vertical-align: top;\">%d<br>\n", trains[i].seconds_late);
    }
    fprintf(table_fp, "      </td>\n");
    if (double(trains[i].arival_time - trains[i].service_entry_time) > 60)
    {
      double percent =
            (double(trains[i].seconds_late) / double(trains[i].arival_time - trains[i].service_entry_time)) * 100.0;
      if (percent < 0)
      {
        fprintf(table_fp, "      <td style=\"vertical-align: top;\">(%0.1lf\%)<br>\n", -percent);
      }
      else
      {
        fprintf(table_fp, "      <td style=\"vertical-align: top;\">%0.1lf\%<br>\n", percent);
      }
    }
    else
    {
      fprintf(table_fp, "      <td style=\"vertical-align: top;\">-<br>\n");
    }
    fprintf(table_fp, "      </td>\n");
    int secs_to_next = trains[i].seconds_to_next;
    int mins_to_next = secs_to_next / 60;
    secs_to_next %= 60;
    if (i < (n_trains - 1))
    {

      fprintf(table_fp, "      <td style=\"vertical-align: top;");
      if ( trains[i].seconds_to_next < 180)
      {
         fprintf(table_fp, "background-color: rgb(255, 50, 50);");
      }
      else if ( trains[i].seconds_to_next < 240)
      {
         fprintf(table_fp, "background-color: rgb(255, 255, 50);");
      }

      fprintf(table_fp, "\">%02d:%02d<br>\n", mins_to_next, secs_to_next);
    }
    else
    {
      fprintf(table_fp, "      <td style=\"vertical-align: top;\">-<br>\n");
    }
    fprintf(table_fp, "      </td>\n");
    if (trains[i].section >= (n_sections / 2))
    {
      fprintf(table_fp, "      <td style=\"vertical-align: top; background-color: rgb(153, 153, 255);\">(%s)<br>\n",
                sections[trains[i].section].station);
    }
    else
    {
      fprintf(table_fp, "      <td style=\"vertical-align: top;\">%s<br>\n", sections[trains[i].section].station);
    }
  /***
      <td
 style=\"vertical-align: top; background-color: rgb(153, 153, 255);\">(IslaRaza)<br>
      </td>
  ***/
    fprintf(table_fp, "      </td>\n");

    if (trains[i].seconds_in_section == 0)
    {
      fprintf(table_fp, " <td style=\"vertical-align: top; background-color: rgb(255, 204, 255);\">(en estaci&oacute;n)<br>\n");

      //fprintf(table_fp, "      <td style=\"vertical-align: top;\">en estacion<br>\n");
    }
    else
    {
      fprintf(table_fp, "      <td style=\"vertical-align: top;\">%d<br>\n", trains[i].seconds_in_section);
    }

    fprintf(table_fp, "      </td>\n");
    fprintf(table_fp, "      <td style=\"vertical-align: top;\">%d<br>\n",
           n_pass[trains[i].section]);
    fprintf(table_fp, "      </td>\n");
    fprintf(table_fp, "    </tr>\n");
  }

  fprintf(table_fp, "  </tbody>\n");
  fprintf(table_fp, "</table>\n");
  fprintf(table_fp, "<br>\n");
  fprintf(table_fp, "</body>\n");
  fprintf(table_fp, "</html>\n");
}

/*********************************************************/
void display_alg_t::gen_display(time_t now)
{
  long bytes;

  fseek(fp, 0, SEEK_SET);
  lockf(fd, F_LOCK, 0);
  gen_html(now);
  fflush(fp);
  bytes = ftell(fp);
  ftruncate(fd, bytes);
  fseek(fp, 0, SEEK_SET);
  lockf(fd, F_ULOCK, 0);

  fseek(table_fp, 0, SEEK_SET);
  lockf(table_fd, F_LOCK, 0);
  gen_table(now);
  fflush(table_fp);
  bytes = ftell(table_fp);
  ftruncate(table_fd, bytes);
  fseek(table_fp, 0, SEEK_SET);
  lockf(table_fd, F_ULOCK, 0);
}

/***
void gen_html(FILE *fp, char *background, int top, int left,
             char *iname, int n_image, int itop[], int ileft[],
             int textleft[], int texttop[], char thetext[][25])
***/
void display_alg_t::gen_html(time_t now)
{
  char *iname = "square10x10.png";
  char *background = "Line1_1024.png";
  //char *iname = "Square20x20.png";
  char *i_unexpected = "square_unexpected.png";
  //char *background = "Line1.png";
  int top = 50;
  int left = 0;


  fprintf(fp, "<!DOCTYPE html PUBLIC \"-//W3C//DTD HTML 4.01 Transitional//EN\">\n");
  fprintf(fp, "<html>\n");
  fprintf(fp, "<head>\n");
  fprintf(fp, "<META HTTP-EQUIV=\"refresh\" CONTENT=\"1\">\n");
  fprintf(fp, "<meta http-equiv=\"pragma\" content=\"no-cache\">\n");
  fprintf(fp, "  <meta http-equiv=\"content-type\"\n");
  fprintf(fp, " content=\"text/html; charset=ISO-8859-1\">\n");
  fprintf(fp, "  <title>Linea 1</title>\n");
  fprintf(fp, "</head>\n");
  fprintf(fp, "<body style=\"color: rgb(0, 0, 0); background-color: rgb(204, 204, 204);\"\n");
  fprintf(fp, " link=\"#000099\" vlink=\"#990099\" alink=\"#000099\">\n");
  /**
  time_t now;
  now = time(NULL);
  ***/
  char buf1[30];
  struct tm mytm;
  localtime_r(&now, &mytm);
  strftime(buf1, sizeof(buf1), "%Y-%m-%d %T", &mytm);
  fprintf(fp, "<h1>Linea 1 (%s)</h1>\n", buf1);
  fprintf(fp, "\n");
  fprintf(fp, "\n");
  fprintf(fp, "<div style=\"position:absolute; top:%dpx; left:%dpx;\">\n", top, left);
  fprintf(fp, "        <img SRC=%s>\n", background);
  fprintf(fp, "</div>\n");

  for (int i=0; i < n_trains; i++)
  {
    fprintf(fp, "\n");
    fprintf(fp, "<div style=\"position:absolute; top:%dpx; left:%dpx; z-index:2\">\n",top + trains[i].y,left + trains[i].x);

    if (trains[i].unexpected)
    {
      fprintf(fp, "        <img SRC=%s>\n", i_unexpected);
    }
    else
    {
      fprintf(fp, "        <img SRC=%s>\n", iname);
    }
    fprintf(fp, "</div>\n");
/*****
    fprintf(fp, "<div style=\"position:absolute; top:%dpx; left:%dpx; z-index:2\">\n", top + texttop[i], left + textleft[i]);    fprintf(fp, "        %s\n", thetext[i]);
    fprintf(fp, "</div>\n");
******/
  }


  fprintf(fp, "\n");
  fprintf(fp, "</body>\n");
  fprintf(fp, "</html>\n");
}


/*********************************************************/

void display_alg_t::update_train(time_t ts, int n)
{
  int tx, ty;
  double fr;
  int s = trains[n].section;
  if (!trains[n].departed)
  {
    fr = 0.0;
    trains[n].seconds_in_section = 0;
    trains[n].line_location = sections[s].time_to_start + (ts - trains[n].arival_time);
    //printf("in station\n");
  }
  else
  {
    trains[n].seconds_in_section = ts - trains[n].departure_time;
    int extra = trains[n].seconds_in_section;
    if (trains[n].seconds_in_section > sections[s].section_time)
    {
      extra = sections[s].section_time;
    }
    trains[n].line_location = sections[s].time_to_start + extra + RT_DWELL_TIME;
    fr = double(extra) / double(sections[s].section_time);
  }
  dd->calc_xy(s, fr, &tx, &ty);
  trains[n].x = tx;
  trains[n].y = ty;
  if (n < (n_trains - 1))
  {
    trains[n].seconds_to_next = trains[n+1].line_location - trains[n].line_location;
  }
  else
  {
    trains[n].seconds_to_next = 0;
  }
  //printf("trx = %d, try = %d\n", trains[n].x, trains[n].y);
}

/*********************************************************/

void display_alg_t::update(time_t ts)
{
  //printf("--- update %d trains\n", n_trains);

  for (int i=0; i < n_trains; i++)
  {
    update_train(ts, i);
  }
  gen_display(ts);
}

/*********************************************************/

void display_alg_t::process_departure(int section, time_t now)
{
  bool found = false;
  int tn = 0;
  for (int i=0; i < n_trains; i++)
  {
    if (trains[i].section == section)
    {
      found = true;
      tn = i;
      break;
    }
  }
  if (!found)
  {
    printf("***** horrible error\n");
    return;
  }
  trains[tn].departed = true;
  trains[tn].departure_time = now - sections[section].departure_sensor_loc;
  trains[tn].seconds_late = (now - trains[tn].service_entry_time) -
            (sections[section].time_to_start + sections[section].departure_sensor_loc + RT_DWELL_TIME);

}

/*********************************************************/

void display_alg_t::process_arival(int section, time_t now)
{
  bool found = false;
  int tn = 0;
  for (int i=0; i < n_trains; i++)
  {
    if (trains[i].section == (section-1))
    {
      found = true;
      tn = i;
      break;
    }
  }
  if (!found)
  {
    printf("***** horrible error\n");
    return;
  }

  trains[tn].arival_time = now +
     (sections[trains[tn].section].section_time -  sections[trains[tn].section].arival_sensor_loc);
  trains[tn].departed = false;
  trains[tn].seconds_late = (trains[tn].arival_time - trains[tn].service_entry_time) - sections[section].time_to_start;
  trains[tn].section = section;
}


/*********************************************************/

void display_alg_t::process_event(crossing_event_t ev)
{
  if (ev.section == 0)
  {
    add_train(ev.time_stamp);
  }
  else if (ev.section == n_sections)
  {
    printf("----------- train left service\n");
    n_trains--;
  }
  else
  {
    if (ev.departure)
    {
      process_departure(ev.section, ev.time_stamp);
    }
    else
    {
      process_arival(ev.section, ev.time_stamp);
    }
  }
}

/*********************************************************/

