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


static const char *pipe_color = "gray";

/*******************************************/

void gen_elbo_ru(FILE *fp, double x, double y, double pipe_width, double elbo_width)
{
  double small_width = elbo_width - pipe_width;
  fprintf(fp, "<path fill=\"url(#%sRadial)\" d=\"M%lf,%lf a%lf,%lf 0 0,0 %lf,%lf h%lf a%lf,%lf 0 0,1 %lf,%lf z\"/>\n",
          pipe_color,
          /* M */ x, y + (pipe_width/2), 
          /* a */  elbo_width, elbo_width, elbo_width, -elbo_width,
          /* h */  -pipe_width,
          /* a */  small_width, small_width, -small_width, small_width);
}

/*******************************************/

void gen_elbo_rd(FILE *fp, double x, double y, double pipe_width, double elbo_width)
{
  double small_width = elbo_width - pipe_width;
  fprintf(fp, "<path fill=\"url(#%sRadial)\" d=\"M%lf,%lf a%lf,%lf 0 0,1 %lf,%lf h%lf a%lf,%lf 0 0,0 %lf,%lf z\"/>\n",
          pipe_color,
          /* M */ x, y - (pipe_width/2), 
          /* a */  elbo_width, elbo_width, elbo_width, elbo_width,
          /* h */  -pipe_width,
          /* a */  small_width, small_width, -small_width, -small_width);
}

/*******************************************/

void gen_elbo_lu(FILE *fp, double x, double y, double pipe_width, double elbo_width)
{
  double small_width = elbo_width - pipe_width;
  fprintf(fp, "<path fill=\"url(#%sRadial)\" d=\"M%lf,%lf a%lf,%lf 0 0,1 %lf,%lf h%lf a%lf,%lf 0 0,0 %lf,%lf z\"/>\n",
          pipe_color,
          /* M */ x, y + (pipe_width/2), 
          /* a */  elbo_width, elbo_width, -elbo_width, -elbo_width,
          /* h */  pipe_width,
          /* a */  small_width, small_width, small_width, small_width);
}

/*******************************************/

void gen_elbo_ld(FILE *fp, double x, double y, double pipe_width, double elbo_width)
{
  double small_width = elbo_width - pipe_width;
  fprintf(fp, "<path fill=\"url(#%sRadial)\" d=\"M%lf,%lf a%lf,%lf 0 0,0 %lf,%lf h%lf a%lf,%lf 0 0,1 %lf,%lf z\"/>\n",
          pipe_color,
          /* M */ x, y - (pipe_width/2), 
          /* a */  elbo_width, elbo_width, -elbo_width, elbo_width,
          /* h */  pipe_width,
          /* a */  small_width, small_width, small_width, -small_width);
}
  
/*******************************************/
static bool last_HR = true;
static bool last_VU = true;
static bool last_V = true;
static bool first = true;

void gen_butt_end(FILE *fp, double width, double butt_width, double x2, double y2)
{
  printf("butt end - width: %lf, bwidth: %lf, x2: %lf, y2: %lf\n",
         width, butt_width, x2, y2);
  if (last_V && last_VU)
  {
    // draw the butt end up
    printf("butt up\n");
    fprintf(fp, 
    "<polygon fill=\"url(#%sLinearV)\" points=\"%lf,%lf %lf,%lf %lf,%lf\"/>\n",
          pipe_color, x2 - (width/2.0), y2, x2 + (width/2.0), y2, x2, y2 - (butt_width/2.0));
  }
  else if (last_V && !last_VU)
  {
    // draw the butt end down 
    printf("butt down\n");
    fprintf(fp, 
    "<polygon fill=\"url(#%sLinearV)\" points=\"%lf,%lf %lf,%lf %lf,%lf\"/>\n",
          pipe_color, x2 - (width/2.0), y2, x2 + (width/2.0), y2, x2, y2 + (butt_width/2.0));
  }
  else if (!last_V && last_HR)
  {
    // draw the butt end right 
    printf("butt right\n");
    fprintf(fp, 
    "<polygon fill=\"url(#%sLinearH)\" points=\"%lf,%lf %lf,%lf %lf,%lf\"/>\n",
          pipe_color, x2, y2 - (width/2.0), x2, y2 + (width/2.0), x2 + (butt_width/2.0), y2);
  }
  else if (!last_V && !last_HR)
  {
    // draw the butt end left 
    printf("butt left\n");
    fprintf(fp, 
    "<polygon fill=\"url(#%sLinearH)\" points=\"%lf,%lf %lf,%lf %lf,%lf\"/>\n",
          pipe_color, x2, y2 - (width/2.0), x2, y2 + (width/2.0), x2 - (butt_width/2.0), y2);
  }
  
}

/*******************************************/

void gen_v_pipe(FILE *fp, double x, double y1, double y2, double width) 
{
  double x1 = x - (width / 2.0);
  double x2 = x + (width / 2.0);

  
  last_V = true;
  if(y1 > y2)
  {
    double tmp;
    tmp = y1;
    y1 = y2;
    y2 = tmp; 
    last_VU = true;
  } 
  else
  {
    last_VU = false;
  }

  printf("V - x1: %lf, y1: %lf, x2: %lf, y2: %lf\n", x1, y1, x2, y2);

  if (first)
  {
    first = false;
    fprintf(fp, 
     "<rect fill=\"url(#%sLinearV)\" x=\"%lf\" y=\"%lf\" width=\"%lf\" height=\"%lf\"/>\n",
          pipe_color, x - (width/2.0), y1, width, y2-y1);
  }
  else if (last_HR && last_VU)
  {
    // case last was to the right, next is up 
    fprintf(fp, 
    "<polygon fill=\"url(#%sLinearV)\" points=\"%lf,%lf %lf,%lf %lf,%lf %lf,%lf %lf,%lf\"/>\n",
          pipe_color, x, y2, x2, y2, x2, y1, x1, y1, x1, y2 - (width * 0.5));
  }
  else if (!last_HR && last_VU)
  {
    // case last was to the left, next is up 
    fprintf(fp, 
    "<polygon fill=\"url(#%sLinearV)\" points=\"%lf,%lf %lf,%lf %lf,%lf %lf,%lf %lf,%lf\"/>\n",
          pipe_color, x, y2, x1, y2, x1, y1, x2, y1, x2, y2 - (width * 0.5));
  }
  else if (last_HR && !last_VU)
  {
    // case last was to the right, next is down
    fprintf(fp, 
    "<polygon fill=\"url(#%sLinearV)\" points=\"%lf,%lf %lf,%lf %lf,%lf %lf,%lf %lf,%lf\"/>\n",
          pipe_color, x, y1, x2, y1, x2, y2, x1, y2, x1, y1 + (width * 0.5));
  }
  else if (!last_HR && !last_VU)
  {
    // case last was to the left, next is down
    fprintf(fp, 
    "<polygon fill=\"url(#%sLinearV)\" points=\"%lf,%lf %lf,%lf %lf,%lf %lf,%lf %lf,%lf\"/>\n",
          pipe_color, x, y1, x1, y1, x1, y2, x2, y2, x2, y1 + (width * 0.5));
  }
}

/*******************************************/

void gen_h_pipe(FILE *fp, double x1, double y, double x2, double width)
{
  double y1 = y - (width / 2.0);
  double y2 = y + (width / 2.0);

  last_V = false;
  if(x1 > x2)
  {
    double tmp;
    tmp = x1;
    x1 = x2;
    x2 = tmp; 
    last_HR = false; 
  } 
  else
  {
    last_HR = true; 
  }
  printf("H - x1: %lf, y1: %lf, x2: %lf, y2: %lf\n", x1, y1, x2, y2);

  if (first)
  {
    first = false;
    fprintf(fp, 
     "<rect fill=\"url(#%sLinearH)\" x=\"%lf\" y=\"%lf\" width=\"%lf\" height=\"%lf\"/>\n",
          pipe_color, x1, y - (width / 2.0), x2-x1, width);
  }
  else if (last_HR && last_VU)
  {
    // case last was up, next is right 
    fprintf(fp, 
   "<polygon fill=\"url(#%sLinearH)\" points=\"%lf,%lf %lf,%lf %lf,%lf %lf,%lf %lf,%lf\"/>\n",
          pipe_color, x1, y1, x2 , y1, x2, y2, x1 + (width * 0.5), y2, x1, y);
  }
  else if (!last_HR && last_VU)
  {
    // case last was up, next is left 
    fprintf(fp, 
   "<polygon fill=\"url(#%sLinearH)\" points=\"%lf,%lf %lf,%lf %lf,%lf %lf,%lf %lf,%lf\"/>\n",
          pipe_color, x2, y1, x1 , y1, x1, y2, x2 - (width * 0.5), y2, x2, y);
  }
  else if (last_HR && !last_VU)
  {
    // case last was down, next is right 
    fprintf(fp, 
    "<polygon fill=\"url(#%sLinearH)\" points=\"%lf,%lf %lf,%lf %lf,%lf %lf,%lf %lf,%lf\"/>\n",
          pipe_color, x1, y2, x2 , y2, x2, y1, x1 + (width * 0.5), y1, x1, y);
  }
  else if (!last_HR && !last_VU)
  {
    // case last was down, next is left 
    fprintf(fp, 
    "<polygon fill=\"url(#%sLinearH)\" points=\"%lf,%lf %lf,%lf %lf,%lf %lf,%lf %lf,%lf\"/>\n",
          pipe_color, x2, y2, x1 , y2, x1, y1, x2 - (width * 0.5), y1, x2, y);
  }
}

/*******************************************/

static int n_instance = 1;

class simple_pipe_t : public gen_plugin_base_t
{
public:
  const char *get_name(void); 
  void generate_doc(doc_object_base_t *dob);
  void generate(plugin_data_t d, int argc, char **argv);
};

/*******************************************/

extern "C" gen_plugin_base_t *get_object(void)
{
  return new simple_pipe_t;
}

/*******************************************/

const char *simple_pipe_t::get_name(void)
{
  return "simple_pipe";
}

/*******************************************/

void simple_pipe_t::generate_doc(doc_object_base_t *dob)
{
  dob->start("simple_pipe", "Simple 3d pipe with shading");
  dob->param("Color of the pipe");
  dob->param("Width of the pipe");
  dob->param("X of the starting point of the pipe");
  dob->param("Y of the starting point of the pipe");
  dob->param("Direction that must be 'V' or 'H'");
  dob->param("Stopping point");
  dob->param("Now, Repeat 5 and 6 as many times as needed to complete the pipe");
  dob->example("simple_pipe|gray|3|90|126|v|112|h|75|v|13.5|h|70|");
  dob->notes("You must alternate V and H, you can not have two Vs or two Hs in a row");
  dob->end();
}

void simple_pipe_t::generate(plugin_data_t d, int argc, char **argv)
{
  const char *the_color = argv[1];
  double width = atof(argv[2]);
  double x1 = atof(argv[3]);
  double y1 = atof(argv[4]);
  double x2;
  double y2;
  double butt_width;

  if (0 == strcasecmp(the_color, "yellow")) 
  {
    pipe_color = "yellow";
    add_svg_library("yellow_gradients.svg");
  }
  else if (0 == strcasecmp(the_color, "black")) 
  {
    pipe_color = "black";
    add_svg_library("black_gradients.svg");
  }
  else if (0 == strcasecmp(the_color, "orange")) 
  {
    pipe_color = "orange";
    add_svg_library("orange_gradients.svg");
  }
  else if (0 == strcasecmp(the_color, "brown")) 
  {
    pipe_color = "brown";
    add_svg_library("brown_gradients.svg");
  }
  else if (0 == strcasecmp(the_color, "white")) 
  {
    pipe_color = "white";
    add_svg_library("white_gradients.svg");
  }
  else if (0 == strcasecmp(the_color, "red")) 
  {
    pipe_color = "red";
    add_svg_library("red_gradients.svg");
  }
  else if (0 == strcasecmp(the_color, "blue")) 
  {
    pipe_color = "blue";
    add_svg_library("blue_gradients.svg");
  }
  else if (0 == strcasecmp(the_color, "green")) 
  {
    pipe_color = "green";
    add_svg_library("green_gradients.svg");
  }
  else // Anything else, use gray
  {
    pipe_color = "gray";
    add_svg_library("gray_gradients.svg");
  }

  printf("Pipe color is: %s\n", pipe_color);
  first = true;

  fprintf(d.svg_fp, "<!-- START insert for simple_pipe (%03d) -->\n", n_instance);
  for (int i=5; i < (argc-1); i+=2)
  {
    switch (argv[i][0])
    {
      case 'h':
      case 'H':
        x2 = atof(argv[i+1]);
        printf("Making Horizontal section to %lf\n", x2);
        y2 = y1;
        if (((i+2) < (argc-1)) && (argv[i+2][0] != 'b') && (argv[i+2][0] != 'B'))
        {
          fprintf(d.svg_fp,  
         "<circle cx=\"%lf\" cy=\"%lf\" r=\"%lf\" fill=\"url(#%sRadial)\"/>\n",
              x2, y2, width / 2.0, pipe_color);
        }
        gen_h_pipe(d.svg_fp, x1, y1,  x2, width);
        break;
      case 'v':
      case 'V':
        y2 = atof(argv[i+1]);
        printf("Making Vertical section to %lf\n", y2);
        x2 = x1;
        if (((i+2) < (argc-1)) && (argv[i+2][0] != 'b') && (argv[i+2][0] != 'B'))
        {
          fprintf(d.svg_fp,  
         "<circle cx=\"%lf\" cy=\"%lf\" r=\"%lf\" fill=\"url(#%sRadial)\"/>\n",
              x2, y2, width / 2.0, pipe_color);
        }
        gen_v_pipe(d.svg_fp, x1, y1, y2, width);
        break;
      case 'b':
      case 'B':
        printf("Making piece to butt against a pipe %lf, %lf\n", x2, y2);
        butt_width = atof(argv[i+1]);
        gen_butt_end(d.svg_fp, width, butt_width, x2, y2);
        break;
      default:
        printf("Error: %s, must be either 'h' or 'v' or 'b' for the direction\n", argv[i]);
        break;
    } 
    x1 = x2;
    y1 = y2;
  }

  fprintf(d.svg_fp, "<!--  END insert for simple_pipe (%03d) -->\n", n_instance);
  n_instance++;
}

/*******************************************/

