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
#include <time.h>
#include <stdlib.h>
#include <unistd.h>

#include "msg_queue.h"
#include "tcontrol.h"
#include "event_alg.h"
#include "section_reader.h"
#include "trainpos.h"
#include "ri_evalg.h"

#include "rtcommon.h"
#include "arg.h"

/***********************************
BVL9 	This train is 33 seconds late tell him t**********************/

void ri_evalg_t::init(void)
{
   fp = fopen("ri_log.txt", "w");
   if (fp == NULL)
	{
	perror("ri_log.txt");
	exit(0);
	}
   m_uiNumOfSections = sections.get_n_sections();
   fprintf(fp, "Number of sections: %d\n", m_uiNumOfSections); 
   fflush(fp);
}

/*********************************************************/

/*********************************************************/

void ri_evalg_t::update(time_t ts)
{
   // Do something!
}

/*********************************************************/

/*********************************************************/

void ri_evalg_t::process_event(crossing_event_t ev)
{
   struct tm ts;
   char buf[30];
   localtime_r(&ev.time_stamp, &ts);
   strftime(buf, sizeof(buf), "%T, %F", &ts);
   fprintf(fp, "Event: Time: %s, Section: %d, Type: %s\n", buf, ev.section, ev.departure?"Departure":"Arrival");
   if(ev.section == 0 && ev.departure)
	fprintf(fp, "Oh my God !... that was a train entering service\n");
   else if(ev.section == (m_uiNumOfSections -1) && !ev.departure)
	fprintf(fp, "On my God ! ... that was a train getting out of service\n"); 
   fflush(fp);  
}

/*********************************************************/

