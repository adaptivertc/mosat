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

// Add other classes / structures as needed
#include <list>

struct ri_train_data_t 
{
	unsigned sec_num;					// sen_num = Número secuencial del tren
	time_t	entry_time;					// tiempo de entrada en servicio
	unsigned cur_seg;					// segmento donde está el tren actualmente
};

class ri_evalg_t : public event_alg_t
{
private:
	FILE * fp;
  	unsigned m_uiNumOfSections;
	unsigned m_uiSecNum;
	train_data_t trainarray[50];
	std::list<ri_train_data_t> train_data_array;
// Add private definitions here!
public:
	ri_evalg_t();
	void init(void);
	void update(time_t time);
	void process_event(crossing_event_t ev);
	void gen_htmlreport();
};



