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

/*******************************************************************

Db.c

Member functions for the real time database.

*************************************************************************/


#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <stdlib.h>

#include "db_point.h"
#include "db.h"
#include "rtcommon.h"
#include "reactpoint.h"
#include "secuencia.h"
#include "conio.h"
#include "timeaccum.h"
#include "arg.h"
//#include "myconio.h"

#ifdef __REACT_MSG__
#include <sys/ipc.h>
#include <sys/msg.h>
#include "reactmsg.h"
#endif
#ifdef __REACT_SHM__
#include <sys/ipc.h>
#include <sys/shm.h>
#endif

static react_t *reactdb = NULL;

/***********************************************************************/

void react_t::send_do(int drv, int crd, int channel, bool val)
{
  /* Send a discrete output */
  io_driver->send_do(channel, val);
}

/***********************************************************************/

void react_t::send_ao(int drv, int crd, int channel, double val)
{
  /* Send an analog output */
  io_driver->send_ao(channel, val);
}

/***********************************************************************/

void react_t::exit_clean_up(void)
{
  if (io_driver != NULL)
  {
    if (num_do > 0) printf("Turning off %d discrete outputs\n", num_do);
    for (int i=0; i < num_do; i++)
    {
      do_points[i]->send(false);
    }
    if (num_ao > 0) printf("Zeroing %d analog outputs\n", num_ao);
    for (int i=0; i < num_ao; i++)
    {
      ao_points[i]->send(0.0);
    }
  }

  for (int i=0; i < num_scan; i++)
  {
    scan_points[i]->write_to_file();
  }

  for (int i=0; i < num_data; i++)
  {
    data_points[i]->write_to_file();
  }

  for (int i=0; i < num_file_logger; i++)
  {
    file_logger_points[i]->write_to_file();
  }



#ifdef __REACT_MSG__
  delete_msg_queue();
#endif

#ifdef __REACT_SHM__
  delete_shared_memory();
#endif

  if (io_driver != NULL)
  {
    io_driver->close();
  }
}

/***********************************************************************/

void react_t::print_all_points(void)
{
  for (int i=0; i < num_di; i++)
  {
    printf("DI: %s\n", di_points[i]->tag);
  }
  for (int i=0; i < num_do; i++)
  {
    printf("DO: %s\n", do_points[i]->tag);
  }
  for (int i=0; i < num_calc; i++)
  {
    printf("Calc: %s\n", calcs[i]->tag);
  }
  for (int i=0; i < num_d_calc; i++)
  {
    printf("DiscreteCalc: %s\n", d_calcs[i]->tag);
  }
  for (int i=0; i < num_dcalc; i++)
  {
    printf("DCalc: %s\n", dcalcs[i]->tag);
  }
  for (int i=0; i < num_int; i++)
  {
    printf("Int: %s\n", ints[i]->tag);
  }
  for (int i=0; i < num_ai; i++)
  {
    printf("AI: %s\n", ai_points[i]->tag);
  }
  for (int i=0; i < num_ao; i++)
  {
    printf("AO: %s\n", ao_points[i]->tag);
  }
  for (int i=0; i < num_pci; i++)
  {
    printf("PCI: %s\n", pci_points[i]->tag);
  }
  for (int i=0; i < num_pid; i++)
  {
    printf("PID: %s\n", pid_points[i]->tag);
  }
  for (int i=0; i < num_pump; i++)
  {
    printf("PUMP: %s\n", pump_points[i]->tag);
  }
  for (int i=0; i < num_ac; i++)
  {
    printf("AC: %s\n", ac_points[i]->tag);
  }
  for (int i=0; i < num_level; i++)
  {
    printf("LEVEL: %s\n", level_points[i]->tag);
  }
  for (int i=0; i < num_data; i++)
  {
    printf("Data: %s\n", data_points[i]->tag);
  }
  for (int i=0; i < num_file_logger; i++)
  {
    printf("FileLogger: %s\n", file_logger_points[i]->tag);
  }
  for (int i=0; i < num_discrete_logger; i++)
  {
    printf("DiscreteLogger: %s\n", discrete_logger_points[i]->tag);
  }
  for (int i=0; i < num_scan; i++)
  {
    printf("Scan: %s\n", scan_points[i]->tag);
  }
  for (int i=0; i < num_web; i++)
  {
    printf("Web: %s\n", web_points[i]->tag);
  }
}

timeaccum_t taa[20];

/***********************************************************************/

void react_t::read_inputs(void)
{
  int j = 0;
  taa[j].start();
  io_driver->read();
  taa[j].stop();

  j++;
  taa[j].start();
  for (int i=0; i < num_di; i++)
  {
    int ch = di_points[i]->channel;
    bool val = io_driver->get_di(ch);
    di_points[i]->update(val);
  }
  taa[j].stop();

  j++;
  taa[j].start();
  for (int i=0; i < num_ai; i++)
  {
    int ch = ai_points[i]->get_channel();
    double val = io_driver->get_ai(ch);
    ai_points[i]->update_analog(val);
  }
  taa[j].stop();

  j++;
  taa[j].start();
  for (int i=0; i < num_pci; i++)
  {
    int ch = pci_points[i]->channel;
    long val = io_driver->get_count(ch);
    pci_points[i]->update(val);
  }
  taa[j].stop();
  io_driver->end_read();


  j++;
  taa[j].start();
  for (int i=0; i < num_pid; i++)
  {
    pid_points[i]->update();
  }
  taa[j].stop();

  for (int i=0; i < num_pump; i++)
  {
    pump_points[i]->update();
  }

  for (int i=0; i < num_ac; i++)
  {
    ac_points[i]->update();
  }

  for (int i=0; i < num_level; i++)
  {
    level_points[i]->update();
  }

  j++;
  taa[j].start();
  for (int i=0; i < num_data; i++)
  {
    data_points[i]->update();
  }
  taa[j].stop();

  for (int i=0; i < num_file_logger; i++)
  {
    file_logger_points[i]->update();
  }

  for (int i=0; i < num_discrete_logger; i++)
  {
    discrete_logger_points[i]->update();
  }

  j++;
  taa[j].start();
  for (int i=0; i < num_ao; i++)
  {
    ao_points[i]->update_ramp();
  }
  taa[j].stop();

  j++;
  taa[j].start();
  for (int i=0; i < num_do; i++)
  {
    do_points[i]->update();
  }
  taa[j].stop();

  j++;
  taa[j].start();
  for (int i=0; i < num_calc; i++)
  {
    calcs[i]->update();
  }
  for (int i=0; i < num_d_calc; i++)
  {
    d_calcs[i]->update();
  }
  for (int i=0; i < num_dcalc; i++)
  {
    dcalcs[i]->update();
  }
  taa[j].stop();

  j++;
  taa[j].start();
  for (int i=0; i < num_int; i++)
  {
    ints[i]->update();
  }
  taa[j].stop();

  j++;
  taa[j].start();
  for (int i=0; i < num_web; i++)
  {
    web_points[i]->update();
  }
  taa[j].stop();

#ifdef __REACT_SHM__
  this->update_shared_memory();
#endif

  //printf("\n");
  if (shutdown)
  {
    printf("\nShuting down, sending all discrete outputs\n\r");
    for (int i=0; i < num_do; i++)
    {
      do_points[i]->send(false);
    }
    for (int i=0; i < num_ao; i++)
    {
      ao_points[i]->send(0.0);
    }
    printf("\n\r");
    exit(0);
  }
}

/***********************************************************************/
timeaccum_t ta1, ta2, ta3;

bool react_t::update(double theTime, bool execute_script)
{
  bool retval;
  this->set_time(theTime);
  if (get_time() > next_sms_check)
  {
    this->check_sms();
    next_sms_check += 10.0;
  }

  ta1.start();
  this->read_inputs();
  ta1.stop();

#ifdef __REACT_MSG__
  check_msg_queue();
#endif

  if (execute_script)
  {
    ta2.start();
    retval = this->execute_secuencia();
    if (retval)
    {
      printf("Execute returned true, time to exit\n");
    }
    ta2.stop();
  }
  else
  {
    retval = false;
  }
  execute_background_scripts();
  return retval;
}

/***********************************************************************/

point_type_t fast_get_point_type(char *tag)
{
  db_point_t *db_point;

  db_point = reactdb->get_db_point(tag);

  if (db_point == NULL)
  {
    return UNDEFINED_POINT;
  }
  return db_point->point_type();
}

/***********************************************************************/

bool *react_get_discrete_ptr_fn(char *tag)
{
  db_point_t *db_point;
  db_point = reactdb->get_db_point(tag);
  if (db_point->pv_type() != DISCRETE_VALUE)
  {
    return NULL;
  }
  else
  {
    discrete_point_t *discrete_point;
    discrete_point = (discrete_point_t *) db_point;
    return discrete_point->pv_ptr();
  }
}

double *react_get_analog_ptr_fn(char *tag)
{
  db_point_t *db_point;
  db_point = reactdb->get_db_point(tag);
  if (db_point->pv_type() != ANALOG_VALUE)
  {
    return NULL;
  }
  else
  {
    analog_point_t *analog_point;
    analog_point = (analog_point_t *) db_point;
    return analog_point->pv_ptr();
  }
}


/***********************************************************************/

react_t::react_t()
{
  /* Constructor for the Real-time DataBase. */

  reactdb = this;
  num_ai = 0;
  ai_points = NULL;
  num_pci = 0;
  pci_points = NULL;
  num_ao = 0;
  ao_points = NULL;
  num_di = 0;
  di_points = NULL;
  num_do = 0;
  do_points = NULL;
  num_dcalc = 0;
  dcalcs = NULL;
  num_calc = 0;
  calcs = NULL;
  num_d_calc = 0;
  d_calcs = NULL;
  num_int = 0;
  ints = NULL;
  num_pid = 0;
  pid_points = NULL;
  num_pump = 0;
  pump_points = NULL;
  num_ac = 0;
  ac_points = NULL;
  num_level = 0;
  level_points = NULL;
  num_data = 0;
  data_points = NULL;
  num_file_logger = 0;
  file_logger_points = NULL;
  num_discrete_logger = 0;
  discrete_logger_points = NULL;
  num_scan = 0;
  scan_points = NULL;
  num_web = 0;
  web_points = NULL;

  num_io_drivers = 0;
  io_driver = NULL;
  shutdown = false;
  dinfo.adata = NULL;
  dinfo.ddata = NULL;
  dinfo.n_analog = 0;
  dinfo.n_discrete = 0;
  shmp = NULL;
  safe_strcpy(home_dir, "./", sizeof(home_dir));
  keyboard_is_on = true;

  set_get_discrete_ptr_fn(react_get_discrete_ptr_fn);
  set_get_analog_ptr_fn(react_get_analog_ptr_fn);
  set_get_point_type_fn(fast_get_point_type);
  next_sms_check = 10.0;
}

/************************************************************************/

void react_t::init_driver(void)
{
  //io_driver = new file_io_driver_t();
  //io_driver = new PCI9111_driver_t();
  //io_driver = new PCI7248_driver_t();
  //io_driver = load_iodriver(db, "./drivers/pci9111/libpci9111.so", "new_pci9111", "libpci_dask.so");
  //io_driver = load_iodriver(db, "./drivers/pci7248/libpci7248.so", "new_pci7248", "libpci_dask.so");
  //io_driver = load_iodriver(db, "./drivers/sim/libsimdriver.so", "new_simdriver", NULL);
  char path[200];

  safe_strcpy(path, this->get_home_dir(), sizeof(path));
  safe_strcat(path, "/dbfiles/drivers.dat", sizeof(path));
  FILE *fp = fopen(path, "r");
  if (fp == NULL)
  {
    printf("Can't open %s, using default sim driver", path);
    io_driver = load_iodriver(db, "./drivers/sim/libsimdriver.so", "new_simdriver", NULL);
    return;
  }
  char line[300];

  for (int i=0; NULL != fgets(line, sizeof(line), fp); i++)
  {
    char tmp[300];
    int argc;
    char *argv[25];

    //safe_strcpy(line, "AI1|Analog Input 1|PSI|0|0|1|0|100|0|4095|1|5|10|20|80|90|5|0|0|0|0|");
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
    else if (argc != 3)
    {
      printf("Line: %s", line);
      printf("%s: Wrong number of args, line %d, %d\n", path, i+1, argc);
      continue;
    }

     /***
    io_driver_factory_t *iodrvf = load_iodriver_factory(db, argv[0], argv[1], argv[2]);
    io_driver =  
    ****/


    io_driver = load_iodriver(db, argv[0], argv[1], argv[2]);
    if (io_driver == NULL)
    {
      printf("Can't load %s, using default sim driver", argv[0]);
      io_driver = load_iodriver(db, "./drivers/sim/libsimdriver.so",
               "new_simdriver", NULL);
      return;
    }

    break; // For now, only load one driver, this will change.
  }
}


/************************************************************************/

db_point_t ** react_t::read_one_point_type(db_point_factory_t *factory, 
     const char *a_home_dir, int *cnt)
{
  printf("Reading %s ........\n", factory->abbreviation());
  int max_points = 50;
  db_point_t **db_points =
	(db_point_t **) malloc(max_points * sizeof(db_point_t*));
  MALLOC_CHECK(db_points);

  int n = 0;
  *cnt = 0;
  char path[200];
  snprintf(path, sizeof(path), "%s/dbfiles/%s", a_home_dir, factory->get_config_name());
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

    char errbuf[100];

    db_points[n] = factory->new_point(argc, argv, errbuf, sizeof(errbuf));

    if (db_points[n] == NULL)
    {
      printf("%s:%d %s\n", path, i+1, errbuf);
      continue;
    }
    printf(line);
    n++;
    if (n > max_points)
    {
      max_points += 10;
      db_points = (db_point_t **) realloc(db_points,
	      max_points * sizeof(db_point_t*));
      MALLOC_CHECK(db_points);
    }
  }
  *cnt = n;
  if (n == 0)
  {
    free(db_points);
    return NULL;
  }
  return db_points;
}

/************************************************************************/

void react_t::read_factory_points(const char *a_home_dir)
{
  char path[200];
  snprintf(path, sizeof(path), "%s/dbfiles/factory.txt", a_home_dir);
  FILE *fp = fopen(path, "r");
  if (fp == NULL)
  {
    printf("Can't open %s", path);
    return;
  }
  char line[300];

  for (int i=0; NULL != fgets(line, sizeof(line), fp); i++)
  {
    ltrim(line);
    rtrim(line);
    if (0 == strlen(line)) continue;
    if (line[0] == '#') continue;
    db_point_factory_t *factory = load_db_point("./ai.so");
    if (factory == NULL)
    {
      exit(0);
    }
    ai_points = read_one_point_type(factory, a_home_dir, &num_ai);
  }
}

/************************************************************************/

void react_t::read_all_points(const char *a_home_dir)
{
  /* Read all database points from disk. */

  this->read_factory_points(a_home_dir);
  //db_point_factory_t *aif = new ai_point_factory_t;
  /**
  db_point_factory_t *aif = load_db_point("./ai.so");
  if (aif == NULL)
  {
    exit(0);
  }

  ai_points = read_one_point_type(aif, a_home_dir, &num_ai);
  ***/

  /**
  printf("Reading ai ........\n\r");
  //ai_points = read_ai_points(&num_ai, a_home_dir);
  ai_points = ai_point_t::read(&num_ai, a_home_dir);
  **/

  printf("Reading ao ........\n\r");
  ao_points = ao_point_t::read(&num_ao, a_home_dir);

  printf("Reading di ........\n\r");
  di_points = di_point_t::read(&num_di, a_home_dir);

  printf("Reading do ........\n\r");
  do_points = do_point_t::read(&num_do, a_home_dir);

  printf("Reading pci ........\n\r");
  pci_points = pci_point_t::read(&num_pci, a_home_dir);

  printf("Reading calc ........\n\r");
  calcs = calc_point_t::read(&num_calc, a_home_dir);

  printf("Reading discrete calc ........\n\r");
  d_calcs = dcalc_point_t::read(&num_d_calc, a_home_dir);
  printf("%d discrete calcs readn\r", num_d_calc);
  

  printf("Reading dcalc ........\n");
  dcalcs = dcalc_t::read(&num_dcalc, a_home_dir);

  printf("Reading int ........\n");
  ints = int_t::read(&num_int, a_home_dir);

  printf("Reading pid ........\n");
  pid_points = pid_point_t::read(&num_pid, a_home_dir);

  printf("Reading pump ........\n");
  pump_points = pump_point_t::read(&num_pump, a_home_dir);

  printf("Reading ac ........\n");
  ac_points = ac_point_t::read(&num_ac, a_home_dir);

  printf("Reading level ........\n");
  level_points = level_point_t::read(&num_level, a_home_dir);

  printf("Reading data ........\n");
  data_points = data_point_t::read(&num_data, a_home_dir);

  printf("Reading file logger ........\n");
  file_logger_points = file_logger_t::read(&num_file_logger, a_home_dir);

  printf("Reading discrete logger ........\n");
  discrete_logger_points = discrete_logger_t::read(&num_discrete_logger, a_home_dir);

  printf("Reading scan ........\n");
  scan_points = scan_point_t::read(&num_scan, a_home_dir);

  printf("Reading web points ........\n");
  web_points = web_point_t::read(&num_web, a_home_dir);

  /* We can only parse the expressions after all points have been loaded! */
  printf("Parsing %d Analog Expressions ......\n", num_calc);
  for (int i=0; i < num_calc; i++)
  {
    calcs[i]->parse_expr();
  }

  printf("Parsing %d Discrete Expressions ......\n", num_d_calc);
  for (int i=0; i < num_d_calc; i++)
  {
    d_calcs[i]->parse_expr();
  }
#ifdef __REACT_SHM__
  this->init_shared_memory();
  this->fill_shared_memory();
  this->update_shared_memory();
#endif
#ifdef __REACT_MSG__
  this->init_msg_queue();
#endif
}

/**********************************************************************/

#ifdef __REACT_MSG__
void react_t::check_msg_queue(void)
{
  reactmsgbuf_t msgbuf;
  //printf("Checking queue . . \n");
  if (-1 != msgrcv(this->qid, &msgbuf, sizeof(msgbuf.data), 1, IPC_NOWAIT))
  {
    //printf("Wow, another connection: %s\n", msgbuf.data.msg);
    reactmsgbuf_t myreply;
    myreply.mtype = 1;
    myreply.data.qid = -1; // reall not used for a reply;

    secuencia_step_t *stp;
    stp = secuencia->new_script_type(msgbuf.data.msg,
              myreply.data.msg, sizeof(myreply.data.msg), false);
    if (stp == NULL)
    {
    }
    else if (!stp->check())
    {
      delete stp;
    }
    else
    {
      stp->execute(0.0);
      delete stp;
      strcpy(myreply.data.msg, "SUCCESS");
    }

    //printf("Sending reply on qid: %d\n", msgbuf.data.qid);
    int ret = msgsnd(msgbuf.data.qid, &myreply, sizeof(myreply.data), 0);
    if (ret == -1)
    {
      perror("Can't send message");
    }
  }
}
#endif

/**********************************************************************/

#ifdef __REACT_SHM__
void react_t::fill_shared_memory(void)
{
  if ((dinfo.adata == NULL) || (dinfo.ddata == NULL))
  {
    return;
  }
  int k=0;
  for (int i=0; i < num_ai; i++)
  {
    safe_strcpy(dinfo.adata[k].tag, ai_points[i]->tag,
                   sizeof(dinfo.adata[k].tag));
    safe_strcpy(dinfo.adata[k].description, ai_points[i]->description,
                   sizeof(dinfo.adata[k].description));
    dinfo.adata[i].min = ai_points[i]->eu_lo;
    dinfo.adata[i].max = ai_points[i]->eu_hi;
    dinfo.adata[i].dec = ai_points[i]->decimal;
    k++;
  }
  for (int i=0; i < num_ao; i++)
  {
    safe_strcpy(dinfo.adata[k].tag, ao_points[i]->tag,
                   sizeof(dinfo.adata[k].tag));
    safe_strcpy(dinfo.adata[k].description, ao_points[i]->description,
                   sizeof(dinfo.adata[k].description));
    dinfo.adata[i].min = ao_points[i]->output_limit_lo;
    dinfo.adata[i].max = ao_points[i]->output_limit_hi;
    dinfo.adata[i].dec = ao_points[i]->decimal;
    k++;
  }
  k=0;
  for (int i=0; i < num_di; i++)
  {
    safe_strcpy(dinfo.ddata[k].tag, di_points[i]->tag,
                   sizeof(dinfo.ddata[k].tag));
    safe_strcpy(dinfo.ddata[k].description, di_points[i]->description,
                   sizeof(dinfo.ddata[k].description));
    k++;
  }
  for (int i=0; i < num_do; i++)
  {
    safe_strcpy(dinfo.ddata[k].tag, do_points[i]->tag,
                   sizeof(dinfo.ddata[k].tag));
    safe_strcpy(dinfo.ddata[k].description, do_points[i]->description,
                   sizeof(dinfo.ddata[k].description));
    k++;
  }
  for (int i=0; i < num_dcalc; i++)
  {
    safe_strcpy(dinfo.ddata[k].tag, dcalcs[i]->tag,
                   sizeof(dinfo.ddata[k].tag));
    safe_strcpy(dinfo.ddata[k].description, dcalcs[i]->description,
                   sizeof(dinfo.ddata[k].description));
    k++;
  }
}

/**********************************************************************/

void react_t::update_shared_memory(void)
{
  if ((dinfo.adata == NULL) || (dinfo.ddata == NULL))
  {
    return;
  }
  int k=0;
  for (int i=0; i < num_ai; i++)
  {
    dinfo.adata[k].pv = ai_points[i]->pv;
    dinfo.adata[k].state = ai_points[i]->get_point_state();
    k++;
  }
  for (int i=0; i < num_ao; i++)
  {
    dinfo.adata[k].pv = ao_points[i]->pv;
    dinfo.adata[k].state = ao_points[i]->get_point_state();
    k++;
  }
  k=0;
  for (int i=0; i < num_di; i++)
  {
    safe_strcpy(dinfo.ddata[k].pv, di_points[i]->pv_string,
                   sizeof(dinfo.ddata[k].pv));
    dinfo.ddata[k].state = di_points[i]->get_point_state();
    k++;
  }
  for (int i=0; i < num_do; i++)
  {
    safe_strcpy(dinfo.ddata[k].pv, do_points[i]->pv_string,
                   sizeof(dinfo.ddata[k].pv));
    dinfo.ddata[k].state = do_points[i]->get_point_state();
    k++;
  }
  for (int i=0; i < num_dcalc; i++)
  {
    safe_strcpy(dinfo.ddata[k].pv, dcalcs[i]->pv_string,
                   sizeof(dinfo.ddata[k].pv));
    dinfo.ddata[k].state = dcalcs[i]->get_point_state();
    k++;
  }
}

/**********************************************************************/

void react_t::init_shared_memory(void)
{
  // First, calculate the size needed.
  int shmsize = sizeof(display_info_t) + (sizeof(display_info_t) % 8);
  int analog_offset = shmsize;
  int n_analog = num_ai + num_ao;
  int n_discrete = num_di + num_do + num_dcalc;
  shmsize +=
        (n_analog * sizeof(analog_display_data_t)) +
       ((n_analog * sizeof(analog_display_data_t)) % 8);
  int discrete_offset = shmsize;
  shmsize +=
        (n_discrete * sizeof(discrete_display_data_t)) +
       ((n_discrete * sizeof(discrete_display_data_t)) % 8);
  printf("Analog offset = %d, discrete offset = %d\n",
        analog_offset, discrete_offset);

  // Create a shared segment, read/write for me, read only for others.
  shmid =  shmget(IPC_PRIVATE, shmsize, IPC_CREAT | 0744);
  if (shmid == -1)
  {
    perror("Could not create shared memory");
    shmp = NULL;
    return;
  }
  printf("Created %d bytes of shared memory, id = %d\n", shmsize, shmid);

  // Now attach to the shared memory.
  shmp = shmat(shmid, NULL, 0);
  if (shmp == (void *) -1)
  {
    perror("Could not attach to shared memory");
    shmp = NULL;
    return;
  }
  printf("Attached to shared memory, addr = %p\n", shmp);

  // Now issue a command to delete the segment. Notice we issue a delete
  // command now, and it will only really be deleted when the LAST
  // process detaches from shared memory.
  int ret = shmctl(shmid, IPC_RMID, NULL);
  if (ret == -1)
  {
    perror("Could not remove shared memory");
    shmp = NULL;
    return;
  }
  char *cp = (char *)shmp;
  *cp = '#';
  // Now put the info at address 0 and set up the pointers.
  shm_data_header_t *shmheader = (shm_data_header_t *) cp;
  shmheader->n_analog = n_analog;
  shmheader->n_discrete = n_discrete;
  shmheader->analog_offset = analog_offset;
  shmheader->discrete_offset = discrete_offset;
  get_display_pointers(shmp, &this->dinfo);

  char path[200];
  safe_strcpy(path, this->get_home_dir(), sizeof(path));
  safe_strcat(path, "/shmid.txt", sizeof(path));
  FILE *fp = fopen(path, "w");
  if (fp == NULL)
  {
    perror(path);
    return;
  }
  fprintf(fp, "%d\n", shmid);
  fclose(fp);
  printf("%s: %d\n", path, shmid);
}

/**********************************************************************/

void react_t::delete_shared_memory(void)
{
  // Detach from shared memory.
  if (shmp == NULL)
  {
    printf("Shared memory pointer is null . . .  can not detach\n");
    return;
  }
  int ret = shmdt(shmp);
  if (ret == -1)
  {
    perror("Could not detach from shared memory");
    return;
  }
  printf("Detach from shared memory successful\n");
}

#endif

/**********************************************************************/

#ifdef __REACT_MSG__
void react_t::init_msg_queue(void)
{
  /* create the message que */

  printf("initializing queue . . \n");
  int myqid = msgget(IPC_PRIVATE, IPC_CREAT | 0777);
  if (myqid == -1)
  {
    perror("Could not create message queue");
    return;
  }
  printf("I got a message queue, id = %d\n", myqid);

  // Write the id to a file for others to use.
  char path[200];
  safe_strcpy(path, this->get_home_dir(), sizeof(path));
  safe_strcat(path, "/reactqid.txt", sizeof(path));
  FILE *fp = fopen(path, "w");
  if (fp == NULL)
  {
    perror(path);
    return;
  }
  fprintf(fp, "%d\n", myqid);
  double val;
  this->qid = myqid;
  fclose(fp);
  printf("%s: %d\n", path, myqid);
}

/**********************************************************************/

void react_t::delete_msg_queue(void)
{
  if (qid == -1)
  {
    printf("Message qid = -1, cannot destroy\n");
    return;
  }
  int ret = msgctl(this->qid, IPC_RMID, NULL);
  if (ret == -1)
  {
    perror("Could not destroy the message queue");
    return;
  }
  printf("Message queue destroyed successfully\n");
}
#endif

/**********************************************************************/

static secuencia_t *background_sequences[20];
static int n_background;

static secuencia_t *secuencia_stack[100];
static int current_secuencia;

void react_t::new_secuencia(secuencia_t *s)
{
  /* would be a good idea to make sure they are not doing recursion! */
  current_secuencia++;
  secuencia_stack[current_secuencia] = s;
  secuencia = s;
}

/**********************************************************************/

const char *react_t::secuencia_name(void)
{
  return secuencia->name;
}

/**********************************************************************/

void react_t::execute_background_scripts(void)
{
  for (int i=0; i < n_background; i++)
  {
    bool done = background_sequences[i]->run(current_time);
  }
}

/**********************************************************************/

void react_t::read_background_sequences(const char *a_home_dir, const char *a_seq_dir)
{
  char path[200];
  safe_strcpy(path, home_dir, sizeof(path));
  safe_strcat(path, "/dbfiles/background.dat", sizeof(path));
  n_background = 0;
  FILE *fp = fopen(path, "r");
  if (fp == NULL)
  {
    printf("Can't open background script file: %s\n", path);
    return;
  }
  char line[300];
  for (int i=0; NULL != fgets(line, sizeof(line), fp); i++)
  {
    ltrim(line);
    rtrim(line);
    if ((line[0] == '#') || (line[0] == '\0'))
    {
      continue;
    }
    background_sequences[n_background] = new secuencia_t(line, a_seq_dir);
    n_background++;
  }

}

/**********************************************************************/

void react_t::read_secuencia(const char *name, const char *a_home_dir)
{
  secuencia = new secuencia_t(name, a_home_dir);
  secuencia_stack[0] = secuencia;
  current_secuencia = 0;
}

/**********************************************************************/

static double quit_time = 0.0;
static bool waiting = false;
static char last_key = '\0';
static bool have_key = false;

timeaccum_t ta4;

bool react_t::execute_secuencia(void)
{
  bool done = secuencia->run(current_time);
  if (done)
  {
    if (current_secuencia != 0)
    {
      logfile->vprint("return from sequence: %s\n", secuencia->name);
      current_secuencia--;
      secuencia = secuencia_stack[current_secuencia];
      secuencia->advance();
      done = false;
    }
  }


  ta4.start();
  if (keyboard_is_on)
  {
  while (kb_hit())
  {
    char c = mygetch();
    if (c == 'q')
    {
      if (!waiting)
      {
        waiting = true;
        quit_time = current_time + 10.0;
        printf("\nTo quit hit q again within 10 seconds . . .\n\n");
      }
      else
      {
        logfile->vprint("User Terminated Program\n");
	done =  true;
      }
    }
    else if (c == '\t')
    {
      secuencia->skip_to_mark();
    }
    else if (c == 0x03)
    {
      printf("<CTRL-C> recieved\n");
      done = true;
    }
    else
    {
      last_key = c;
      have_key = true;
    }
  }
  }
  ta4.stop();

  if (waiting)
  {
    if (current_time > quit_time)
    {
      waiting = false;
      printf("Quit canceled.\n");
    }
  }

  return done;
}


/**********************************************************************/

void react_t::clear_kb_buf(void)
{
  have_key = false;
}

/**********************************************************************/

bool react_t::kbhit(void)
{
  return have_key;
}

/**********************************************************************/

void react_t::set_shutdown(void)
{
  shutdown = true;
}

/**********************************************************************/

const char *react_t::get_home_dir(void)
{
  return home_dir;
}

/**********************************************************************/

void react_t::set_home_dir(const char *a_dir)
{
  safe_strcpy(home_dir, a_dir, sizeof(home_dir));
}

/**********************************************************************/

db_point_t::db_point_t(void)
{
  /* Constructor for a database point. */

  tag[0] = '\0';
  description[0] = '\0';
  display_is_on = true;
  display_x = display_y = 0;
  point_state = last_point_state = STATE_NORMAL;
}

/***********************************************************************/

rt_double_ref_t *db_point_t::get_double_ref(const char *expr, char *err, int sz) 
{
  //pv_type_t pv_type(void) {return ANALOG_VALUE;};
  if (ANALOG_VALUE == this->pv_type())
  {
    analog_point_t *ap = (analog_point_t *) this;
    return new rt_double_ptr_ref_t(&ap->pv);  
  }
  snprintf(err, sz, "Not an analog point"); 
  return NULL;
}

/***********************************************************************/

rt_bool_ref_t *db_point_t::get_bool_ref(const char *expr, char *err, int sz) 
{
  snprintf(err, sz, "Not implemented"); return NULL;
}

/***********************************************************************/

rt_long_ref_t *db_point_t::get_long_ref(const char *expr, char *err, int sz) 
{
  snprintf(err, sz, "Not implemented"); 
  return NULL;
}

/***********************************************************************/

void db_point_t::enable_display(int x, int y)
{
  /* This enables the display for a database point. */

  display_is_on = true;
  display_x = x;
  display_y = y;
}

/*************************************************************************/

void db_point_t::disable_display(void)
{
  if (display_is_on)
  {
    /***
    gotoxy(display_x, display_y);
    //textattr(NORMAL_ATTR);
    cprintf("                                   ");
    ****/
    display_is_on = false;
  }
}
/*************************************************************************/

db_point_t *react_t::get_db_point(char *tag)
{
  /* This returns a pointer to a database point given the tag. */

  int i;

  //printf("%s:%d\n", __FILE__, __LINE__);
  for (i=0; i < num_ai; i++)
  {
    if (!strcasecmp(ai_points[i]->tag, tag))
    {
      return ai_points[i];
    }
  }

  for (i=0; i < num_pci; i++)
  {
    if (!strcasecmp(pci_points[i]->tag, tag))
    {
      return pci_points[i];
    }
  }


  for (i=0; i < num_ao; i++)
  {
    if (!strcasecmp(ao_points[i]->tag, tag))
    {
      return ao_points[i];
    }
  }

  for (i=0; i < num_di; i++)
  {
    if (!strcasecmp(di_points[i]->tag, tag))
    {
      return di_points[i];
    }
  }


  for (i=0; i < num_do; i++)
  {
    if (!strcasecmp(do_points[i]->tag, tag))
    {
      return do_points[i];
    }
  }

  for (i=0; i < num_calc; i++)
  {
    if (!strcasecmp(calcs[i]->tag, tag))
    {
      return calcs[i];
    }
  }

  for (i=0; i < num_d_calc; i++)
  {
    if (!strcasecmp(d_calcs[i]->tag, tag))
    {
      return d_calcs[i];
    }
  }


  for (i=0; i < num_dcalc; i++)
  {
    if (!strcasecmp(dcalcs[i]->tag, tag))
    {
      return dcalcs[i];
    }
  }

  for (i=0; i < num_int; i++)
  {
    if (!strcasecmp(ints[i]->tag, tag))
    {
      return ints[i];
    }
  }

  for (i=0; i < num_pid; i++)
  {
    if (!strcasecmp(pid_points[i]->tag, tag))
    {
      return pid_points[i];
    }
  }

  for (i=0; i < num_pump; i++)
  {
    if (!strcasecmp(pump_points[i]->tag, tag))
    {
      return pump_points[i];
    }
  }

  for (i=0; i < num_ac; i++)
  {
    if (!strcasecmp(ac_points[i]->tag, tag))
    {
      return ac_points[i];
    }
  }


  for (i=0; i < num_level; i++)
  {
    if (!strcasecmp(level_points[i]->tag, tag))
    {
      return level_points[i];
    }
  }


  for (i=0; i < num_data; i++)
  {
    if (!strcasecmp(data_points[i]->tag, tag))
    {
      return data_points[i];
    }
  }

  for (i=0; i < num_file_logger; i++)
  {
    if (!strcasecmp(file_logger_points[i]->tag, tag))
    {
      return file_logger_points[i];
    }
  }

  for (i=0; i < num_discrete_logger; i++)
  {
    if (!strcasecmp(discrete_logger_points[i]->tag, tag))
    {
      return discrete_logger_points[i];
    }
  }

  for (i=0; i < num_scan; i++)
  {
    if (!strcasecmp(scan_points[i]->tag, tag))
    {
      return scan_points[i];
    }
  }

  for (i=0; i < num_web; i++)
  {
    if (!strcasecmp(web_points[i]->tag, tag))
    {
      return web_points[i];
    }
  }

  return NULL;
}
/***********************************************************************/

