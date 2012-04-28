
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "gen_display.h"

static int n_instance = 1;

class blades_t : public gen_plugin_base_t
{
public:
  const char *get_name(void); 
  void generate(FILE *svg_fp, FILE *svg_top_of_file_fp, FILE *js_fp, int argc, char **argv);
};

extern "C" gen_plugin_base_t *get_object(void)
{
  return new blades_t;
}

const char *blades_t::get_name(void)
{
  return "blades";
}

void blades_t::generate(FILE *svg_fp, FILE *svg_top_of_file_fp, FILE *js_fp, int argc, char **argv)
{
  const char *tag = argv[1];
  double x1 = atof(argv[2]);
  double y1 = atof(argv[3]);
  double height = atof(argv[4]);
  int angle = atoi(argv[5]);
  double scale_factor = height / 50.0;

  double cx = x1;
  double cy = y1;

//tag|x1|y1|height|rotation(0,90,180,270)|

  char str[200];
  if (angle == 0) str[0] = '\0';
  else snprintf(str, sizeof(str), "transform=\"rotate(%d %lf,%lf)\"", angle, cx, cy);

  fprintf(svg_fp, "<!--  START insert for blades (%03d) -->\n", n_instance);

  fprintf(svg_fp, "<g id=\"group_object\" fill=\"%s\" stroke=\"black\" stroke-width=\"%lf\" %s>\n", 
                  "lime",  0.3 * scale_factor, str);

  fprintf(svg_fp, "<circle cx=\"%lf\" cy=\"%lf\" r=\"%lf\" stroke=\"black\" stroke-width=\"%lf\" fill=\"black\"/>\n",
                    cx, cy, 1.6 * scale_factor, 0.3 * scale_factor);
  fprintf(svg_fp, "<polygon points=\"%lf,%lf %lf,%lf %lf,%lf\" />\n",
                    cx, cy, x1 + (5.0 * scale_factor), y1 - (20.0 * scale_factor), x1, y1 - (25 * scale_factor) );
  fprintf(svg_fp, "<polygon points=\"%lf,%lf %lf,%lf %lf,%lf\" />\n",
                    cx, cy, x1 - (5.0 * scale_factor), y1 + (20.0 * scale_factor), x1, y1 + (25.0 * scale_factor));
  fprintf(svg_fp, "<polygon points=\"%lf,%lf %lf,%lf %lf,%lf\" />\n",
                    cx, cy, x1 - (4.0 * scale_factor), y1 - (5.0 * scale_factor), x1 - (7.0 * scale_factor), y1 - (1.0 * scale_factor)); 
  fprintf(svg_fp, "<polygon points=\"%lf,%lf %lf,%lf %lf,%lf\"/>\n",
                    cx, cy, x1 + (6.0 * scale_factor), y1 + (4.0 * scale_factor), x1 + (7.0 * scale_factor), y1 ); 
  fprintf(svg_fp, "<line x1=\"%lf\" y1=\"%lf\" x2=\"%lf\" y2=\"%lf\" stroke=\"black\" stroke-width=\"%lf\"/>\n",
                    cx, cy, x1 + (20.0 * scale_factor), y1, 2.0 * scale_factor); 


  fprintf(svg_fp, "</g>\n");

  fprintf(svg_fp, "<!--  END insert for blades (%03d) -->\n", n_instance);
  n_instance++;
}

