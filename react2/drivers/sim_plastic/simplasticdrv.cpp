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

#include "simplasticdrv.h"

/***********************************************************************

class simplasticdrv_factory_t : public io_driver_factory_t
{
private:
  react_drv_base_t *drvb;
  
public:
  simplasticdrv_factory_t(react_drv_base_t *r) {drvb = r;};
  io_driver_t *new_driver(const char *config_data, char *err, int esz)
     {return new simplasticdrv_t(drvb);};
  const char *long_description(void) {return "PumpSimulator";};
  const char *short_discription(void) {return "plasticSim";}; 
  const char *abbreviation(void) {return "PSIM";};
  ~simplasticdrv_factory_t(void) {};
};


***********************************************************************/
/*

extern "C" io_driver_factory_t *new_simplasticdrv_factory_1_0(react_drv_base_t *r, const char *option)
{
  return new simplasticdrv_factory_t(r);
}

***********************************************************************/

static double add_rand_noise(double val, double full_scale, double fraction_noise)
{
  double rand_mult = (((double) random() / (double) RAND_MAX) 
        * 2.0 * fraction_noise) - fraction_noise;
  return val + (full_scale * rand_mult);
}

/**********************************************************************/

static double calc_level_raw(double level_eu)
{
  //double pct = level_eu / 500.0;
  return 32000.0 - ((32000.0 - 6400.0) * (level_eu / 500.0));
}

/**********************************************************************/

extern "C" io_driver_t *new_simplasticdrv(react_drv_base_t *r, const char *option)
{
  return new simplasticdrv_t(r);
}

/***********************************************************************/


simplasticdrv_t::simplasticdrv_t(react_drv_base_t *r)
{
  //printf("Read: ");
  printf("Creating sim plastic driver . . . .\n");
  driver_name = "simplasticdrv";
  driver = 1;
  card = 1;
  base_addr = 0;
  fp = NULL;
  react = r;

  memset(di_data, '\0', sizeof(di_data));
  memset(do_data, '\0', sizeof(do_data));
  memset(ai_data, '\0', sizeof(ai_data));
  memset(ao_data, '\0', sizeof(ao_data));

  start = react->get_time();
  filling = true;

  di_data[P1V_DI_CH] = true;
  di_data[P2V_DI_CH] = true;

  di_data[P1_DI_CH] = true;
  ai_data[P1_AI_CH] = 6400.0;

  di_data[P2_DI_CH] = true;
  ai_data[P2_AI_CH] = 6400.0;

  di_data[P3_DI_CH] = true;
  ai_data[P3_AI_CH] = 6400.0;

  di_data[LO_LEVEL_CH] = true;
  di_data[HI_LEVEL_CH] = false;

  di_data[MIXER_DI_CH] = true;

  /* plastic */

  di_data[TANK_HI_LEVEL_DI] = false;
  di_data[TANK_HI_LEVEL_DI] = true;

  tank_temperature = START_TEMPERATURE;
  tank_level = START_LEVEL;

 /* plastic */

  fill_rate = (HI_SWITCH_CM - LO_SWITCH_CM) / PUMP_OFF_TIME; 
  plastic_rate = (HI_SWITCH_CM - LO_SWITCH_CM) / PUMP_ON_TIME; 
  plastic_rate += fill_rate; // fills while plasticing 
  plastic_rate /= 3.0; // 3 plastics

   // Start the level at the middle.
  the_level = (HI_SWITCH_CM + LO_SWITCH_CM) / 2.0; 
  ai_data[LEVEL_CH] = calc_level_raw(add_rand_noise(the_level, 500, 0.015));

  printf("fill rate: %lf, plastic_rate %lf, level %lf\n", 
       fill_rate, plastic_rate,  the_level);
  do_data[P1V_DO_CH] = false;
  do_data[P2V_DO_CH] = false;

  do_data[P1_DO_CH] = false;
  do_data[P2_DO_CH] = false;
  do_data[P3_DO_CH] = false;
}

/***********************************************************************/

void simplasticdrv_t::send_ao(int aChannel, double aVal)
{
  if ((aChannel < 0) || (aChannel > 31))
  {
    return;
  }
  //printf("New AO[%d] = %f\n", aChannel, aVal);
  ao_data[aChannel] = aVal;
  ai_data[aChannel] = aVal;
}

/***********************************************************************/

void simplasticdrv_t::send_do(int aChannel, bool aVal)
{
  if ((aChannel < 0) || (aChannel > 31))
  {
    return;
  }
  //di_data[aChannel] = aVal;
  do_data[aChannel] = aVal;
}


/***********************************************************************/

double simplasticdrv_t::get_ai(int aChannel)
{
  if ((aChannel < 0) || (aChannel > 31))
  {
    return 0.0;
  }
  return ai_data[aChannel];
}

/***********************************************************************/

void simplasticdrv_t::close(void)
{
}

/***********************************************************************/

bool simplasticdrv_t::get_di(int aChannel)
{
  if ((aChannel < 0) || (aChannel > 31))
  {
    return false;
  }
  return di_data[aChannel];
}

/***********************************************************************/

void simplasticdrv_t::read(void)
{
  double now = react->get_time();
  double elapsed_time = now - last_time;
  last_time = now;
  if (elapsed_time < 0.05) return;

  /* plastic */
  double fill_rate = 0 ;
  double temperature_rate = 0 ;
  
  if (do_data[COLD_WATER_VALVE_DO])
  {
    fill_rate += COLD_WATER_FILL_RATE;
    temperature_rate = VALVE_ON_TEMPERATURE_RATE;
  }
  else 
	 temperature_rate = VALVE_OFF_TEMPERATURE_RATE;

  if (do_data[SUCCTION_VALVE_DO])
    fill_rate += PUMP_DRAIN_RATE;
 
  tank_level += fill_rate * elapsed_time;
  tank_temperature += temperature_rate * elapsed_time;

  di_data[TANK_HI_LEVEL_DI] = tank_level > TANK_HI_LEVEL_CM;
  di_data[TANK_LO_LEVEL_DI] = tank_level > TANK_LO_LEVEL_CM;
  

  ai_data[TANK_TEMPERATURE_CH1] = tank_temperature;
  ai_data[TANK_TEMPERATURE_CH2] = tank_temperature;
    
  /* plastic */

  int n_plastics = 0;
  if (do_data[P1_DO_CH]) n_plastics++;
  if (do_data[P2_DO_CH]) n_plastics++;
  if (do_data[P3_DO_CH]) n_plastics++;

  // outputs are positive logic, inputs negative logic
  di_data[P1_DI_CH] = ! do_data[P1_DO_CH];
  di_data[P2_DI_CH] = ! do_data[P2_DO_CH];
  di_data[P3_DI_CH] = ! do_data[P3_DO_CH];

  di_data[P1V_DI_CH] = ! do_data[P1V_DO_CH];
  di_data[P2V_DI_CH] = ! do_data[P2V_DO_CH];

  double level_change_rate = fill_rate - (double(n_plastics) * fill_rate); 
  double level_change = level_change_rate * elapsed_time;

  the_level += level_change; 
  the_level = add_rand_noise(the_level, 500, 0.005);

  ai_data[LEVEL_CH] = calc_level_raw(the_level);



  //printf("%d plastics, lchange %lf, level %lf\n", 
  //      n_plastics, level_change,  the_level); 

  if (di_data[HI_LEVEL_CH]) // Set the high switch acording to the level
  {
    if ( the_level < (HI_SWITCH_CM - SWITCH_DEADBAND)) 
    {
      di_data[HI_LEVEL_CH] = false;
    }
  }
  else
  {
    if ( the_level > HI_SWITCH_CM) 
    {
      di_data[HI_LEVEL_CH] = true;
    }
  }

  if (di_data[HI_HI_LEVEL_CH]) // Set the very high switch acording to the level
  {
    if ( the_level < (HI_HI_SWITCH_CM - SWITCH_DEADBAND)) 
    {
      di_data[HI_HI_LEVEL_CH] = false;
    }
  }
  else
  {
    if ( the_level > HI_HI_SWITCH_CM) 
    {
      di_data[HI_HI_LEVEL_CH] = true;
    }
  }

  if (di_data[LO_LEVEL_CH]) // Set the low switch acording to the level
  {
    if ( the_level < (LO_SWITCH_CM - SWITCH_DEADBAND)) 
    {
      di_data[LO_LEVEL_CH] = false;
    }
  }
  else
  {
    if ( the_level > LO_SWITCH_CM) 
    {
      di_data[LO_LEVEL_CH] = true;
    }
  }

  if (do_data[P1_DO_CH])
  {
    ai_data[P1_AI_CH] = 20480.0;
    ai_data[P1_AI_CH] = add_rand_noise(ai_data[P1_AI_CH], 25000, 0.015);
  }
  else
  {
    ai_data[P1_AI_CH] = 6455.0;
    ai_data[P1_AI_CH] = add_rand_noise(ai_data[P1_AI_CH], 25000, 0.015);
  }

  if (do_data[P2_DO_CH])
  {
    ai_data[P2_AI_CH] = 20492.0;
    ai_data[P2_AI_CH] = add_rand_noise(ai_data[P2_AI_CH], 25000, 0.015);
  }
  else
  {
    ai_data[P2_AI_CH] = 6465.0;
    ai_data[P2_AI_CH] = add_rand_noise(ai_data[P2_AI_CH], 25000, 0.015);
  }

  if (do_data[P3_DO_CH])
  {
    ai_data[P3_AI_CH] = 20451.0;
    ai_data[P3_AI_CH] = add_rand_noise(ai_data[P3_AI_CH], 25000, 0.015);
  }
  else
  {
    ai_data[P3_AI_CH] = 6445.0;
    ai_data[P3_AI_CH] = add_rand_noise(ai_data[P3_AI_CH], 25000, 0.015);
  }

}

/***********************************************************************/
