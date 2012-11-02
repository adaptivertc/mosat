
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "gen_display.h"

/*******************************************/

static int n_instance = 1;

class spray_t : public gen_plugin_base_t
{
public:
  const char *get_name(void); 
  void generate(plugin_data_t d, int argc, char **argv);
};

/*******************************************/

extern "C" gen_plugin_base_t *get_object(void)
{
  return new spray_t;
}

/*******************************************/

const char *spray_t::get_name(void)
{
  return "spray";
}

/*******************************************/

void spray_t::generate(plugin_data_t d, int argc, char **argv)
{
  int i, nlines;
  double topx1, bottomx1, top_increment, bottom_increment;
  double x1, y1, x2, y2;
  double x, y, line_width, top_spray_width, bottom_spray_width, spray_height;

  fprintf(d.svg_fp, "<!-- START insert for spray (%03d) -->\n", n_instance);
  fprintf(d.svg_fp, "<!-- Original generator version written by Ricardo Tucker Carr -->\n");

  const char *the_tag = argv[1];
  x = atof(argv[2]);
  y = atof(argv[3]);
  nlines = atoi(argv[4]);
  line_width = atof(argv[5]);
  top_spray_width = atof(argv[6]);
  bottom_spray_width = atof(argv[7]);
  spray_height = atof(argv[8]);

  fprintf(stderr, "x = %lf\n", x);
  fprintf(stderr, "y = %lf\n", y);
  fprintf(stderr, "nlines = %d\n", nlines);
  fprintf(stderr, "line_width = %lf\n", line_width);
  fprintf(stderr, "top_spray_width = %lf\n", top_spray_width);
  fprintf(stderr, "bottom_spray_width = %lf\n", bottom_spray_width);
  fprintf(stderr, "spray_height = %lf\n", spray_height);
  
  topx1 = x-(top_spray_width/2.0);
  bottomx1 = x-(bottom_spray_width/2.0);
  top_increment = top_spray_width/(nlines-1.0);
  bottom_increment = bottom_spray_width/(nlines-1.0);
  y1 = y;
  y2 = y+spray_height;

  char js_object_name[60];
  char js_group_name[60];
  snprintf(js_object_name, sizeof(js_object_name), "spray_obj_%03d", n_instance);
  snprintf(js_group_name, sizeof(js_group_name), "spray_group_obj_%03d", n_instance);

  fprintf(d.svg_fp, "<g id=\"%s\" fill=\"none\" stroke=\"white\" stroke-width=\"%lf\" stroke-dasharray=\"%lf,%lf\">\n",
           js_group_name, line_width, line_width, 2.0 * line_width);
  for (i = 0; i < nlines; i++)
  {
    x1 = topx1+(i*top_increment);
    x2= bottomx1+(i*bottom_increment);
    //printf(" %lf, %lf,  %lf, %lf\n", x1, y1, x2, y2);
    fprintf(d.svg_fp, "  <line x1=\"%lf\" y1=\"%lf\" x2=\"%lf\" y2=\"%lf\"/>\n", x1, y1, x2, y2);
  }
  fprintf(d.svg_fp, "</g>\n");
  fprintf(d.js_fp, "var %s = new spray_t(\"%s\");\n",
             js_object_name, js_group_name);
  fprintf(d.svg_fp, "<!--  END insert for spray (%03d) -->\n", n_instance);
  if ((strlen(the_tag) > 0) && (0 != strcmp(the_tag, "null")))
  {
    add_js_library("spray.js");
    add_animation_object(the_tag, js_object_name);
  }

  n_instance++;
}

/*******************************************/

