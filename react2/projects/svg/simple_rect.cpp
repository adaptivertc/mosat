
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "gen_display.h"

static int n_instance = 1;

class simple_rect_t : public gen_object_base_t
{
public:
  const char *get_name(void); 
  void generate(FILE *svg_fp, FILE *js_fp, int argc, char **argv);
};

extern "C" gen_object_base_t *get_object(void)
{
  return new simple_rect_t;
}

const char *simple_rect_t::get_name(void)
{
  return "simple_rect";
}

void simple_rect_t::generate(FILE *svg_fp, FILE *js_fp, int argc, char **argv)
{
  const char *tag = argv[1];
  const char *color = argv[2];
  double x = atof(argv[3]);
  double y = atof(argv[4]);
  double height = atof(argv[5]);
  double width = height * 0.2;
  double font_size = height * 0.1;
  double cx = x + (width/2.0);
  double cy = y + (height/2.0);

  fprintf(svg_fp, "<rect  id=\"simple_rect_%03d\" fill=\"%s\" x=\"%lf\" y=\"%lf\" width=\"%lf\" height=\"%lf\" stroke=\"none\" stroke-width=\"0\" transform=\"rotate(180 %lf %lf)\"/>\n",
                     n_instance, color, x, y, width, height, cx, cy);
  fprintf(svg_fp, "<rect  fill=\"none\" x=\"%lf\" y=\"%lf\" width=\"%lf\" height=\"%lf\" stroke=\"black\" stroke-width=\"1\"/>\n",
                     x, y, width, height);
  fprintf(svg_fp, "<text id=\"simple_rect_pv_%03d\" x=\"%lf\" y=\"%lf\" font-family=\"Verdana\" font-size=\"%lf\" fill=\"black\" text-anchor=\"middle\">0</text>\n",
                     n_instance, cx, y + height + (font_size * 1.1), font_size); 

  char js_object_name[30];
  snprintf(js_object_name, sizeof(js_object_name), "simple_rect_obj_%03d", n_instance);

  fprintf(js_fp, "var %s = new simple_rect_t(\"simple_rect_%03d\", \"simple_rect_pv_%03d\", %lf);\n", js_object_name, n_instance, n_instance, height); 

  add_library("simple_rect.js");
  add_update_object(tag, js_object_name);

  n_instance++;
}

