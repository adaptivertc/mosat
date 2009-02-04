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

Db_point.h

Header file contain individual point definitions for the real time
database.

**************************************************************************/

#ifndef __DB_POINT_INC__
#define __DB_POINT_INC__

#include <time.h>
#include <sys/time.h>
//#include <ncurses.h>
#include "rtcommon.h"
#include "reactpoint.h"
#include "exp.h"
#include "dbref.h"

#define DESCRIPTION_OFFSET 0
#define PV_OFFSET 21
#define EU_OFFSET 28

class db_point_t;

struct driver_data_t
{
  char *driver;
  char *point_id;
};

/*******
class discrete_input_interface_t
{
};

class discrete_output_interface_t
{
};

class analog_input_interface_t
{
};

class analog_output_interface_t
{
};
*******/

struct driver_info_t
{
  char name[51];
  char card[26];
  char channel[26];
};

class secuencia_step_t;

class db_point_factory_t
{
public:
  virtual db_point_t *new_point(int argc, char *argv[], char *err, int esz) = 0;
  virtual void delete_point(db_point_t *) = 0;
  // Following is the name of the config file, example "ai.dat"
  virtual const char *get_config_name(void) = 0;
  // The folowing are class descriptions.
  virtual const char * long_description(void) = 0; // multiple lines
  virtual const char * short_description(void) = 0; // less than 60 chars
  virtual const char * abbreviation(void) = 0;    // less than 8 chars
  virtual bool is_analog_input() = 0;
  virtual bool is_discrete_input() = 0;
  virtual bool is_integer_input() = 0;
  virtual bool is_analog_output() = 0;
  virtual bool is_discrete_output() = 0;
  virtual bool is_integer_output() = 0;
  virtual bool needs_update() = 0;
  virtual bool has_analog_val() = 0;
  virtual bool has_discrete_val() = 0;
  virtual bool has_int_val() = 0;
  virtual const char *list_functions(void) {return "";};
  virtual ~db_point_factory_t(void) {};
};

class db_point_interface_t
{
public:
  virtual void update(void) {};
  virtual void update_analog(double v) {};
  virtual void update_discrete(bool v) {};
  virtual void update_int(long v) {};
  virtual double get_analog(void) {return 0.0;};
  virtual bool get_discrete(void) {return false;};
  virtual long get_int(void) {return 0;};
  virtual int get_driver(void) {return -1;};
  virtual int get_card(void) {return -1;};
  virtual int get_channel(void) {return -1;};
  virtual const char *get_tag(void) = 0;
  virtual const char *get_description(void) = 0;
  virtual void exit_cleanup(void) {};
  virtual driver_info_t *get_driver_info(void) {return NULL;};
  virtual secuencia_step_t *get_secuencia_step(int argc, char *argv[], char *err_str, int esize)
       {snprintf(err_str, esize, "%s", "No functions for this point type"); return NULL;};
  virtual rt_double_ref_t *get_double_ref(const char *expr, char *err, int sz) = 0;
  virtual rt_bool_ref_t *get_bool_ref(const char *expr, char *err, int sz) = 0;
  virtual rt_long_ref_t *get_long_ref(const char *expr, char *err, int sz) = 0;

  virtual ~db_point_interface_t(void) {};
};

class db_point_t : public db_point_interface_t
{
public:
  tag_t tag;
  char description[21];
  point_state_t point_state;
  point_state_t last_point_state;
  int display_x, display_y, display_width;
  bool display_is_on;
  bool point_alarm_disable;

  void enable_display(int x, int y);
  void disable_display(void);
  point_state_t get_point_state(void) {return point_state;};
  virtual void display(void) {};
  virtual point_type_t point_type(void) = 0;
  virtual pv_type_t pv_type(void) {return UNDEFINED_VALUE;};
  virtual void display_pv(void) {};
  db_point_t(void);
  void trip_shutdown(char *reason);

  void send_sms_group(const char *msg, const char *group);
  const char *get_tag(void) {return tag;};
  const char *get_description(void) {return description;};
  rt_double_ref_t *get_double_ref(const char *expr, char *err, int sz);
  rt_bool_ref_t *get_bool_ref(const char *expr, char *err, int sz);
  rt_long_ref_t *get_long_ref(const char *expr, char *err, int sz);

  virtual ~db_point_t(void) {};
};

class field_point_t
{
public:
  int driver;
  int card;
  int channel;
};

class analog_point_t : public db_point_t
{
public:
  double pv;
  char eu[8];
  char fmt[8];
  int round;
  int decimal;
  analog_point_t(void);
  void set_format(void);
  inline double get_pv(void) {return pv;};
  void display(void);
  void display_pv(void);
  inline double *pv_ptr(void) {printf("Got ptr: %s\n", tag);return &pv;};
  pv_type_t pv_type(void) {return ANALOG_VALUE;};
};

class analog_update_point_t;

class analog_update_point_t : public analog_point_t
{
public:
  double hi_alarm;
  double hi_caution;
  double lo_caution;
  double lo_alarm;
  double deadband;
  bool hi_alarm_enable;
  bool lo_alarm_enable;
  bool hi_caution_enable;
  bool lo_caution_enable;
  bool hi_alarm_shutdown;
  bool lo_alarm_shutdown;
  void check_alarms(void);
  analog_update_point_t(void);
};


class ai_point_t : public analog_update_point_t, public field_point_t
{
public:
  double pv_last;
  double raw_value;
  int update_time_stamp;
  int last_update_time_stamp;
  double eu_hi;
  double eu_lo;
  double raw_lo;
  double raw_hi;
  double conversion_factor;
  double zero_cutoff;
public:
  void update_analog(double v) {this->update(v);};
  double get_analog(void) {return this->pv;};

  /* Will have to do this for all points that have driver, card channel */
  virtual int get_driver(void) {return this->driver;};
  virtual int get_card(void) {return this->card;};
  virtual int get_channel(void) {return this->channel;};

  void update(double new_raw_value);
  void set_conversion(double rl, double rh, double eul, double euh);
  void set_conversion(void);
  point_type_t point_type(void) {return ANALOG_INPUT;};
  static ai_point_t *read_one(int argc, char *argv[], char *err, int esz);
  //static ai_point_t **read(int *cnt, const char *home_dir);
};

/*#SCRIPT_OBJECT#(ANALOG_OUTPUT)*/
class ao_point_t : public analog_point_t, public field_point_t
{
public:
  int output_time_stamp;
  double output_limit_hi;
  double output_limit_lo;
  double eu_hi;
  double eu_lo;
  double raw_lo;
  double raw_hi;
  bool ramp_on;
  double ramp_inc;
  double ramp_val;
public:

  virtual int get_driver(void) {return this->driver;};
  virtual int get_card(void) {return this->card;};
  virtual int get_channel(void) {return this->channel;};

/*#SCRIPT_FUNCTION#*/
  void send(double val);
/*#SCRIPT_FUNCTION#*/
  void ramp(double val, double time);
  void sendit(double val);
  void update_ramp(void);
  point_type_t point_type(void) {return ANALOG_OUTPUT;};
  static ao_point_t *read_one(int argc, char *argv[], char *err, int esz);
  static ao_point_t **read(int *cnt, const char * home_dir);
};


class pci_point_t : public analog_update_point_t, public field_point_t
{
public:
  double pv_last;
  long raw_count;
  long last_count;
  long history_loc;
  long count_history[100];
  struct timeval time_history[100];
  double accel;
  double position;
  double eu_per_count;
  long min_counts_for_calc;
  long rollover_count;
  double time_factor;
  double last_time;
  double zero_cutoff;
public:
  virtual int get_driver(void) {return this->driver;};
  virtual int get_card(void) {return this->card;};
  virtual int get_channel(void) {return this->channel;};
  void update(long new_raw_count);
  point_type_t point_type(void) {return PCI_POINT;};
  static pci_point_t **read(int *cnt, const char *home_dir);
};

class discrete_point_t : public db_point_t
{
public:
  bool pv;
  const char *pv_string;
  char hi_desc[12];
  char lo_desc[12];
  void display(void);
  void display_pv(void);
  inline bool get_pv(void) {return pv;};
  bool *pv_ptr(void) {return &pv;};
  pv_type_t pv_type(void) {return DISCRETE_VALUE;};
};

class integer_point_t : public db_point_t
{
public:
  int pv;
  //void display(void);
  //void display_pv(void);
  inline int get_pv(void) {return pv;};
  int *pv_ptr(void) {return &pv;};
  pv_type_t pv_type(void) {return INTEGER_VALUE;};
};


class discrete_update_point_t : public discrete_point_t
{
public:
  discrete_value_t shutdown_state;
  discrete_value_t alarm_state;
  void check_alarms(void);
  discrete_update_point_t(void);
};

class di_point_t : public discrete_update_point_t, public field_point_t
{
public:
  bool pv_last;
  int update_time_stamp;
  int last_update_time_stamp;
public:
  virtual int get_driver(void) {return this->driver;};
  virtual int get_card(void) {return this->card;};
  virtual int get_channel(void) {return this->channel;};
  void update(bool new_value);
  point_type_t point_type(void) {return DISCRETE_INPUT;};
  static di_point_t **read(int *cnt, const char * home_dir);
};

class dcalc_t : public discrete_update_point_t
{
public:
  bool pv_last;
  int update_time_stamp;
  int last_update_time_stamp;
  //char calc[200];
  int n_dpoints;
  discrete_point_t *dpoints[30];
public:
  void update(void);
  bool eval_expr(void);
  point_type_t point_type(void) {return DCALC_POINT;};
  static dcalc_t **read(int *cnt, const char * home_dir);
};

class int_t : public integer_point_t
{
public:
  bool pv_last;
  int update_time_stamp;
  int last_update_time_stamp;
  //char calc[200];
  int n_dpoints;
  discrete_point_t *dpoints[30];
public:
  void update(void);
  int eval_expr(void);
  point_type_t point_type(void) {return INT_POINT;};
  static int_t **read(int *cnt, const char * home_dir);
};


/*#SCRIPT_OBJECT#(DISCRETE_OUTPUT)*/
class do_point_t : public discrete_point_t, public field_point_t
{
private:
  bool tdo_on;
  bool blink_on;
  double blink_time;
  double operation_end_time;
public:
  virtual int get_driver(void) {return this->driver;};
  virtual int get_card(void) {return this->card;};
  virtual int get_channel(void) {return this->channel;};
  int output_time_stamp;
  void send_it(bool value);
/*#SCRIPT_FUNCTION#*/
  void send(bool value);
/*#SCRIPT_FUNCTION#*/
  void tdo(double val);
/*#SCRIPT_FUNCTION#*/
  void blink(double val);
  void update(void);
  point_type_t point_type(void) {return DISCRETE_OUTPUT;};
  static do_point_t **read(int *cnt, const char * home_dir);
};

class calc_point_t : public analog_update_point_t
{
public:
  analog_expr_t expression;
  char *expr_string;
  void update(void);
  void parse_expr(void);
  point_type_t point_type(void) {return CALC_POINT;};
  static calc_point_t *read_one(int argc, char *argv[], char *err, int esz);
  static calc_point_t **read(int *cnt, const char * home_dir);
};

class dcalc_point_t : public discrete_update_point_t
{
public:
  discrete_expr_t expression;
  char *expr_string;
  void evaluate(void);
  void parse_expr(void);
  point_type_t point_type(void) {return DCALC_POINT;};
  static dcalc_point_t **read(int *cnt, const char * home_dir);
};


/*#SCRIPT_OBJECT#(DATA_POINT)*/
class data_point_t : public discrete_point_t
{
public:
  char file_name[50];
  int num_points;
  int max_time;
  analog_point_t **analog_points;
  int max_samples;
  bool collecting;
  int count;
  double **data;
  void update(void);
/*#SCRIPT_FUNCTION#*/
  void start(void);
/*#SCRIPT_FUNCTION#*/
  void stop(void);
  void write_to_file(void);
  point_type_t point_type(void) {return DATA_POINT;};
  static data_point_t **read(int *cnt, const char * home_dir);
};

/*#SCRIPT_OBJECT#(FILE_LOGGER)*/
class file_logger_t : public discrete_point_t
{
public:
  char base_name[50];
  int num_points;
  analog_point_t **analog_points;
  bool collecting;
  int sample_interval;
  int n_days_of_history;
  bool instantaneous_enable;
  bool hour_enable;
  bool day_enable;
  double *hour_averages;
  int n_hour_samples;
  bool week_enable;
  time_t last_log_time;
  bool month_enable;
  FILE *instantaneous_fp;
  FILE *hour_fp;
  void update(void);
/*#SCRIPT_FUNCTION#*/
  void start(void);
/*#SCRIPT_FUNCTION#*/
  void stop(void);
  void write_to_file(void);
  point_type_t point_type(void) {return DATA_POINT;};
  static file_logger_t **read(int *cnt, const char * home_dir);
  void delete_old_files(time_t now);
};


class discrete_logger_t : public discrete_point_t
{
public:
  char base_name[50];
  int num_points;
  discrete_point_t **discrete_points;
  bool *log_rising;
  bool *log_falling;
  bool *last_discrete_vals;
  FILE *instantaneous_fp;
  bool collecting;
  time_t last_log_time;
  void update(void);
  point_type_t point_type(void) {return DATA_POINT;};
  static discrete_logger_t **read(int *cnt, const char * home_dir);
};

/*#SCRIPT_OBJECT#(SCAN_POINT)*/
class scan_point_t : public discrete_point_t
{
private:
  char file_name[50];
  int num_points;
  analog_point_t **analog_points;
  int max_samples;
  bool collecting;
  int count;
  float **data;
public:
  void update(void){};
/*#SCRIPT_FUNCTION#*/
  void scan(void);
  void write_to_file(void);
  point_type_t point_type(void) {return SCAN_POINT;};
  static scan_point_t **read(int *cnt, const char * home_dir);
};


class control_point_t : public analog_point_t
{
protected:
	/* configuration */
  double setpoint_limit_hi;
  double setpoint_limit_lo;
  double deviation_alarm;
  double deviation_caution;
  double deviation_delay;
  bool dev_alarm_enable;
  bool dev_alarm_shutdown;
  bool dev_caution_enable;
  ao_point_t *ao_point;
  ai_point_t *ai_point;

      /* real-time */
  bool control_enabled;
  double dev_caut_detect_time;
  double dev_alm_detect_time;
  double setpoint;

  bool ramp_is_on;
  double ramp_increment;
  double ramp_value;
  int ramp_counter;
public:
  void set_ramp(double val, double ramp_time);
  void update_ramp(void);
  void reset_alarms(void);
  void check_alarms(void);
};

/*#SCRIPT_OBJECT#(TIMER_POINT)*/
class timer_point_t : public analog_point_t
{
private:
  bool mode_count_up; 
  bool running;
  double last_time;

public:
/*#SCRIPT_FUNCTION#*/
  void zero(void) {pv = 0.0;};
/*#SCRIPT_FUNCTION#*/
  void set(double aval) {pv = aval;};
/*#SCRIPT_FUNCTION#*/
  void stop(void);
/*#SCRIPT_FUNCTION#*/
  void count_up(void);
/*#SCRIPT_FUNCTION#*/
  void count_down(void);
/*#SCRIPT_FUNCTION#*/
  void start(void);
  void update(void);
  point_type_t point_type(void) {return TIMER_POINT;};
  static timer_point_t *read_one(int argc, char *argv[], char *err, int esz);
  static timer_point_t **read(int *cnt, const char * home_dir);
};

/*#SCRIPT_OBJECT#(PID_POINT)*/
class pid_point_t : public control_point_t
{
private:
	/* configuration */
  double p_gain;
  double i_time;
  double d_time;

      /* real-time */
  double last_input;
  double last_output;
  double last_error;
  double integrated_error;
  double last_time;
  int updates_per_evaluation;
  int delay_counter;
  bool tune;
  bool tune_phase_2;
  double tune_start_time;
  double tune_delay;
  double tune_initial_output;
  double tune_final_output;
  double *tune_data;
  int tune_n_samples;
  int tune_total_samples;
public:
/*#SCRIPT_FUNCTION#*/
  void change_setpoint(double val, double ramp_time);
/*#SCRIPT_FUNCTION#*/
  void spt(double val) {change_setpoint(val,0.0);};
/*#SCRIPT_FUNCTION#*/
  void ramp(double val, double time) {change_setpoint(val,time);};
/*#SCRIPT_FUNCTION#*/
  void start_control(void);
/*#SCRIPT_FUNCTION#*/
  void stop_control(void);
/*#SCRIPT_FUNCTION#*/
  void auto_tune(double initial, double final, double time);
  point_type_t point_type(void) {return PID_POINT;};
  void update(void);
  void tune_update(void);
  static pid_point_t **read(int *cnt, const char * home_dir);
};

class pump_point_t : public discrete_point_t
{
private:
  ai_point_t *ai_point;
  di_point_t *di_point;
  ai_point_t *level_ai_point;

  time_t last_change_time;
  bool last_state_at_change; 
  double last_current;

  bool change_started;
  char change_start_line[100];

  time_t this_hour;
  time_t this_day;

  double hour_total_amps; // When ON
  int hour_num_on_readings;
  int hour_num_off_reading;

  double day_total_amps;  // When ON
  int day_num_on_readings;
  int day_num_off_reading;

  int hour_total_seconds; // Total seconds ON
  int day_total_seconds;  // Total seconds ON


  double min_amps; // When ON
  double max_amps; // When ON
  double delay; // Delay before applying the alarm

  FILE *history_fp;
public:
  point_type_t point_type(void) {return PUMP_POINT;};
  void update(void);
  static pump_point_t **read(int *cnt, const char * home_dir);
};

class level_point_t : public discrete_point_t
{
private:
  bool valid;

  ai_point_t *level_point;
  di_point_t *di_lo;
  di_point_t *di_hi;
  double volume;

  double current_level;
  bool lo_state;
  bool hi_state;
  bool pumping;


  time_t time_at_bottom;
  time_t this_hour;
  time_t this_day;

  double total_day_fill_rates;
  int total_day_samples;
  double total_hour_fill_rates;
  int total_hour_samples;

  FILE *level_fp;
public:
  point_type_t point_type(void) {return LEVEL_POINT;};
  void update(void);
  static level_point_t **read(int *cnt, const char * home_dir);
};

class ac_point_t : public discrete_point_t
{
private:
  /**
  ai_point_t *ai_point;
  di_point_t *di_point;
  ai_point_t *level_ai_point;
  **/

  ai_point_t *cold_temp_point;
  ai_point_t *hot_temp_point;
  di_point_t *unit_running_point;
  do_point_t *unit_disable_point;

  time_t last_change_time;
  bool last_state_at_change; 
  double last_current;

  bool change_started;
  bool delay_elapsed;

  bool cold_detected;
  bool cold_alarm;
  time_t cold_detect_time;

  bool small_diference_dectected;
  bool diff_alarm;
  time_t small_difference_detect_time;

  time_t this_hour;
  time_t this_day;

  double hour_total_amps; // When ON

  double hour_total_cold; // When ON after delay
  double hour_total_hot; // When ON after delay

  int hour_num_temp_readings;
  int hour_num_on_readings;
  int hour_num_off_readings;

  double day_total_amps; // When ON
  double day_total_cold; // When ON after delay
  double day_total_hot; // When ON after delay

  int day_num_on_readings;
  int day_num_off_readings;

  int day_num_temp_readings;

  int hour_total_seconds; // Total seconds ON
  int day_total_seconds;  // Total seconds ON


  double min_amps; // When ON
  double max_amps; // When ON
  double delay; // Delay before applying the alarm

  FILE *history_fp;
public:
  point_type_t point_type(void) {return AC_POINT;};
  void update(void);
  static ac_point_t **read(int *cnt, const char * home_dir);
  const char *is_disabled(bool *disable);
  double get_hot_average(void); 
  double get_cold_average(void); 
  void get_status(char *status, int len);
  void reset(void);
};

/* Structure used by Web Points. */
struct discrete_ref_t
{
  char *str;
  int size;
  const char *true_string;
  const char *false_string;
  discrete_point_t *dp;
  discrete_ref_t *next;
};

struct analog_ref_t
{
  char *str;
  int size;
  bool is_bar;
  char *bar;
  char *half_on; 
  char *half_off;
  int bar_top;
  int bar_left;
  int bar_length; 
  int bar_min; 
  int bar_max; 
  analog_point_t *ap;
  analog_ref_t *next;
};

struct int_ref_t
{
  char *str;
  int size;
  char *default_str;
  int nvals;
  char *strings[10];
  int values[10];
  int_t *ip;
  int_ref_t *next;
};

class web_point_t : public discrete_point_t
{
private:
  double update_interval;
  char source_file[200];
  char dest_file[200];
  int file_size;
  char *file_buf;
  discrete_ref_t *drefs;
  analog_ref_t *arefs;
  int_ref_t *irefs;
  void subst_string(char *sdest, const char *snew, int len);
  void read_file(void);
  void update_file_and_write(void);
  double last_time;
  char *time_location;
  int time_size;
public:
  point_type_t point_type(void) {return WEB_POINT;};
  void update(void);
  static web_point_t **read(int *cnt, const char * home_dir);
};

void enable_alarm_display(db_point_t *db_point);


#endif
