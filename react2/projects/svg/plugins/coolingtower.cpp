
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "gen_display.h"

static int n_instance = 1;

class cooling_tower_t : public gen_plugin_base_t
{
public:
  const char *get_name(void); 
  void generate(FILE *svg_fp, FILE *svg_top_of_file_fp, FILE *js_fp, int argc, char **argv);
};

extern "C" gen_plugin_base_t *get_object(void)
{
  return new cooling_tower_t;
}

const char *cooling_tower_t::get_name(void)
{
  return "cooling_tower";
}

void cooling_tower_t::generate(FILE *svg_fp, FILE *svg_top_of_file_fp, FILE *js_fp, int argc, char **argv)
{
  double x1 = atof(argv[1]);
  double y1 = atof(argv[2]);
  double width = atof(argv[3]);
  const char *color = argv[4];
  const char *the_text = argv[5];
  const char *text_color = argv[6];
  double scale_factor = width / 50.0;
  double y2 = y1 + 40 * scale_factor;
  
  fprintf(svg_fp, "<!--  START insert for cooling_tower (%03d) -->\n", n_instance);

  fprintf(svg_fp, "<polygon points=\"%lf,%lf %lf,%lf %lf,%lf %lf,%lf\" stroke=\"black\" stroke-width=\"%lf\" fill=\"%s\"/>\n",
             x1, y1, x1 + width, y1, x1 + (40 * scale_factor), y2, x1 + (10.0 * scale_factor), y2, 0.5 * scale_factor, color);
  fprintf(svg_fp, "<text x=\"%lf\" y=\"%lf\" font-size=\"%lf\" fill=\"%s\" text-anchor=\"middle\">%s</text>\n",
           x1 + (width / 2.0), y1 + (8.0 * scale_factor), 5.0 * scale_factor, text_color, the_text);
  fprintf(svg_fp, "<!--  END insert for cooling_tower (%03d) -->\n", n_instance);
  n_instance++;
}

