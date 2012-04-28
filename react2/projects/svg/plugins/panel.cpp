
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "gen_display.h"

/*******************************************/

static int n_instance = 1;

class panel_t : public gen_plugin_base_t
{
public:
  const char *get_name(void); 
  void generate(FILE *svg_fp, FILE *svg_top_of_file_fp, FILE *js_fp, int argc, char **argv);
};

/*******************************************/

extern "C" gen_plugin_base_t *get_object(void)
{
  return new panel_t;
}

/*******************************************/

const char *panel_t::get_name(void)
{
  return "panel";
}

/*******************************************/

void panel_t::generate(FILE *svg_fp, FILE *svg_top_of_file_fp, FILE *js_fp, int argc, char **argv)
{
  double x1 = atof(argv[1]);
  double y1 = atof(argv[2]);
  double width = atof(argv[3]);
  double height = atof(argv[4]);
  double shadow_width = atof(argv[5]);
  double x2 = x1 + width;
  double y2 = y1 + height;
  double sw = shadow_width;

  int pcolor = 200; 
  int lcolor = 240; 
  int scolor = 80;

  char str_pcolor[100];  
  char str_lcolor[100];
  char str_scolor[100];
  snprintf(str_pcolor, sizeof(str_pcolor), "rgb(%d,%d,%d)", 
          pcolor, pcolor, pcolor);
  snprintf(str_lcolor, sizeof(str_lcolor), "rgb(%d,%d,%d)", 
          lcolor, lcolor, lcolor);
  snprintf(str_scolor, sizeof(str_scolor), "rgb(%d,%d,%d)", 
          scolor, scolor, scolor);

  fprintf(svg_fp, "<!-- START insert for panel (%03d) -->\n", n_instance);

  fprintf(svg_fp, "<rect fill=\"%s\" x=\"%lf\" y=\"%lf\" width=\"%lf\" height=\"%lf\" stroke=\"none\"/>\n",
        str_pcolor, x1, y1, width, height);

  fprintf(svg_fp, "<polygon points=\"%lf,%lf %lf,%lf %lf,%lf %lf,%lf\" fill=\"%s\" stroke=\"none\"/>\n",
     x1, y1, x1+sw, y1+sw, x2-sw, y1+sw, x2, y1, str_lcolor);
  fprintf(svg_fp, "<polygon points=\"%lf,%lf %lf,%lf %lf,%lf %lf,%lf\" fill=\"%s\" stroke=\"none\"/>\n", 
     x1, y1, x1+sw, y1+sw, x1+sw, y2-sw, x1, y2, str_lcolor);
  fprintf(svg_fp, "<polygon points=\"%lf,%lf %lf,%lf %lf,%lf %lf,%lf\" fill=\"%s\" stroke=\"none\"/>\n",
     x1, y2, x1+sw, y2-sw, x2-sw, y2-sw, x2, y2, str_scolor);
  fprintf(svg_fp, "<polygon points=\"%lf,%lf %lf,%lf %lf,%lf %lf,%lf\" fill=\"%s\" stroke=\"none\"/>\n",
     x2, y2, x2-sw, y2-sw, x2-sw, y1+sw, x2, y1, str_scolor);

 

  fprintf(svg_fp, "<!--  END insert for panel (%03d) -->\n", n_instance);
  n_instance++;
}

/*******************************************/

