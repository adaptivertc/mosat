
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "gen_display.h"

static int n_instance = 1;

class analog_pv_t : public gen_plugin_base_t
{
public:
  const char *get_name(void); 
  void generate(plugin_data_t d, int argc, char **argv);
};

extern "C" gen_plugin_base_t *get_object(void)
{
  return new analog_pv_t;
}

const char *analog_pv_t::get_name(void)
{
  return "analog_pv";
}

void analog_pv_t::generate(plugin_data_t d, int argc, char **argv)
{
  const char *tag = argv[1];
  const char *color = argv[2];
  double x = atof(argv[3]);
  double y = atof(argv[4]);
  double height = atof(argv[5]);
  double font_size = height;

  
  fprintf(d.svg_fp, "<!--  START insert for analog_pv (%03d) -->\n", n_instance);
  fprintf(d.js_fp, "// --  START insert for analog_pv (%03d)\n", n_instance);
  fprintf(d.svg_fp, "<text id=\"analog_pv_%03d\" x=\"%lf\" y=\"%lf\" font-family=\"Verdana\" font-size=\"%lf\" fill=\"%s\" text-anchor=\"middle\">0</text>\n",
             n_instance, x, y, font_size, color); 

  char js_object_name[30];
  snprintf(js_object_name, sizeof(js_object_name), "analog_pv_obj_%03d", n_instance);

  fprintf(d.js_fp, "var %s = new analog_pv_t(\"analog_pv_%03d\");\n", js_object_name, n_instance); 
  fprintf(d.svg_fp, "<!--  END insert for analog_pv (%03d) -->\n", n_instance);
  fprintf(d.js_fp, "// --  END insert for analog_pv (%03d)\n", n_instance);
  add_js_library("analog_pv.js");
  add_animation_object(tag, js_object_name);
  n_instance++;
}

