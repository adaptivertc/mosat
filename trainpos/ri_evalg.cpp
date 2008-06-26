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
  std::list<ri_train_data_t>::iterator i = m_lsTrains.begin();

  localtime_r(&ev.time_stamp, &ts);
  strftime(buf, sizeof(buf), "%T, %F", &ts);
//  fprintf(m_fpLogFile, "Event: Time: %s, Section: %d, Type: %s\n", buf, ev.section, ev.departure?"Departure":"Arrival");
  if(ev.departure && 0 == ev.section)                                                                 // <-- If true, a train has just started opperating so it must me added to the list of trains on
  {                                                                                                   //     service.
    ri_train_data_t tmp_train;

    tmp_train.m_uiStlNum = m_uiStlNum++;
    tmp_train.m_tEntryTime = ev.time_stamp;
    tmp_train.m_uiCurSec = 0;
    m_lsTrains.push_front(tmp_train);
  }
  else
  {
    if(ev.section == m_uiNumOfSec)                                                                    // <-- If true, a train has traversed the entire circuit and is getting out of service so I pop it
      m_lsTrains.pop_back();                                                                          //     from the list.
    while(m_lsTrains.end() != i && ev.section - 1 != i->m_uiCurSec)
      ++i;
    i->m_uiCurSec = ev.section;
  }

  char szStlNum[10];                                                                                  // <-- TODO: I need to change this hardcoded 10 for a macro
  unsigned uiNextPOS = 0;                                                                             // <-- uiExpTrainPos = "Next possible occupied section".
  int iPrevTS = -1;                                                                                   // <-- iPrevTS = "Section that the previous train occupies". -1 means there is not a previous train.
  std::string szPositions = "|[";

  for (i = m_lsTrains.begin(); i != m_lsTrains.end(); ++i)
  {
    for (unsigned uiNES = i->m_uiCurSec; uiNES > 0 && uiNES > uiNextPOS; --uiNES)                     // <-- uiNES = "Number of empty sections"
      szPositions += "-][";
    if (i->m_uiCurSec == iPrevTS)                                                                     // <-- This shouldn't normally happen because it means there is more than one train ocupping the
      sprintf(szStlNum, "\b\b,%d][", i->m_uiStlNum);                                                  //     the same section, and that is a condition that is possible but considered dangerous.
    else
    {
      sprintf(szStlNum, "%d][", i->m_uiStlNum);
      uiNextPOS = i->m_uiCurSec + 1;
    }
    szPositions += szStlNum;
    iPrevTS == i->m_uiCurSec;
  }
  for (unsigned uiRES = uiNextPOS; uiRES < 36; ++uiRES)                                               // <-- uiRES = "Number of remaining empty sections after the last occupied section".
    szPositions += "-][";                                                                             //     TODO: I need to change this hardcoded 36 for a macro
  szPositions += "\b|\n";
  fprintf(m_fpLogFile, szPositions.data());
  fflush(m_fpLogFile);
  gen_htmlreport();
}

