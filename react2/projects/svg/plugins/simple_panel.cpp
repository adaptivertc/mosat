
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "gen_display.h"

static int n_instance = 1;

class simple_panel_t : public gen_plugin_base_t
{
public:
  const char *get_name(void); 
  void generate(FILE *svg_fp, FILE *svg_top_of_file_fp, FILE *js_fp, int argc, char **argv);
};

extern "C" gen_plugin_base_t *get_object(void)
{
  return new simple_panel_t;
}

const char *simple_panel_t::get_name(void)
{
  return "simple_panel";
}

void simple_panel_t::generate(FILE *svg_fp, FILE *svg_top_of_file_fp, FILE *js_fp, int argc, char **argv)
{
  const char *tag = argv[1];
  const char *on_color = argv[2];
  const char *off_color = argv[3];
  double cx = atof(argv[4]);
  double cy = atof(argv[5]);
  double r = atof(argv[6]);
  double stroke_width = r * 0.1;
  double font_size = r * 0.6;

  fprintf(svg_fp, "<!--  START insert for simple_panel (%03d) -->\n", n_instance);
  fprintf(js_fp, "// --  START insert for simple_panel (%03d)\n", n_instance);

  fprintf(svg_fp, "<circle id=\"simple_panel_%03d\" cx=\"%lf\" cy=\"%lf\" r=\"%lf\" fill=\"%s\" stroke=\"black\" stroke-width=\"%lf\"/>\n",
            n_instance, cx, cy, r, on_color, stroke_width);
  fprintf(svg_fp, "<text id=\"simple_panel_pv_%03d\" x=\"%lf\" y=\"%lf\" font-family=\"Verdana\" font-size=\"%lf\" fill=\"black\" text-anchor=\"middle\">0</text>\n",
                     n_instance, cx, cy + r + (font_size * 1.1), font_size);


  char js_object_name[30];
  char js_pv_name[30];
  snprintf(js_object_name, sizeof(js_object_name), "simple_panel_obj_%03d", n_instance);
  snprintf(js_pv_name, sizeof(js_object_name), "simple_panel_pv_obj_%03d", n_instance);

  fprintf(js_fp, "var %s = new simple_panel_t(\"simple_panel_%03d\", \"simple_panel_pv_%03d\", \"%s\", \"%s\");\n", js_object_name, n_instance, n_instance, on_color, off_color); 

  fprintf(svg_fp, "<!--  END insert for simple_panel (%03d) -->\n", n_instance);
  fprintf(js_fp, "// --  END insert for simple_panel (%03d)\n", n_instance);

  add_js_library("simple_panel.js");
  add_animation_object(tag, js_object_name);

  n_instance++;
}

