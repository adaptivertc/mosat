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

#define _XOPEN_SOURCE /* glibc2 needs this */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>

# include <sys/types.h>
# include <sys/ipc.h>
#include <sys/msg.h>

#include "msg_queue.h"
#include "tcontrol.h"
#include "train_sim.h"
#include "rtcommon.h"
#include "sim_msg.h"

#include "tpconfig.h"

tp_config_t tpconfig;

class my_notify_t : public sim_ev_notify_t
{
public:
  void trigger_arrival(int section, time_t now);
  void trigger_departure(int section, time_t now);
};


static int qid;

/********************************************************/

void my_notify_t::trigger_arrival(int section, time_t now)
{
  char buf[30];
  struct tm mytm;
  localtime_r(&now, &mytm);
  strftime(buf, sizeof(buf), "%T", &mytm);

  crossing_queue_event_t msgbuf;
  msgbuf.mtype = 1;
  msgbuf.data.time_stamp = now;
  msgbuf.data.section = section;
  msgbuf.data.departure = false;
  msgbuf.data.update = false;
  int ret = msgsnd(qid, &msgbuf, sizeof(msgbuf.data), 0);
  if (ret == -1)
  {
    perror("Could not send message");
    exit(0);
  }
  printf("Arrival(%s): %d\n", buf, section);
}

void my_notify_t::trigger_departure(int section, time_t now)
{
  char buf[30];
  struct tm mytm;
  localtime_r(&now, &mytm);
  strftime(buf, sizeof(buf), "%T", &mytm);

  crossing_queue_event_t msgbuf;
  msgbuf.mtype = 1;
  msgbuf.data.time_stamp = now;
  msgbuf.data.section = section;
  msgbuf.data.departure = true;
  msgbuf.data.update = false;
  msgbuf.data.skip = false;
  int ret = msgsnd(qid, &msgbuf, sizeof(msgbuf.data), 0);
  if (ret == -1)
  {
    perror("Could not send message");
    exit(0);
  }
  printf("Departure(%s): %d\n", buf, section);
}

/********************************************************/

void send_update(time_t now)
{
  crossing_queue_event_t msgbuf;
  msgbuf.mtype = 1;
  msgbuf.data.time_stamp = now;
  msgbuf.data.section = 0;
  msgbuf.data.departure = false;
  msgbuf.data.update = true;
  msgbuf.data.skip = false;
  int ret = msgsnd(qid, &msgbuf, sizeof(msgbuf.data), 0);
  if (ret == -1)
  {
    perror("Could not send message");
    exit(0);
  }
}

/********************************************************/

int main(int argc, char *argv[])
{
  tpconfig.read_file("tpconfig.txt");

  bool use_actual_time = false;

  qid = connect_message_queue();
  create_sim_queue();

  time_t now;
  time_t start;
  class my_notify_t my_notify;
  class train_sim_t sim(&my_notify);
  now = sim.read_day();
  start = now;
  sim.read_sections(SECTIONS_FILE);
  long wait_time = 100000;
  if (argc > 1)
  {
    if (0 == strcmp(argv[1], "-rt")) 
    {
      wait_time = 1000000;
      use_actual_time = true;
    }
  }

  if (use_actual_time)
  {
    while(true)
    {
      sim.update(now);
      send_update(now);
      now++;
      if (now > time(NULL)) break;
    }
  }


  sim_msg_t sim_msg;
  int i = 0;
  while(true)
  {
    //printf("-------------------- %d\n", i);
    sim.update(now);
    send_update(now);
 
    if (0 == check_for_sim_msg(&sim_msg))
    {
      printf("sim command recieved: type %d, section %d, sensor %d\n",
         sim_msg.type, sim_msg.section, sim_msg.sensor);
      if (sim_msg.type == 1)
      {
         if (sim_msg.sensor == 0)
         {
           printf("----------triggering departure at section %d\n", sim_msg.section);
           my_notify.trigger_departure(sim_msg.section, now);
         }
         else if (sim_msg.sensor == 1)
         {
           printf("----------triggering arrival at section %d\n", sim_msg.section + 1);
           my_notify.trigger_arrival(sim_msg.section + 1, now);
         }
      }
    } 
    else
    {
      //printf("no sim message . . . \n");
    } 

    if (use_actual_time)
    {
      now = time(NULL);
      usleep(wait_time);
    }
    else
    {
      now++;
      if (i > 5400) usleep(wait_time);
    }


    i++;
  }
}

/********************************************************/



