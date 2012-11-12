/************************************************************************
This software is part of React, a control engine
Copyright (C) 2012 Donald Wayne Carr 

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
#include <stdlib.h>
#include <string.h>

#include "gen_display.h"

static int n_instance = 1;

class heatex_t : public gen_plugin_base_t
{
public:
  const char *get_name(void); 
  void generate_doc(doc_object_base_t *dob);
  void generate(plugin_data_t d, int argc, char **argv);
};

extern "C" gen_plugin_base_t *get_object(void)
{
  return new heatex_t;
}

const char *heatex_t::get_name(void)
{
  return "heatex";
}

void heatex_t::generate_doc(doc_object_base_t *dob)
{
  dob->start("heatex", "A simple schematic for a heat exchanger");
  dob->param("X of upper left corner");
  dob->param("Y of upper left corner");
  dob->param("Width");
  dob->param("Angle of rotation about the center (typically 0, 90, 180, 270)");
  dob->notes("No animation");
  dob->end();
}


void heatex_t::generate(plugin_data_t d, int argc, char **argv)
{
  //const char *tag = argv[1];
  double x1 = atof(argv[2]);
  double y1 = atof(argv[3]);
  double width = atof(argv[4]);
  int angle = atoi(argv[5]);
  double scale_factor = width / 40.0;

  double x2 = x1 + width;
  double y2 = y1 + (width * (5.0/4.0));
  //double height = y2 - y1;

  double cx = (x1 + x2) / 2.0;
  double cy = (y1 + y2) / 2.0;

//tag|x1|y1|width|rotation(0,90,180,270)|

  char str[200];
  if (angle == 0) str[0] = '\0';
  else snprintf(str, sizeof(str), "transform=\"rotate(%d %lf,%lf)\"", angle, cx, cy);

  fprintf(d.svg_fp, "<!--  START insert for heatex (%03d) -->\n", n_instance);

  fprintf(d.svg_fp, "<g id=\"group_object\" fill=\"%s\" stroke=\"none\"  %s>\n", "rgb(40,40,40)", str);

  fprintf(d.svg_fp, "<rect x=\"%lf\" y=\"%lf\" width=\"%lf\" height=\"%lf\"/>\n", 
            x1, y1, 5.0 * scale_factor, 50.0 * scale_factor);
  fprintf(d.svg_fp, "<rect x=\"%lf\" y=\"%lf\" width=\"%lf\" height=\"%lf\"/>\n",
                  x1 + 35.0 * scale_factor, y1, 5.0 * scale_factor, 50.0 * scale_factor);

  fprintf(d.svg_fp, "<rect x=\"%lf\" y=\"%lf\" width=\"%lf\" height=\"%lf\"/>\n",
            x1, y1 + 5.0 * scale_factor, 40.0 * scale_factor, 5.0 * scale_factor);
  fprintf(d.svg_fp, "<rect x=\"%lf\" y=\"%lf\" width=\"%lf\" height=\"%lf\"/>\n",
            x1, y1 + 40.0 * scale_factor, 40.0 * scale_factor, 5.0 * scale_factor);

  for (int i=0; i < 5; i++)
  {
    double the_x = x1 + (scale_factor * (8.75 + (5.0 * i)));
    fprintf(d.svg_fp, "<rect x=\"%lf\" y=\"%lf\" width=\"%lf\" height=\"%lf\"/>\n",
               the_x, y1 + (5.0 * scale_factor), 2.0 * scale_factor, 40 * scale_factor);
  }

  fprintf(d.svg_fp, "</g>\n");

  fprintf(d.svg_fp, "<!--  END insert for heatex (%03d) -->\n", n_instance);
  n_instance++;
}

