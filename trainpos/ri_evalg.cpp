/************************************************************************
This is software to monitor and/or control trains.
Copyright (C) 2005,2006 Do/nald Wayne Carr

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
#include <string>
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
  FILE* fp;

  fp = fopen("ri_log.html", "w");
  if (NULL == fp)
  {
    perror("ri_log.html");
    exit(0);
  }
  fprintf(fp, HTML_F_FIXED_SECTION);
  std::list<ri_train_data_t>::iterator i;
  for (i = m_lsTrains.begin(); i != m_lsTrains.end(); ++i)                                            // <-- My loop starts right here ! !!
  {
    struct tm ts;
    char buf[30];
    
    localtime_r(&i->m_tEntryTime, &ts);
    strftime(buf, sizeof(buf), "%T, %F", &ts);
    fprintf(fp, HTML_TABLE_ROW, i->m_uiStlNum, buf, i->m_uiCurSec);
  }                                                                                                   // <-- My loop ends right here ! !!
  fprintf(fp, HTML_S_FIXED_SECTION);
  fclose(fp);
}

ri_evalg_t::ri_evalg_t():m_uiNumOfSec(0), m_uiStlNum(0), m_fpLogFile(NULL)
{}

ri_evalg_t::~ri_evalg_t()
{
  if (NULL != m_fpLogFile)
    fclose(m_fpLogFile);
}

void ri_evalg_t::init(void)
{
  m_fpLogFile = fopen("ri_log.txt", "w");
  if (NULL == m_fpLogFile)
  {
    perror("ri_log.txt");
    exit(0);
  }
  m_uiNumOfSec = sections.get_n_sections();
  fprintf(m_fpLogFile, "Number of sections: %d\n", m_uiNumOfSec);
  fflush(m_fpLogFile);
}

void ri_evalg_t::update(time_t ts)
{
  // Do something!
}

void ri_evalg_t::process_event(crossing_event_t ev)
{
  struct tm ts;
  char buf[30];
  std::list<ri_train_data_t>::iterator i;

  localtime_r(&ev.time_stamp, &ts);
  strftime(buf, sizeof(buf), "%T, %F", &ts);
//  fprintf(m_fpLogFile, "Event: Time: %s, Section: %d, Type: %s\n", buf, ev.section, ev.departure?"Departure":"Arrival");
  if(ev.departure)
  {
    if(ev.section == 0)
    {
      ri_train_data_t tmp_train;

      tmp_train.m_uiStlNum = m_uiStlNum++;
      tmp_train.m_tEntryTime = ev.time_stamp;
      tmp_train.m_uiCurSec = 0;
      m_lsTrains.push_front(tmp_train);
    }
  }
  else 
  {
    for (i = m_lsTrains.begin(); m_lsTrains.end() != i; ++i)
      if (ev.section - 1 == i->m_uiCurSec)
      {
        i->m_uiCurSec = ev.section;
        break;
      }
    if(ev.section == (m_uiNumOfSec/*-1*/))
    {
//      fprintf(m_fpLogFile, "On my God ! ... that was a train getting out of service\n");
      m_lsTrains.pop_back();
    }
  }
  char szStlNum[3];
  unsigned uiNextSec = 35;
  unsigned uiNumOfTrains = m_lsTrains.size();
  std::string szPositions;
  i = m_lsTrains.end();
  for (--i; uiNumOfTrains > 0; --i, --uiNumOfTrains)
  {
    for (unsigned j = i->m_uiCurSec; j < uiNextSec; ++j)
      szPositions += "[-]"; 
    sprintf(szStlNum, "[%d]", i->m_uiStlNum);
    szPositions += szStlNum;
    uiNextSec = i->m_uiCurSec;    
  }
  for (unsigned j = uiNextSec; j > 0; --j)
    szPositions += "[-]";
  szPositions += "\n";
  fprintf(m_fpLogFile, szPositions.data());
  fflush(m_fpLogFile);
  gen_htmlreport();
}

