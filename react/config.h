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

#ifndef __CONFIG_INC__
#define __CONFIG_INC__

class react_config_t
{
private:
  int n_keys;
  int cur_key;
  char *key[100];
  char *val[100];

public:
  const char *get_config(const char *);
  int get_int(const char *);
  double get_double(const char *);
  void read_file(char * path);
  const char *first(void);
  const char *next(void);
  react_config_t();
};

extern react_config_t *react_config;

#endif
