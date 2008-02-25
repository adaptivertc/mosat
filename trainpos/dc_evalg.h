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

#include "time_table.h"

struct train_data_t
{
  int section;
  double fraction_traveled;
  bool departed;
  time_t arival_time;
  time_t departure_time;
  time_t service_entry_time;
  int seconds_in_section;
  int seconds_late;
  int num;
  bool unexpected;
  bool switched_direction;
  int line_location;
  int seconds_to_next;
  char train_id[30];
};

struct tsecdata_t
{
  int section_time;
  int departure_sensor_loc;
  int arival_sensor_loc;
  int time_to_start;
  char station[20];
};

class display_dist_t
{
/***
       x1                                      x2
       |                                       |
    y1 ------------------------------------------
       ------------------------------------------
       |                                       |
       |                                       |
    y2 ------------------------------------------
       ------------------------------------------
****/
private:
  int x1;
  int x2;
  int y1;
  int y2;
  int total_x_pixels;
  int n_sections;
  int n_squares;
  int return_start;
public:
  display_dist_t(int a_x1, int a_x2, int a_y1, int a_y2, int n_sections);
  void calc_xy(int section, double fraction, int *x, int *y, bool *coming_back);
};

class display_data_t
{
private:
  bool valid;
  FILE *fp;
  int  fd;
  display_dist_t *dd;
  const char *square;
  const char *square_unexpected;
  const char *background;
  const char *fname;
public:
  void set(const char *a_background, const char *a_square, 
       const char *a_unexpected, const char *a_fname, 
       int a_x1, int a_x2, int a_y1, int a_y2, int n_sections);
  void read(const char *base_name);
  void gen_html(time_t now, train_data_t *trains, int n_trains);
};

class display_alg_t : public event_alg_t
{
private:
  int train_number;
  int n_trains;
  int n_sections;
  int n_displays;
  time_t when_last_train_entered;

  train_data_t trains[50];
  tsecdata_t sections[50];
  time_table_t time_table;
  void update_train(time_t ts, int n);
  display_data_t *ddata;
  FILE *fp;
  int fd;
  FILE *table_fp;
  int table_fd;

  FILE *perf_fp;
  int perf_fd;

  alarm_entry_t actual_alarms[RT_MAX_ALARMS]; 
  alarm_entry_t alarm_history[RT_MAX_ALARMS]; 

  void add_train(time_t ts, const char *train_id);
public:
  void read_sections(const char *fname);
  void update(time_t time);
  void process_event(crossing_event_t ev);
  void process_departure(int section, time_t now);
  void process_arival(int section, time_t now);
  void process_arrival(crossing_event_t ev);
  void gen_display(time_t now);
  void gen_html(time_t now);
  void gen_table(time_t now);
  void gen_performance(time_t now);
  void gen_alarms(time_t now);
};


