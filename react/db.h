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

/********
Db.h

Header file containing main real time database structure.

**************************************************************************/

#ifndef __DB_INC__
#define __DB_INC__

// The following defines tell react to use shared memory and message queues (Unix/Linux)

//#define __REACT_SHM__
//#define __REACT_MSG__

class react_base_t;
class react_drv_base_t;

#include "db_point.h"
#include "secuencia.h"
#include "iodriver.h"
#include "displaydata.h"

#define MAX_POINT_TYPES (100)
class react_drv_base_t
{
public:
  virtual double get_time(void) = 0;
  virtual ~react_drv_base_t(void) {};
};

class react_base_t : public react_drv_base_t
{
public:
  //virtual double get_time(void) = 0;
  virtual double get_sample_rate(void) = 0;
  virtual const char *get_home_dir(void) = 0;
  virtual void send_do(int drv, int crd, int chnl, bool val) = 0;
  virtual void send_ao(int drv, int crd, int chnl, double val) = 0;
  virtual db_point_t *get_db_point(char *tag) = 0;
  virtual bool global_alarm_disabled(void) = 0;
  virtual void set_shutdown(void) = 0;
  virtual void clear_kb_buf(void) = 0;
  virtual bool kbhit(void) = 0;
  virtual void new_secuencia(secuencia_t *s) = 0;
  virtual const char *secuencia_name(void) = 0;
  virtual ~react_base_t(void) {};
};

typedef db_point_factory_t *(*get_db_factory_t)(react_drv_base_t *r);
db_point_factory_t *load_db_point(const char *sofile);

class react_t : public react_base_t
{
private:
  int qid;
  int shmid;
  void *shmp;
  display_info_t dinfo;
  char home_dir[200];
  double current_time;
  double sample_rate;
  bool keyboard_is_on;
  db_point_factory_t *point_factory[MAX_POINT_TYPES];
  db_point_t **db_points[MAX_POINT_TYPES];
  db_point_t **read_one_point_type(db_point_factory_t *factory, const char *a_home_dir, int *n);
  void read_factory_points(const char *a_home_dir);

public:
  bool global_alarm_disable;
  db_point_t **ai_points;
  int num_ai;
  pci_point_t **pci_points;
  int num_pci;
  ao_point_t **ao_points;
  int num_ao;
  di_point_t **di_points;
  int num_di;
  do_point_t **do_points;
  int num_do;
  calc_point_t **calcs;
  int num_calc;
  dcalc_t **dcalcs;
  int num_dcalc;
  int_t **ints;
  int num_int;
  pid_point_t **pid_points;
  int num_pid;
  pump_point_t **pump_points;
  int num_pump;
  level_point_t **level_points;
  int num_level;
  data_point_t **data_points;
  int num_data;
  file_logger_t **file_logger_points;
  int num_file_logger;
  scan_point_t **scan_points;
  int num_scan;
  web_point_t **web_points;
  int num_web;
  io_driver_t *io_driver;
  int num_io_drivers;
  secuencia_t *secuencia;
  bool shutdown;
  void read_inputs(void);
  void read_secuencia(const char *name, const char *a_home_dir);
  void new_secuencia(secuencia_t *s);
  const char *secuencia_name(void);
  bool execute_secuencia(void);
  bool update(double theTime, bool execute_script);
  void read_io_cards(void);
  react_t();
  void read_all_points(const char *a_home_dir);
  void print_all_points(void);
  void send_do(int drv, int crd, int chnl, bool val);
  void send_ao(int drv, int crd, int chnl, double val);
  void init_driver(void);
  void set_time(double tnow) {current_time = tnow;};
  void set_sample_rate(double rate) {sample_rate = rate;};
  double get_sample_rate(void) {return sample_rate;};
  double get_time(void) {return current_time;};
  void set_shutdown(void);
  db_point_t *get_db_point(char *tag);
  void clear_kb_buf(void);
  void exit_clean_up(void);
  bool kbhit(void);
  void check_msg_queue(void);
  void init_msg_queue(void);
  void delete_msg_queue(void);
  void init_shared_memory(void);
  void fill_shared_memory(void);
  void update_shared_memory(void);
  void delete_shared_memory(void);
  void set_home_dir(const char *dir);
  const char *get_home_dir(void);
  void keyboard_on(bool v){keyboard_is_on = v;};
  bool global_alarm_disabled(void) {return global_alarm_disable;};
};

extern react_base_t *db;
extern bool signal_recieved;

#endif
