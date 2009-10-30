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
#include <iostream>

/**************************************************************************************************/
/* Implementation of ri_evalg_t                                                                   */
/**************************************************************************************************/

ri_evalg_t::ri_evalg_t(): m_secNum(0)
{
  m_Logger.open("ri_log.txt");
  m_Logger << "BEGINNING OF LOG.\n";
}

ri_evalg_t::~ri_evalg_t()
{
  m_Logger << "END OF LOG. ";
  m_Logger.close();
}

void ri_evalg_t::init(const char* config_file)
{
  m_Logger << INDENT << "Entering ri_evalg_t::init...\n";
  m_Logger.increaseIndent();
  ri_cfReader cfReader(m_Logger);
  TPRC_ENUM rCode = cfReader.readTPConfig(config_file);
  if (TPRC_OS == rCode)
  {
    rCode = cfReader.readTimeTable(m_timeTable);
    if (TPRC_OS == rCode)
    {
      rCode = cfReader.readSections(m_listOfSections);
      if (TPRC_OS == rCode)
        rCode = cfReader.readDisplay();
    }
  }
  m_Logger.decreaseIndent();
  m_Logger << INDENT << "Exiting ri_evalg_t::init.\n";
}

void ri_evalg_t::update(time_t ts)                                                                    // <-- This method is used to update de position of the trains with estimated position since it's called even when an event has not happend.
{

  for (LinkedList<ri_rwSection>::Iterator secIterator = m_listOfSections.NewIterator();
    !secIterator.EndReached(); secIterator++)
    if (secIterator->isBusy())
      secIterator->updatePos();
  //updateTabInfo();
}

void ri_evalg_t::process_event(crossing_event_t ev)
{
  m_Logger << INDENT << "Entering ri_evalg_t::process_event...\n";
  m_Logger.increaseIndent();
  ri_trainData trainData;
  ri_rwSection& curSection = m_listOfSections.Retrieve(ev.section);
  //tm ts;
  //char buf[30];
  //localtime_r(&ev.time_stamp, &ts);
  //strftime(buf, sizeof(buf), "%T, %F", &ts);
  //ri_str sTxtInfo, sHtmlInfo;
  //printf("Evento en section: %s, sensor: %d\n", (const char*)m_listOfSections.Retrieve(ev.section).sectionName(), ev.departure ? 0 : 1);
  if (ev.departure)
  {
    if (0 == ev.section)
    {
      curSection.procTES(m_secNum, time(NULL)/*ev.time_stamp*/);
      m_secNum++;
    }
    else
      curSection.procTCS(0);
  }
  else
    curSection.procSCE(m_listOfSections.Retrieve(ev.section-1));
//   if(ev.departure && 0 == ev.section)                                                                 // <-- If true, a train has just entered service, so it must me added to the list of trains on
//   {                                                                                                   //     service.
//     ri_train_data_t tmp_train;
//     tmp_train.m_secNum = m_secNum++;
//     tmp_train.m_tEntryTime = ev.time_stamp;
//     tmp_train.m_uiCurSec = 0;
//     m_lsTrains.push_front(tmp_train);
//    }
//   else if (!ev.departure)                                                                             // <-- If true an arrival has just occurred.
//   {
//     if(ev.section == m_listOfSections.Size())                                                                    // <-- If true, a train has traversed the entire circuit and is getting out of service so I pop it
//       m_lsTrains.pop_back();                                                                          //     from the list.
//     std::list<ri_train_data_t>::iterator i = m_lsTrains.begin();
//     while(m_lsTrains.end() != i && ev.section - 1 != i->m_uiCurSec)
//       ++i;
//     i->m_uiCurSec = ev.section;
//  }
  for (LinkedList<ri_rwSection>::Iterator ss = m_listOfSections.NewIterator(); !ss.EndReached(); ss++)
    if (ss->isBusy())
      m_Logger << INDENT << "Section: " << ss->name() << "Train secnum:" << trainData.m_secNum << " Train position: " << trainData.m_cPos << "\n";
  updateTabInfo();
  m_Logger.flush();
  m_Logger.decreaseIndent();
  m_Logger << INDENT << "Exiting ri_evalg_t::process_event.\n";
}

void ri_evalg_t::tab_info_html(ri_str& sHtmlInfo)                                            // <-- sHtmlInfo = "Stl string containing train's tabular information formated as HTML"
{
//   sHtmlInfo = HTML_F_FIXED_SECTION;
//   char szTableRow[SIZE_HTML_TABLE_ROW + 30];                                                          // <-- TODO: I need to change this hardcoded 30 for an appropriate macro
//   std::list<ri_train_data_t>::iterator i;
//   for (i = m_lsTrains.begin(); i != m_lsTrains.end(); ++i)                                            // <-- My loop starts right here
//   {
//     struct tm ts;
//     char buf[30];
//     localtime_r(&i->m_tEntryTime, &ts);
//     strftime(buf, sizeof(buf), "%T, %F", &ts);
//     sprintf(szTableRow, HTML_TABLE_ROW, i->m_secNum, buf, i->m_uiCurSec);
//     sHtmlInfo += szTableRow;
//   }                                                                                                   // <-- My loop ends right here
//   sHtmlInfo += HTML_S_FIXED_SECTION;
}

void ri_evalg_t::graph_info_txt(ri_str& sTxtInfo)
{
/*  unsigned uiNextPOS = 0, uiPrevTS = 0;                                                               // <-- uiNextPOS = "Next possible occupied section". uiPrevTS = "Section the previous train occupies".
  char szStlNum[10];                                                                                  // <-- TODO: I need to change this hardcoded 10 for an appropriate macro
  sTxtInfo = "|[";
  std::list<ri_train_data_t>::iterator i = m_lsTrains.begin();
  for (i = m_lsTrains.begin(); i != m_lsTrains.end(); ++i)
  {
    for (unsigned uiNES = i->m_uiCurSec; uiNES > uiNextPOS; --uiNES)                                  // <-- uiNES = "Number of empty sections"
      sTxtInfo += "-][";
    if (i != m_lsTrains.begin() && i->m_uiCurSec == uiPrevTS)                                         // <-- This normally shouldn't happen because it means there is more than one train occuping the
      sprintf(szStlNum, "\b\b,%d][", i->m_secNum);                                                  //     same section and, even though that's a possible condition, it's considered dangerous.
    else
    {
      sprintf(szStlNum, "%d][", i->m_secNum);
      uiNextPOS = i->m_uiCurSec + 1;
    }
    sTxtInfo += szStlNum;
    uiPrevTS = i->m_uiCurSec;
  }
  for (unsigned uiRES = uiNextPOS; uiRES < m_listOfSections.Size(); ++uiRES)                                     // <-- uiRES = "Number of remaining empty sections ahead of the last occupied section".
    sTxtInfo += "-][";
  sTxtInfo += "\b|\n";*/
}

void ri_evalg_t::updateTabInfo()
{
  TPRC_ENUM rCode;
  //m_Logger << INDENT << "Entering ri_evalg_t::updateTabInfo...\n";
  m_Logger.increaseIndent();
  //m_Logger << INDENT << "Opening ri_tabinfo.htm...\n";
  rCode = m_tabInfo.open("ri_tabinfo.htm");
  if (TPRC_OS != rCode)
    m_Logger << INDENT << "Oops!. " << getTPRCDesc(rCode);
  else
  {
    //m_Logger << INDENT << "Updating ri_tabinfo.htm...\n";
    m_tabInfo << HTML_F_FIXED_SECTION;
  //   m_tpTabIngo << "      <TR VALIGN=TOP>\n        <TD WIDTH=33%%>\n";
  //   m_tpTabIngo << "          <P ALIGN=CENTER>" << %d << "</P>\n        </TD>\n";
  //   m_tpTabIngo << "        <TD WIDTH=33%%>\n          <P ALIGN=CENTER>" << %s << "</P>\n";
  //   m_tpTabIngo << "        </TD>\n        <TD WIDTH=33%%>\n          <P ALIGN=CENTER>" << %d;
  //   m_tpTabIngo << "</P>\n        </TD>\n      </TR>\n";
    m_tabInfo << HTML_S_FIXED_SECTION;
    //m_Logger << INDENT << "ri_tabinfo.htm updated.\n";
    //m_Logger << INDENT << "Closing ri_tabinfo.htm.\n";
    m_tabInfo.close();
  }
  m_Logger.decreaseIndent();
  //m_Logger << INDENT << "Exiting ri_evalg_t::updateTabInfo.\n";
}