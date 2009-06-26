/************************************************************************
This software is part of React, a control engine
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

#define REACT_MOD_DI (1)
#define REACT_MOD_AI (2)
#define REACT_MAX_MOD_IO (5)

struct mod_io_def_t
{
  int type;
  int opcode;
  int n;
  int modbus_offset;
  int channel_offset;   
};


struct do_send_t
{
  int ch;
  bool val;
};

struct ao_send_t
{
  int ch;
  unsigned short val;
};


class reactmodbus_driver_t : public io_driver_t
{
private:
  bool read_values;
  bool wake_him_up;
  int x[20];
  rtmodbus_t *modbus;
  unsigned short ai_vals[64];
  bool di_vals[64];
  unsigned short tmp_ai_vals[64];
  bool tmp_di_vals[64];
  do_send_t do_vals_to_send[64];  
  ao_send_t ao_vals_to_send[64];  
  int n_dos_to_send;
  int n_aos_to_send;
  int di_offset;
  int do_offset;
  int ai_offset;
  int ao_offset;
  sem_t output_mutex_sem; 
  sem_t read_mutex_sem; 
  sem_t read_wait_sem; 
  int n_mod_io; 
  mod_io_def_t mod_io[REACT_MAX_MOD_IO];
  void add_io(const char *io_type, int opcode, int n_io, 
               int modbus_offset, int channel_offset);
  void read_mod_io(void);
public:
  reactmodbus_driver_t(react_drv_base_t *react, const char *option);
  void read(void);
  void end_read(void);
  void read_thread(void);
  bool get_di(int channel);
  double get_ai(int channel);
  long get_count(int channel);
  void send_do(int channel, bool val);
  void send_ao(int channel, double val);
  void close(void);
};


