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

class text_t : public gen_plugin_base_t
{
public:
  const char *get_name(void); 
  void generate_doc(doc_object_base_t *dob);
  void generate(plugin_data_t d, int argc, char **argv);
};

extern "C" gen_plugin_base_t *get_object(void)
{
  return new text_t;
}

const char *text_t::get_name(void)
{
  return "text";
}

void text_t::generate_doc(doc_object_base_t *dob)
{
  dob->start("text", "Multi-line text widget - no animation");
  dob->param("X position of achor point");
  dob->param("Y position of first base-line");
  dob->param("Text Height");
  dob->param("Text Color");
  dob->param("Text Anchor (start, middle, end)");
  dob->notes("Preferred text object");
  dob->end();
}

void text_t::generate(plugin_data_t d, int argc, char **argv)
{
  double x = atof(argv[1]);
  double y = atof(argv[2]);
  double height = atof(argv[3]);
  const char *color = argv[4];
  const char *anchor = argv[5];

//x|y|height|color|anchor|line1|line2|. . . |lineN
  
  fprintf(d.svg_fp, "<!--  START insert for text (%03d) -->\n", n_instance);
  for (int i=6; i < argc; i++)
  {
    const char *the_text = argv[i];
    fprintf(d.svg_fp, "<text x=\"%lf\" y=\"%lf\" font-family=\"Verdana\" font-size=\"%lf\" fill=\"%s\" text-anchor=\"%s\">%s</text>\n",
                     x, y, height, color, anchor, the_text); 
    y += height * 1.05; 
  }
  fprintf(d.svg_fp, "<!--  END insert for text (%03d) -->\n", n_instance);
  n_instance++;
}

