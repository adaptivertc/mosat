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


/************************************************************************

Iodriver.c

Contains code for input/output drivers.

*************************************************************************/

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h> // for open(...)
#include <stdlib.h>
#include <string.h>
#include <float.h>
#include <sys/time.h>
#include <sys/types.h>
#include <string.h>

#include "reactpoint.h"
#include "db_point.h"
#include "db.h"
#include "iodriver.h"

#include "sim.h"
#include "simiodriver.h"

/***********************************************************************/

class sim_io_driver_factory_t : public io_driver_factory_t
{
private:
  react_drv_base_t *drvb;
  
public:
  sim_io_driver_factory_t(react_drv_base_t *r) {drvb = r;};
  io_driver_t *new_driver(const char *config_data, char *err, int esz)
     {return new sim_io_driver_t(drvb);};
  const char *long_description(void) {return "Simulator";};
  const char *short_discription(void) {return "Simulator";}; 
  const char *abbreviation(void) {return "SIM";};
  ~sim_io_driver_factory_t(void) {};
};


/***********************************************************************

extern "C" io_driver_factory_t *new_simdriver_factory_1_0(react_drv_base_t *r, const char *option)
{
  return new sim_io_driver_factory_t(r);
}

***********************************************************************/

extern "C" io_driver_t *new_simdriver(react_drv_base_t *r, const char *option)
{
  return new sim_io_driver_t(r);
}

/***********************************************************************/


sim_io_driver_t::sim_io_driver_t(react_drv_base_t *r)
{
  //printf("Read: ");
  printf("Creating sim_io_driver . . . .\n");
  const char *fname = "didata.txt";
  driver_name = "File";
  driver = 1;
  card = 1;
  base_addr = 0;
  fp = NULL;
  react = r;
  /****
  //fp = fopen(fname, "r");
  if (fp == NULL)
  {
    printf("Can not open: %s", fname);
  }
  fpout = fopen("cout.txt", "w");
  if (fp == NULL)
  {
    printf("Can not open: cout.txt");
  }
  ****/
  //printf("Creating new burner simulator\n");
  burnsim = new burnersim_t(50.0, 10.0, 20.0);
  //burnsim->nextValue(0.1, 50.0, 10.0, 50.0);

  memset(di_data, '\0', sizeof(di_data));
  memset(do_data, '\0', sizeof(do_data));
  memset(ai_data, '\0', sizeof(ai_data));
  memset(ao_data, '\0', sizeof(ao_data));
  ao_data[0] = 25.0;
  ao_data[1] = 10.0;
  ao_data[2] = 25.0;
  //ai_data[0] = burnsim->nextValue(0.2, ao_data[0],
//		                   ao_data[1], ao_data[2]);
}

/***********************************************************************/

void sim_io_driver_t::send_ao(int aChannel, double aVal)
{
  if ((aChannel < 0) || (aChannel > 31))
  {
    return;
  }
  //printf("New AO[%d] = %f\n", aChannel, aVal);
  ao_data[aChannel] = aVal;
}

/***********************************************************************/

void sim_io_driver_t::send_do(int aChannel, bool aVal)
{
  /* Use DOs to change DIs */
  //printf("--- SendDO: %d %s\n", aChannel, (aVal)?"T":"F");
  if ((aChannel <=8) && aVal && !do_data[aChannel])
  {
    int offset = (aChannel - 1)  / 2;
    bool newVal = ((aChannel - 1)  % 2) == 0;
    di_data[13 + offset] = newVal;
  }
  di_data[aChannel] = aVal;
  do_data[aChannel] = aVal;
}


/***********************************************************************/

double sim_io_driver_t::get_ai(int aChannel)
{
  if ((aChannel < 0) || (aChannel > 31))
  {
    return 0.0;
  }
  return ai_data[aChannel];
}

/***********************************************************************/

void sim_io_driver_t::close(void)
{
}

/***********************************************************************/

bool sim_io_driver_t::get_di(int channel)
{
  if ((channel > 0) && (channel < 64))
  {
    di_data[channel] = ! di_data[channel];
    return di_data[channel];
  }
  else
  {
    return false;
  }
}

/***********************************************************************/

void sim_io_driver_t::read(void)
{
  /*********
  if (fp != NULL)
  {
    char line[300];
    if (NULL != fgets(line, sizeof(line), fp))
    {
      for (int i=0; i < 8; i++)
      {
        di_data[i] = (line[i] == 'T');
      }
    }
    else
    {
      for (int i=0; i < 8; i++)
      {
        di_data[i] = false;
      }
    }
  }
  //printf("Read: ");
  for(int i=0; i < 8; i++)
  {
    //printf("%s ", di_data[i] ? "T": "F");
  }
  ***/

  //printf("Calling burner sim\n");
  ai_data[0] = burnsim->nextValue(react->get_time(), ao_data[0],
		                   ao_data[1], ao_data[2]);
  //printf("new val = %lf\n", ai_data[0]);
	  /**
  if (fpout != NULL)
  {
    fprintf(fpout, "%lf\t %lf\t %lf\n",
		  react->get_time(),
		  ai_data[0],
		  ao_data[2]);
  }
	/***/
}



/***********************************************************************/
