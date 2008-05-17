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

reactmodbusdrv.cpp

Contains code for react modbus driver.

*************************************************************************/

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <float.h>
#include <sys/time.h>
#include <sys/types.h>

#include <semaphore.h>
#include <pthread.h>

#include "rtmodbus.h"

#include "reactpoint.h"
#include "db_point.h"
#include "db.h"
#include "iodriver.h"

#include "reactmodbusdrv.h"

/****
  Ok, this is a first stab at a modbus driver. There are many issues
  to deal with, such as what is the round trip delay, and how many
  round trips to you need to read all the data. For a LAN, the round
  trips should be very short, and typically you would only read modbus
  devices that were local. You could have a modbus bridge with a
  slow serial device on the other end though. Also, this same driver
  will be adapted to talk directly to serial devices.

  Also, remember that you need to send outputs. If you have a script
  that sends lots of outputs, that can add up. You might want a back-
  ground thread to send the outputs so the main thread does not have
  to wait while the outputs are being sent, and you might also be able
  to send more than one output in the same message. This would mean we
  would need a hook to tell the driver to flush the outputs.

  I am thinking we need a hook so that all device can be told a poll
  is starting, so they could all start polling the devices at the same
  time. This would be ignored by drivers that did not have a background
  thread.

  This uses the main thread
  to read the remote device. It assumes a single start address for
  ai, ao, di, do, counters. It assumes a single modbus device number.

  In the future, we would like the driver to possible read more than one
  offset, and more than one modbus device number.

  1) The main thread will block waiting for the modbus data when the
     data is read. This would work just fine for sample periods that
     are sufficiently less than the summ of the round trip delays.

  2) A back ground thread the continually reads the modbus data and puts
     it in memory protected by a semaphore. This could be free running, or
     at a given sample rate. Then when the main thread
     wants to read the data, a recent value will be in memory.

  3) Further, especially for each slow serial modbus device, we MUST have a 
     seperate thread polling it, rather than polling them all in serial. 
     With this in mind, it might be a good idea to have a thread for each device number,
     since each might be a different serial port on a bridge, and could thus be
     polled at the same time. Should this be configurable?? Thinking more, it would
     have to be configurable, since often multiple devices are on the SAME RS-485 serial 
     port, and thus MUST be polled in serial!!
     
  4) So what is the design? One single modbus driver with different threads, a modbus
     driver for each ip address, or a modbus driver for each modbus device number??

  5) How do we do the configuration? It might be that IP addresses and modbus device 
     numbers could change when the network is re-configured. How about if the modbus
     ip address and device number are part of the driver configuration, and then you
     would select modbus1, modbus2, etc, (which would include the ip address and device number),
     and then, if the ip address or device number changes for a particular device, we
     just need to change it in one place. It would also be VERY cumbersome to be specifying
     the IP address and device number for EACH point, then sorting these to create groups.
     Better to explicitly create the groups bye ip address and device number, or serial device
     and device number. Then, the card will be the offsett, and the channel will be the
     channel from there. 

     Given this, we might then need a way to group drivers when they are all on the same
     serial port and must be polled by the same thread. And, since each modbus bridge could
     have more than one serial port, we can not necessarily do this by ip address.

  6) On startup, it should be configurable driver by driver if the outputs should be zeroed
     when starting React and when stopping React. Since modbus allows us to read the current
     state of the outputs, this is possible! With other cards, you MUST start with the outputs
     in a known state.

****/
/***********************************************************************/

static void *rtmodbus_start_read_thread(void *driver_ptr)
{
  reactmodbus_driver_t *p = (reactmodbus_driver_t *)driver_ptr;
  p->read_thread();
}

extern "C" io_driver_t *new_reactmodbus(react_drv_base_t *r)
{
  printf("Creating new reactmodbus iodriver\n");
  return new reactmodbus_driver_t(r);
}

/***********************************************************************/

reactmodbus_driver_t::reactmodbus_driver_t(react_drv_base_t *react)
{

/* Ok, we really need a configuration file to configure modbus, to start
 * it will be all hard-coded 
 */

  driver_name = "reactmodbus";
  di_offset = 0;
  do_offset = 0;
  ai_offset = 0;
  ao_offset = 0;

  read_values = false;
  wake_him_up = true;
  printf("initializing modbus\n");
  //modbus = rt_create_modbus("127.0.0.1:502");
  //modbus = rt_create_modbus("192.168.1.104:502");
  modbus = rt_create_modbus("127.0.0.1:502");
  if (modbus == NULL)
  {
    exit(0);
  }
  modbus->set_debug_level(3);
  printf("DONE initializing modbus\n");

  printf("Initializing semaphores\n");
  if (0 != sem_init(&read_mutex_sem, 0, 1))
  {
    perror("sem_init");
  }
  if (0 != sem_init(&read_wait_sem, 0, 0))
  {
    perror("sem_init");
  }
  printf("DONE Initializing semaphores\n");
  
  printf("Creating thread\n");
  pthread_t thr;
  int retval;
  retval = pthread_create(&thr, NULL, rtmodbus_start_read_thread, this);
  if (retval != 0)
  {
    perror("can't create thread");
    exit(0);
  }
  retval = pthread_detach(thr);
  if (retval != 0)
  {
    perror("can't detach thread");
    exit(0);
  }
  printf("DONE Creating thread\n");

}

/***********************************************************************/

void reactmodbus_driver_t::send_ao(int ch, double val)
{
  if ((ch >= 0) && (ch < 64))
  {
    //shm->ao_val[ch] = val;
  }
  //printf("Send AO ch = %d, val = %lf\n", ch, val);
}

/***********************************************************************/

void reactmodbus_driver_t::close(void)
{
}

/***********************************************************************/

void reactmodbus_driver_t::send_do(int ch, bool val)
{
  if ((ch >= 0) && (ch < 64))
  {
    //shm->do_val[ch] = val;
    modbus->send_do(ch, val);
  }
}

/***********************************************************************/

double reactmodbus_driver_t::get_ai(int channel)
{
  double read_val;
  if ((channel >= 0) && (channel < 64))
  {
    read_val = (double) ai_vals[channel];
    printf("AI %d, read %lf\n", channel, read_val);
    return read_val;
  }
  else
  {
    return 0.0;
  }
}

/***********************************************************************/

bool reactmodbus_driver_t::get_di(int channel)
{
  bool read_val;
  if ((channel >= 0) && (channel < 64))
  {
    read_val = di_vals[channel];
    printf("DI %d, read %s\n", channel, read_val ? "T" : "F");
    return read_val;
  }
  else
  {
    return false;
  }
}

/***********************************************************************/

long reactmodbus_driver_t::get_count(int channel)
{
  if ((channel >= 0) && (channel < 64))
  {
    return 0;//shm->count_val[channel];
  }
  else
  {
    return 0;
  }
}

/***********************************************************************/
static int mycounter = 0;

void reactmodbus_driver_t::read_thread(void)
{
  while(true)
  {
    printf("read thread reading modbus values . . .\n");
    modbus->read_ai(0, 16, tmp_ai_vals);
    modbus->read_di(0, 16, tmp_di_vals);
    printf("read thread DONE reading modbus values . . .\n");

    printf("read thread copying values . . .\n");
    sem_wait(&read_mutex_sem);
      printf("  -- read thread in critical section . . .\n");
      memcpy(ai_vals, tmp_ai_vals, sizeof(ai_vals));
      memcpy(di_vals, tmp_di_vals, sizeof(di_vals));
      read_values = true;
      printf("  -- read thread leaving critical section . . .\n");
    sem_post(&read_mutex_sem);
    printf("read thread DONE copying values . . .\n");

    printf("read thread waiting %d\n", mycounter++);
    sem_wait(&read_wait_sem);
    printf("thread woke up\n");
  }
}

/***********************************************************************/

void reactmodbus_driver_t::read(void)
{
    sem_wait(&read_mutex_sem);
    printf("  -- main thread in critical section . . .\n");
    if (read_values)
    {
      wake_him_up = true;
    }
    else
    {
      // Either the reader is slow or the link is down
      wake_him_up = false;
    }
  //modbus->read_ai(0, 16, ai_vals);
  //modbus->read_di(0, 16, di_vals);
}

/***********************************************************************/

void reactmodbus_driver_t::end_read(void)
{
  read_values = false;
  printf("  -- main thread leaving critical section . . .\n");
  sem_post(&read_mutex_sem);
  if (wake_him_up)
  {
    printf("waking up reader\n");
    sem_post(&read_wait_sem); // wake up the read thread to read the next values.
    printf("DONE waking up reader\n");
  }
  else
  {
    printf("The read thread is hosed, no reason to keep incrementing . . .\n");
  } 
}

/***********************************************************************/
