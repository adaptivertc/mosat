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

extern const double driver_factors[10];

class sim_ev_notify_t
{
public:
  virtual void trigger_arrival(int section, time_t now) = 0;
  virtual void trigger_departure(int section, time_t now) = 0;
};

class dummy_notify_t : public sim_ev_notify_t
{
  void trigger_arrival(int section, time_t now);
  void trigger_departure(int section, time_t now);
};

struct tsimdata_t
{
  double driver_factor; // factor for this driver in calculating times
  int section; // section where the train is
  bool departure_triggered;
  time_t next_departure; // time that the next event is triggered
  time_t next_arival; // time that the next event is triggered
};

struct tsecdata_t
{
  int section_time;
  int departure_sensor_loc;
  int arival_sensor_loc;
};

class train_sim_t
{
private:
  int next_entry;
  time_t next_entry_time;
  int n_trains;
  tsimdata_t trains[20];
  int n_sections;
  tsecdata_t sections[50];
  int n_times;
  time_t times[500];
  char train_num[500][10];
  int week_day;
  sim_ev_notify_t *notify_obj;
  void add_train(time_t now);
  void update_train(int n, time_t now);
public:
  train_sim_t(sim_ev_notify_t *nobj);
  time_t read_day(char *fname);
  void read_sections(char *fname);
  void update(time_t now);
};

