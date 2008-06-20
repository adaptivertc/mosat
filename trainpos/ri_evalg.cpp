/************************************************************************
This is software to monitor and/or control trains.
Copyright (C) 2005,2006 Donald Wayne Carr 

This program is free software; you can redistribute it and/or modify 
it under the terms of the GNU General Public License as published by 
the Free Software Foundation; either version 2 of the License, or 
(at your option) any later version.
	struct tm ts;
	char buf[30];
	localtime_r(&ev.time_stamp, &ts);
	strftime(buf, sizeof(buf), "%T, %F", &ts);
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


void ri_evalg_t::gen_htmlreport()
{
	FILE * fp;
        fp = fopen("ri_log.html", "w");
	if (fp == NULL)
	{
		perror("ri_log.html");
		exit(0);
	}
	 
	fprintf(fp, "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0 Transitional//EN\">\n");
	fprintf(fp, "<HTML>\n");
	fprintf(fp, "<HEAD>\n");
	fprintf(fp, "	<META HTTP-EQUIV=\"refresh\" CONTENT=\"1\">\n");
	fprintf(fp, "	<meta http-equiv=\"pragma\" content=\"no-cache>\"\n");
	fprintf(fp, "	<META HTTP-EQUIV=\"CONTENT-TYPE\" CONTENT=\"text/html; charset=utf-8\">\n");
	fprintf(fp, "	<TITLE></TITLE>\n");
	fprintf(fp, "	<META NAME=\"GENERATOR\" CONTENT=\"OpenOffice.org 2.3  (Linux)\">\n");
	fprintf(fp, "	<META NAME=\"AUTHOR\" CONTENT=\"Rub&eacute;n Gonz&aacute;lez\">\n");
	fprintf(fp, "	<META NAME=\"CREATED\" CONTENT=\"20080619;20154000\">\n");
	fprintf(fp, "	<META NAME=\"CHANGEDBY\" CONTENT=\"Rub&eacute;n Gonz&aacute;lez\">\n");
	fprintf(fp, "	<META NAME=\"CHANGED\" CONTENT=\"20080619;20214800\">\n");
	fprintf(fp, "	<STYLE TYPE=\"text/css\">\n");
	fprintf(fp, "	<!--\n");
	fprintf(fp, "		@page { size: 8.5in 11in; margin: 0.79in }\n");
	fprintf(fp, "		P { margin-bottom: 0.08in }\n");
	fprintf(fp, "		TD P { margin-bottom: 0in }\n");
	fprintf(fp, "		TH P { margin-bottom: 0in }\n");
	fprintf(fp, "	-->\n");
	fprintf(fp, "	</STYLE>\n");
	fprintf(fp, "</HEAD>\n");
	fprintf(fp, "<BODY LANG=\"en-US\" DIR=\"LTR\">\n");
	fprintf(fp, "<TABLE WIDTH=100%% BORDER=1 BORDERCOLOR=\"#000000\" CELLPADDING=4 CELLSPACING=0>\n");
	fprintf(fp, "	<COL WIDTH=85*>\n");
	fprintf(fp, "	<COL WIDTH=85*>\n");
	fprintf(fp, "	<COL WIDTH=85*>\n");
	fprintf(fp, "	<TR VALIGN=TOP>\n");
	fprintf(fp, "		<TH WIDTH=33%% BGCOLOR=\"#000080\">\n");
	fprintf(fp, "			<P><FONT COLOR=\"#ffffff\"><SPAN STYLE=\"background: #000080\">N&uacute;mero\n");
	fprintf(fp, "			Secuencial</SPAN></FONT></P>\n");
	fprintf(fp, "\n");
	fprintf(fp, "		</TH>\n");
	fprintf(fp, "		<TH WIDTH=33%% BGCOLOR=\"#000080\">\n");
	fprintf(fp, "			<P><FONT COLOR=\"#ffffff\"><SPAN STYLE=\"background: #000080\">Tiempo\n");
	fprintf(fp, "			de entrada en servicio</SPAN></FONT></P>\n");
	fprintf(fp, "		</TH>\n");
	fprintf(fp, "		<TH WIDTH=33%% BGCOLOR=\"#000080\">\n");
	fprintf(fp, "			<P><FONT COLOR=\"#ffffff\"><SPAN STYLE=\"background: #000080\">Segmento\n");
	fprintf(fp, "			actual</SPAN></FONT></P>\n");
	fprintf(fp, "		</TH>\n");
	fprintf(fp, "	</TR>\n");
	fprintf(fp, "\n");
	fprintf(fp, "	<TR VALIGN=TOP>\n");
	fprintf(fp, "		<TD WIDTH=33%%>\n");
	fprintf(fp, "			<P ALIGN=CENTER>0</P>\n");
	fprintf(fp, "		</TD>\n");
	fprintf(fp, "		<TD WIDTH=33%%>\n");
	fprintf(fp, "			<P ALIGN=CENTER>00:00:00</P>\n");
	fprintf(fp, "		</TD>\n");
	fprintf(fp, "		<TD WIDTH=33%%>\n");
	fprintf(fp, "\n");
	fprintf(fp, "			<P ALIGN=CENTER>0</P>\n");
	fprintf(fp, "		</TD>\n");
	fprintf(fp, "	</TR>\n");

	// I need to insert my loop right here ! !!
       	std::list<ri_train_data_t>::iterator i;
	for (i = train_data_array.begin(); i != train_data_array.end(); ++i)
	{
		fprintf(fp, "	<TR VALIGN=TOP>\n");
		fprintf(fp, "		<TD WIDTH=33%%>\n");
		fprintf(fp, "			<P ALIGN=CENTER>%d</P>\n", i->sec_num);
		fprintf(fp, "		</TD>\n");
		fprintf(fp, "		<TD WIDTH=33%%>\n");
		fprintf(fp, "\n");
		struct tm ts;
		char buf[30];
		localtime_r(&i->entry_time, &ts);
		strftime(buf, sizeof(buf), "%T, %F", &ts);
		fprintf(fp, "			<P ALIGN=CENTER>%s</P>\n", buf);
		fprintf(fp, "		</TD>\n");
		fprintf(fp, "		<TD WIDTH=33%%>\n");
		fprintf(fp, "			<P ALIGN=CENTER>%d</P>\n", i->cur_seg);
		fprintf(fp, "		</TD>\n");
		fprintf(fp, "	</TR>\n");
	}
 	// My loop ends right here ! !!
	fprintf(fp, "</TABLE>\n");
	fprintf(fp, "<P STYLE=\"margin-bottom: 0in\"><BR>\n");
	fprintf(fp, "</P>\n");
	fprintf(fp, "\n");
	fprintf(fp, "</BODY>\n");
	fprintf(fp, "</HTML>\n");
	fclose(fp);
}

/***********************************
BVL9 	This train is 33 seconds late tell him t**********************/

ri_evalg_t::ri_evalg_t():m_uiSecNum(0)					// m_uiSecNum = Number of sections in the system.
{}

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
   gen_htmlreport();
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
	{
		ri_train_data_t tmp_train;
		tmp_train.sec_num = m_uiSecNum;				// <-- At this point I'm initialiazing my  ri_train_data_t structure to insert it on my list
		tmp_train.entry_time = ev.time_stamp;
		tmp_train.cur_seg = 0;
		++m_uiSecNum;
		fprintf(fp, "Oh my God !... that was a train entering service\n");
		train_data_array.push_front(tmp_train);
		std::list<ri_train_data_t>::iterator i;
		for (i = train_data_array.begin(); i != train_data_array.end(); ++i)
			fprintf(fp, "Train Secuential Num: %d\n", i->sec_num);
		gen_htmlreport();
	}
	else if(ev.section == (m_uiNumOfSections -1) && !ev.departure)						
	{
		fprintf(fp, "On my God ! ... that was a train getting out of service\n");
		train_data_array.pop_back();
		fprintf(fp, "Remaining trains:\n");

		std::list<ri_train_data_t>::iterator i;
		for (i = train_data_array.begin(); i != train_data_array.end(); ++i)
			fprintf(fp, "Train Secuential Num: %d\n", i->sec_num);
		gen_htmlreport();
	}
	fflush(fp);
}

/*********************************************************/

