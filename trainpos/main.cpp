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
#include <stdlib.h>
#include <time.h>

# include <sys/types.h>
# include <sys/ipc.h>
#include <sys/msg.h>

#include "msg_queue.h"
#include "tcontrol.h"
#include "alg1.h"

#include "event_alg.h"
#include "dc_evalg.h"
#include "gp_evalg.h"

int main(int argc, char *argv[])
{
  time_t last_time = 0;
  /***
  int skip_section = -1;
  bool skip_departure = true;
  ****/

  int qid = create_message_queue();

  display_alg_t dc_alg;
  dc_alg.read_sections(SECTIONS_FILE);

  gp_evalg_t gp_alg;
  gp_alg.init();

  alarm_admin_t admin(0, 0, 0);
  alarm_notify_obj_t ano0(0, &admin);
  alarm_notify_obj_t ano1(1, &admin);
  alarm_notify_obj_t ano2(2, &admin);

  base_alg_t *algs[3];
  algs[0] = new alg1_t;
  algs[1] = new alg1_t;
  algs[2] = new alg1_t;

  event_dispatch_t dispatch(algs, 3);

  crossing_queue_event_t qevent;
  qevent.data.update = false;
  while (true)
  {
    //if (!qevent.data.update)
    //printf("Waiting on message queue %d . . . \n", qid);
    int ret = msgrcv(qid, &qevent, sizeof(qevent.data), 1, 0);
    if (ret == -1)
    {
      perror("Error recieving");
    }
    /****
    if (qevent.data.skip)
    {
      skip_section = qevent.data.section;
      skip_departure = qevent.data.departure;
      continue;
    }
    *****/

    if (!qevent.data.update)
    {
      if (qevent.data.time_stamp == 0)
      {
        qevent.data.time_stamp = last_time;
      }
      struct tm mytm;
      char buf1[30];
      localtime_r(&qevent.data.time_stamp, &mytm);
      strftime(buf1, sizeof(buf1), "%T, %F", &mytm);

      printf("Event: time %s, section %d (%s)\n", buf1, qevent.data.section,
         (qevent.data.departure) ? "departure" : "arival");
    }
    //dispatch.new_event(qevent.data);
    if (qevent.data.update)
    {
      last_time = qevent.data.time_stamp;
      dc_alg.update(qevent.data.time_stamp);
      gp_alg.update(qevent.data.time_stamp);
    }
    else
    {
      dc_alg.process_event(qevent.data);
      gp_alg.process_event(qevent.data);
    }
  }
}

