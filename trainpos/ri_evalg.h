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

#include <list>
#include <string>
#include <stdarg.h>

/**************************************************************************************************/
/* Macros to create the HTML report I use for debugging purposes                                  */
/**************************************************************************************************/

#define HTML_F_FIXED_SECTION \
  "  <!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0 Transitional//EN\">\n\
  <HTML>\n\
  <HEAD>\n\
    <META HTTP-EQUIV=\"refresh\" CONTENT=\"1\">\n\
    <meta http-equiv=\"pragma\" content=\"no-cache>\"\n\
    <META HTTP-EQUIV=\"CONTENT-TYPE\" CONTENT=\"text/html; charset=utf-8\">\n\
    <TITLE></TITLE>\n\
    <STYLE TYPE=\"text/css\">\n\
    <!--\n\
      @page { size: 8.5in 11in; margin: 0.79in }\n\
      P { margin-bottom: 0.08in }\n\
      TD P { margin-bottom: 0in }\n\
      TH P { margin-bottom: 0in }\n\
    -->\n\
    </STYLE>\n\
  </HEAD>\n\
  <BODY LANG=\"en-US\" DIR=\"LTR\">\n\
    <TABLE WIDTH=100%% BORDER=1 BORDERCOLOR=\"#000000\" CELLPADDING=4 CELLSPACING=0>\n\
      <COL WIDTH=85*>\n\
      <COL WIDTH=85*>\n\
      <COL WIDTH=85*>\n\
      <TR VALIGN=TOP>\n\
        <TH WIDTH=33%% BGCOLOR=\"#000080\">\n\
          <P><FONT COLOR=\"#ffffff\"><SPAN STYLE=\"background: #000080\">N&uacute;mero\n\
          Secuencial</SPAN></FONT></P>\n\
        </TH>\n\
        <TH WIDTH=33%% BGCOLOR=\"#000080\">\n\
          <P><FONT COLOR=\"#ffffff\"><SPAN STYLE=\"background: #000080\">Tiempo\n\
          de entrada en servicio</SPAN></FONT></P>\n\
        </TH>\n\
        <TH WIDTH=33%% BGCOLOR=\"#000080\">\n\
          <P><FONT COLOR=\"#ffffff\"><SPAN STYLE=\"background: #000080\">Segmento\n\
          actual</SPAN></FONT></P>\n\
        </TH>\n\
      </TR>\n"
#define HTML_TABLE_ROW \
  "      <TR VALIGN=TOP>\n\
        <TD WIDTH=33%%>\n\
          <P ALIGN=CENTER>%d</P>\n\
        </TD>\n\
        <TD WIDTH=33%%>\n\
          <P ALIGN=CENTER>%s</P>\n\
        </TD>\n\
        <TD WIDTH=33%%>\n\
          <P ALIGN=CENTER>%d</P>\n\
        </TD>\n\
      </TR>\n"
#define SIZE_HTML_TABLE_ROW sizeof(HTML_TABLE_ROW)
#define HTML_S_FIXED_SECTION \
  "    </TABLE>\n\
  </BODY>\n\
  </HTML>\n"

/**************************************************************************************************/
/*                                                                                                */
/**************************************************************************************************/

struct ri_train_data_t
{
  unsigned m_uiStlNum;                                                                                // m_uiStlNum = "Secuential number assigned to this train"
  unsigned m_uiCurSec;                                                                                // m_uiCurSec = "Section where a this train currently is"
  time_t m_tEntryTime;                                                                                // m_tEntryTime = "Time this train entered service"
};

/**************************************************************************************************/
/*                                                                                                */
/**************************************************************************************************/

class ri_log_t
{
  private:
  FILE *m_fpLog;
  public:
  ri_log_t();
  ~ri_log_t();
  bool open_log(const char *szLogPath, const char *szMode = "w");
  bool log(const char *szData, ...);
  void flush_log();
  void close_log();
};

/**************************************************************************************************/
/*                                                                                                */
/**************************************************************************************************/

class ri_evalg_t : public event_alg_t
{
  private:
  unsigned m_uiStlNum;                                                                                // m_uiStlNum = "Secuential number", a consecutive value assigned to every train entering service.
  unsigned m_uiNumOfSec;                                                                              // m_uiNumOfSec = "Number of sections on the circuit"
  ri_log_t m_lgTrainsOp;                                                                              // m_lgTrainsOp = "Log of train's operation"
  ri_log_t m_lgTabHTML;                                                                               // m_lgTabHTML = "HTML document with trains' tabular information"
  std::list<ri_train_data_t> m_lsTrains;                                                              // m_lsTrains = "List of trains on operation"
  public:
  ri_evalg_t();
  void init(void);
  void update(time_t time);
  void process_event(crossing_event_t ev);
  void tab_info_html(std::string &stlSzHtmlInfo);                                                     // stlSzHtmlInfo = "STL string with tabular info of the trains, formated as html"
  void graph_info_txt(std::string &stlSzTxtInfo);                                                     // stlSzTxtInfo = "STL string with graphic info of the trains, formated as text"
};

/**************************************************************************************************/
/*                                                                                                */
/**************************************************************************************************/

struct ri_time_table_item
{
  std::string m_stlSzTrainID;                                                                           // stlSzTrainID = "ID of a train that is entering service"
  tm m_tmEnterTime;                                                                                     // tmEnterTime = "Time the train is entering service"
};

/**************************************************************************************************/
/*                                                                                                */
/**************************************************************************************************/

class ri_time_table_t
{
  private:
  std::list<ri_time_table_item> m_lsTimeTbl;
  public:
  bool load_table(const char *szTimeTblPath);
  void clear_table();
};
