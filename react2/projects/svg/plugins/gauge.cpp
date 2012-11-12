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

class gauge_t : public gen_plugin_base_t
{
public:
  const char *get_name(void); 
  void generate_doc(doc_object_base_t *dob);
  void generate(plugin_data_t d, int argc, char **argv);
};

extern "C" gen_plugin_base_t *get_object(void)
{
  return new gauge_t;
}

const char *gauge_t::get_name(void)
{
  return "gauge";
}

void gauge_t::generate_doc(doc_object_base_t *dob)
{
  dob->start("gauge", "A circular gage with needle, automatic scale, pv text, engineering units\n");
  dob->param("Type (1 = with simulated black plastic cover and screws, 2 = round gauge only"); 
  dob->param("Color of needle, pv text, and engieering units"); 
  dob->param("X of upper left corner"); 
  dob->param("Y of upper left corner"); 
  dob->param("Width"); 
  dob->notes("Height = Width");
  dob->end();
}

void gauge_t::generate(plugin_data_t d, int argc, char **argv)
{
  int gtype = atol(argv[1]);
  const char *tag = argv[2];
  const char *color = argv[3];
  double x = atof(argv[4]);
  double y = atof(argv[5]);
  double width = atof(argv[6]);
  //double width = height * 0.2;
  //double font_size = height * 0.1;
  //double cx = x + (width/2.0);
  //double cy = y + (height/2.0);
  //double stroke_width = height / 100.0;

  double cx = x + width / 2.0;
  double cy = y + width / 2.0;
  
  fprintf(d.svg_fp, "<!--  START insert for gauge (%03d) -->\n", n_instance);
  fprintf(d.js_fp, "// --  START insert for gauge (%03d)\n", n_instance);

  char js_object_name[30];
  snprintf(js_object_name, sizeof(js_object_name), "gauge_obj_%03d", n_instance);

  fprintf(d.js_fp, "var %s = new gauge_t(\"gauge_needle_%03d\", \"gauge_pv_%03d\", \"gauge_eu_%03d\", %lf, %lf, %lf);\n", 
           js_object_name, n_instance, n_instance, n_instance, x, y, width); 

  if (gtype == 1)
  {
    fprintf(d.svg_fp, "<rect x=\"%lf\" y=\"%lf\" rx=\"%lf\" ry=\"%lf\" width=\"%lf\" height=\"%lf\"\n",
               x, y, width * (1.0/15.0), width * (1.0/15.0), width, width);
    fprintf(d.svg_fp, "style=\"fill:rgb(54,60,60);stroke:black;stroke-width:%lf;opacity:1.0\"/>\n", width *(2.0/150));
  }

  fprintf(d.svg_fp, "<circle cx=\"%lf\" cy=\"%lf\" r=\"%lf\" fill=\"white\"/>\n", 
             cx, cy, width * (70.0/150.0));
  fprintf(d.svg_fp, "  <circle cx=\"%lf\" cy=\"%lf\" r=\"%lf\" fill=\"cornsilk\"\n", 
                          cx, cy, width * (65.0/150.0));
  fprintf(d.svg_fp, "         style=\"stroke:rgb(30,30,30);stroke-width:%lf\"/>\n", width / 150.0);


  if (gtype != 1)
  {
    fprintf(d.svg_fp, 
     "<circle cx=\"%lf\" cy=\"%lf\" r=\"%lf\" fill=\"none\""
     " stroke-width=\"%lf\" stroke = \"black\"/>\n", 
     //" stroke-width=\"%lf\" stroke = \"black\" filter=\"url(#filter1)\"/>\n", 
       cx, cy, width * (72.0/150.0), width * 4/150);
  }
  else
  {
    double screw_cx = width * 0.1;
    double screw_cy = width * 0.1; 
    double screw_x1 = width * (13.0/150.0);
    double screw_y1 = width * 0.1;
    double screw_x2 = width * (17.0/150.0);
    double screw_y2 = width * 0.1;

    double screw_r = width * (3.0/150.0);
    double screw_stroke_width = width * (1.2/150.0);

    fprintf(d.svg_fp, "  <circle cx=\"%lf\" cy=\"%lf\" r=\"%lf\" stroke=\"none\" stroke-width=\"0\" fill=\"black\"/>\n",
                  x + screw_cx, y + screw_cy, screw_r);
    fprintf(d.svg_fp, "  <line x1=\"%lf\" y1=\"%lf\" x2=\"%lf\" y2=\"%lf\" transform=\"rotate(20 %lf %lf)\"\n", 
                      x + screw_x1, y + screw_y1, 
                      x + screw_x2, y + screw_y2, x + 
                      screw_cx, y + screw_cy);
    fprintf(d.svg_fp, "      style=\"stroke:rgb(64,65,65);stroke-width:%lf\"/>\n", screw_stroke_width);
    fprintf(d.svg_fp, "  <line x1=\"%lf\" y1=\"%lf\" x2=\"%lf\" y2=\"%lf\" transform=\"rotate(110 %lf %lf)\"\n",
                      x + screw_x1, y + screw_y1, x + screw_x2, y + screw_y2, x + screw_cx, y + screw_cy);
    fprintf(d.svg_fp, "      style=\"stroke:rgb(64,65,65);stroke-width:%lf\"/>\n", screw_stroke_width);

    screw_cx = width - screw_cx;
    screw_x1 = width - screw_x1;
    screw_x2 = width - screw_x2;

    fprintf(d.svg_fp, "  <circle cx=\"%lf\" cy=\"%lf\" r=\"%lf\" stroke=\"none\" stroke-width=\"0\" fill=\"black\"/>\n",
                  x + screw_cx, y + screw_cy, screw_r);
    fprintf(d.svg_fp, "  <line x1=\"%lf\" y1=\"%lf\" x2=\"%lf\" y2=\"%lf\" transform=\"rotate(-10 %lf %lf)\"\n", 
                      x + screw_x1, y + screw_y1, x + screw_x2, y + screw_y2, x + screw_cx, y + screw_cy);
    fprintf(d.svg_fp, "      style=\"stroke:rgb(64,65,65);stroke-width:%lf\"/>\n", screw_stroke_width);
    fprintf(d.svg_fp, "  <line x1=\"%lf\" y1=\"%lf\" x2=\"%lf\" y2=\"%lf\" transform=\"rotate(80 %lf %lf)\"\n",
                      x + screw_x1, y + screw_y1, x + screw_x2, y + screw_y2, x + screw_cx, y + screw_cy);
    fprintf(d.svg_fp, "      style=\"stroke:rgb(64,65,65);stroke-width:%lf\"/>\n", screw_stroke_width);

    screw_cy = width - screw_cy;
    screw_y1 = width - screw_y1;
    screw_y2 = width - screw_y2;

    fprintf(d.svg_fp, "  <circle cx=\"%lf\" cy=\"%lf\" r=\"%lf\" stroke=\"none\" stroke-width=\"0\" fill=\"black\"/>\n",
                  x + screw_cx, y + screw_cy, screw_r);
    fprintf(d.svg_fp, "  <line x1=\"%lf\" y1=\"%lf\" x2=\"%lf\" y2=\"%lf\" transform=\"rotate(17 %lf %lf)\"\n", 
                      x + screw_x1, y + screw_y1, x + screw_x2, y + screw_y2, x + screw_cx, y + screw_cy);
    fprintf(d.svg_fp, "      style=\"stroke:rgb(64,65,65);stroke-width:%lf\"/>\n", screw_stroke_width);
    fprintf(d.svg_fp, "  <line x1=\"%lf\" y1=\"%lf\" x2=\"%lf\" y2=\"%lf\" transform=\"rotate(107 %lf %lf)\"\n",
                      x + screw_x1, y + screw_y1, x + screw_x2, y + screw_y2, x + screw_cx, y + screw_cy);
    fprintf(d.svg_fp, "      style=\"stroke:rgb(64,65,65);stroke-width:%lf\"/>\n", screw_stroke_width);

    screw_cx = width - screw_cx;
    screw_x1 = width - screw_x1;
    screw_x2 = width - screw_x2;

    fprintf(d.svg_fp, "  <circle cx=\"%lf\" cy=\"%lf\" r=\"%lf\" stroke=\"none\" stroke-width=\"0\" fill=\"black\"/>\n",
                  x + screw_cx, y + screw_cy, screw_r);
    fprintf(d.svg_fp, "  <line x1=\"%lf\" y1=\"%lf\" x2=\"%lf\" y2=\"%lf\" transform=\"rotate(3 %lf %lf)\"\n", 
                      x + screw_x1, y + screw_y1, x + screw_x2, y + screw_y2, x + screw_cx, y + screw_cy);
    fprintf(d.svg_fp, "      style=\"stroke:rgb(64,65,65);stroke-width:%lf\"/>\n", screw_stroke_width);
    fprintf(d.svg_fp, "  <line x1=\"%lf\" y1=\"%lf\" x2=\"%lf\" y2=\"%lf\" transform=\"rotate(93 %lf %lf)\"\n",
                      x + screw_x1, y + screw_y1, x + screw_x2, y + screw_y2, x + screw_cx, y + screw_cy);
    fprintf(d.svg_fp, "      style=\"stroke:rgb(64,65,65);stroke-width:%lf\"/>\n", screw_stroke_width);
  }

  char needle_d[200];
  double needle_r = width * (6.0/150.0);
  snprintf(needle_d, sizeof(needle_d), "d=\"M%lf,%lf A%lf,%lf 0 1,1 %lf,%lf L%lf,%lf Z\"", 
                          cx - needle_r, cy, needle_r, needle_r, cx + needle_r, cy, 
                                       cx, y + (width * (140.0/150.0))),
  fprintf(d.svg_fp, "<!-- d: '%s' -->\n", needle_d); 

  /**
  fprintf(d.svg_fp, "  <path fill=\"blue\" id=\"gauge_sneedle1_%03d\" \n", n_instance);
  fprintf(d.svg_fp, "      %s style=\"fill-opacity:0.3\"\n", needle_d);
  fprintf(d.svg_fp, "         onmouseover=\"this.title='Testing, what will happen?';\"/>\n");

  fprintf(d.svg_fp, "  <path fill=\"blue\" id=\"gauge_sneedle2_%03d\" \n", n_instance);
  fprintf(d.svg_fp, "      %s style=\"fill-opacity:0.3\"\n", needle_d);
  fprintf(d.svg_fp, "         onmouseover=\"this.style.cursor='crosshair';\"/>\n");
  ***/


  fprintf(d.svg_fp, "  <path fill=\"%s\" id=\"gauge_needle_%03d\" \n", color, n_instance);
  fprintf(d.svg_fp, "      %s />\n", needle_d);


  fprintf(d.svg_fp, "  <circle cx=\"%lf\" cy=\"%lf\" r=\"%lf\" stroke=\"none\" stroke-width=\"0\" fill=\"black\"/>\n",
            cx, cy, width * (4.0/150.0));

  fprintf(d.svg_fp, "  <text id=\"gauge_pv_%03d\" x=\"%lf\" y=\"%lf\" font-size=\"%lf\" style=\"fill:%s;\" \n",
                            n_instance, cx, cy + (width * (25.0/150.0)), width * (20.0/150.0), color);
  fprintf(d.svg_fp, "       alignment-baseline=\"middle\" text-anchor=\"middle\">300</text>\n");
  fprintf(d.svg_fp, "\n");

/**
  fprintf(d.svg_fp, "  <text id=\"gauge_range_%03d\" x=\"%lf\" y=\"%lf\" font-size=\"%lf\" style=\"fill:blue;\" \n",
                            n_instance, cx, cy + (width * (45.0/150.0)), width * (15.0/150.0));
  fprintf(d.svg_fp, "       alignment-baseline=\"middle\" text-anchor=\"middle\">300</text>\n");
***/

  fprintf(d.svg_fp, "  <text id=\"gauge_eu_%03d\" x=\"%lf\" y=\"%lf\" font-size=\"%lf\" \n",
                            n_instance, cx, cy + (width * (45.0/150.0)), width * (15.0/150.0));
  fprintf(d.svg_fp, "       alignment-baseline=\"middle\" text-anchor=\"middle\">cm</text>\n");

  fprintf(d.svg_fp, "<!--  END insert for gauge (%03d) -->\n", n_instance);
  fprintf(d.js_fp, "// --  END insert for gauge (%03d)\n", n_instance);
  //add_svg_library("filter1.svg");
  add_js_library("scales.js");
  add_js_library("gauge.js");
  add_animation_object(tag, js_object_name);

  n_instance++;
}

