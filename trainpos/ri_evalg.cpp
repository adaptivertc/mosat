/************************************************************************
This is software to monitor and/or control trains.
Copyright (C) 2005,2006 Do/nald Wayne Carr

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

/**************************************************************************************************/
/* Implementation of ri_log_t                                                                     */
/**************************************************************************************************/

ri_log_t::ri_log_t():m_fpLog(NULL)
{}

ri_log_t::~ri_log_t()
{
  close_log();
}

bool ri_log_t::open_log(const char* szLogPath, const char* szMode)
{
  bool bOpS = false;                                                                                  // <-- bOpS = "Operation succeeded"
  m_fpLog = fopen(szLogPath, szMode);
  if (NULL != m_fpLog)
    bOpS = true;
  else
    perror(szLogPath);
  return bOpS;
}

bool ri_log_t::log(const char* szData, ...)
{
  bool bOpS = false;                                                                                  // <-- bOpS = "Operation succeeded"
  if (NULL != m_fpLog)
  {
    va_list vaLOfArgs;                                                                                // <-- vaLOfArgs = "Variable list of arguments".
    va_start(vaLOfArgs, szData);
    if (vfprintf(m_fpLog, szData, vaLOfArgs) >= 0)
      bOpS = true;
    va_end(vaLOfArgs);
  }
  return bOpS;
}

void ri_log_t::flush_log()
{
  if (NULL != m_fpLog)
    fflush(m_fpLog);
}

void ri_log_t::close_log()
{
  if (NULL != m_fpLog)
  {
    fclose(m_fpLog);
    m_fpLog = NULL;
  }
}

/**************************************************************************************************/
/* Implementation of ri_evalg_t                                                                   */
/**************************************************************************************************/

ri_evalg_t::ri_evalg_t():m_uiStlNum(0), m_uiNumOfSec(0)
{}

void ri_evalg_t::init(void)
{
  if (!m_lgTrainsOp.open_log("ri_log.txt"))
    exit(0);
  m_uiNumOfSec = sections.get_n_sections();
  m_lgTrainsOp.log("Number of sections: %d\n", m_uiNumOfSec);
}

void ri_evalg_t::update(time_t ts)
{
  // Do something!
}

void ri_evalg_t::process_event(crossing_event_t ev)
{
  struct tm ts;
  char buf[30];
  localtime_r(&ev.time_stamp, &ts);
  strftime(buf, sizeof(buf), "%T, %F", &ts);
  std::string stlSzTxtInfo, stlSzHtmlInfo;
  m_lgTabHTML.open_log("ri_log.html");
//  m_lgTrainsOp.log("Event: Time: %s, Section: %d, Type: %s\n", buf, ev.section, ev.departure?"Departure":"Arrival");
  if(ev.departure && 0 == ev.section)                                                                 // <-- If true, a train has just entered service, so it must me added to the list of trains on
  {                                                                                                   //     service.
    ri_train_data_t tmp_train;
    tmp_train.m_uiStlNum = m_uiStlNum++;
    tmp_train.m_tEntryTime = ev.time_stamp;
    tmp_train.m_uiCurSec = 0;
    m_lsTrains.push_front(tmp_train);
    graph_info_txt(stlSzTxtInfo);
    m_lgTrainsOp.log(stlSzTxtInfo.data());
  }
  else if (!ev.departure)                                                                             // <-- If true an arrival has just occurred.
  {
    if(ev.section == m_uiNumOfSec)                                                                    // <-- If true, a train has traversed the entire circuit and is getting out of service so I pop it
      m_lsTrains.pop_back();                                                                          //     from the list.
    std::list<ri_train_data_t>::iterator i = m_lsTrains.begin();
    while(m_lsTrains.end() != i && ev.section - 1 != i->m_uiCurSec)
      ++i;
    i->m_uiCurSec = ev.section;
    graph_info_txt(stlSzTxtInfo);
    m_lgTrainsOp.log(stlSzTxtInfo.data());
  }
  m_lgTrainsOp.flush_log();
  tab_info_html(stlSzHtmlInfo);
  m_lgTabHTML.log(stlSzHtmlInfo.data());
  m_lgTabHTML.close_log();
}

void ri_evalg_t::tab_info_html(std::string &stlSzHtmlInfo)                                            // <-- stlSzHtmlInfo = "Stl string containing train's tabular information formated as HTML"
{
  stlSzHtmlInfo = HTML_F_FIXED_SECTION;
  char szTableRow[SIZE_HTML_TABLE_ROW + 30];                                                          // <-- TODO: I need to change this hardcoded 30 for an appropriate macro
  std::list<ri_train_data_t>::iterator i;
  for (i = m_lsTrains.begin(); i != m_lsTrains.end(); ++i)                                            // <-- My loop starts right here
  {
    struct tm ts;
    char buf[30];
    localtime_r(&i->m_tEntryTime, &ts);
    strftime(buf, sizeof(buf), "%T, %F", &ts);
    sprintf(szTableRow, HTML_TABLE_ROW, i->m_uiStlNum, buf, i->m_uiCurSec);
    stlSzHtmlInfo += szTableRow;
  }                                                                                                   // <-- My loop ends right here
  stlSzHtmlInfo += HTML_S_FIXED_SECTION;
}

void ri_evalg_t::graph_info_txt(std::string &stlSzTxtInfo)
{
  unsigned uiNextPOS = 0, uiPrevTS = 0;                                                               // <-- uiNextPOS = "Next possible occupied section". uiPrevTS = "Section the previous train occupies".
  char szStlNum[10];                                                                                  // <-- TODO: I need to change this hardcoded 10 for an appropriate macro
  stlSzTxtInfo = "|[";
  std::list<ri_train_data_t>::iterator i = m_lsTrains.begin();
  for (i = m_lsTrains.begin(); i != m_lsTrains.end(); ++i)
  {
    for (unsigned uiNES = i->m_uiCurSec; uiNES > uiNextPOS; --uiNES)                                  // <-- uiNES = "Number of empty sections"
      stlSzTxtInfo += "-][";
    if (i != m_lsTrains.begin() && i->m_uiCurSec == uiPrevTS)                                         // <-- This normally shouldn't happen because it means there is more than one train occuping the
      sprintf(szStlNum, "\b\b,%d][", i->m_uiStlNum);                                                  //     same section and, even though that's a possible condition, it's considered dangerous.
    else
    {
      sprintf(szStlNum, "%d][", i->m_uiStlNum);
      uiNextPOS = i->m_uiCurSec + 1;
    }
    stlSzTxtInfo += szStlNum;
    uiPrevTS = i->m_uiCurSec;
  }
  for (unsigned uiRES = uiNextPOS; uiRES < m_uiNumOfSec; ++uiRES)                                     // <-- uiRES = "Number of remaining empty sections ahead of the last occupied section".
    stlSzTxtInfo += "-][";
  stlSzTxtInfo += "\b|\n";
}

/**************************************************************************************************/
/* Implementation of ri_time_table_t                                                              */
/**************************************************************************************************/

// ri_time_table_t::ri_time_table_t():m_fpTimeTbl(NULL)
// {}

// ri_time_table_t::~ri_time_table_t()
// {
//   clear_table();
// }

void ri_time_table_t::clear_table()
{
  m_lsTimeTbl.clear();
}

bool ri_time_table_t::load_table(const char *szTimeTblPath)
{
   bool bOpS = false;
//   FILE *fpTimeTbl = fopen(szTimeTblPath, "r");
//   if (NULL != fpTimeTbl)
//   {
//     char szData[2] = {0,0};
//     std::string stlSzTrainID, stlEnterTime;
//     szData[0] = (char)getc(fpTimeTbl);
//     bool bIDRS = false, bETRS = false;                                                                // <-- bIDRS = "ID retrieved successfully". bETRS = "Enter time retrieved successfully".
//     while (szData[0] != EOF)
//     {
//       while ('\t' != szData[0] && EOF != szData[0]);
//       {
//         stlSzTrainID += szData;
//         szData[0] = (char)getc(fpTimeTbl);
//         bIDRS = true;
//       }
//       szData[0] = (char)getc(fpTimeTbl);
//       while ('\n' != szData[0] && EOF != szData[0]);
//       {
//         stlEnterTime += szData;
//         szData[0] = (char)getc(fpTimeTbl);
//       }
//       szData[0] = (char)getc(fpTimeTbl);
//     }
//     fclose(fpTimeTbl);
//   }
//   else
//     perror(szTimeTblPath);
  return bOpS;
}