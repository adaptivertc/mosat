
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "gen_display.h"

static int n_instance = 1;

class trasladar_t : public gen_plugin_base_t
{
public:
  const char *get_name(void); 
  void generate(FILE *svg_fp, FILE *svg_top_of_file_fp, FILE *js_fp, int argc, char **argv);
};

extern "C" gen_plugin_base_t *get_object(void)
{
  return new trasladar_t;
}

const char *trasladar_t::get_name(void)
{
  return "trasladar";
}

void trasladar_t::generate(FILE *svg_fp, FILE *svg_top_of_file_fp, FILE *js_fp, int argc, char **argv)
{
//trasladar|LEVEL|mediumslateblue|20|20|280|75
  const char *tag = argv[1];
  const char *color = argv[2];
  double width = atof(argv[3]);
  double x1 = atof(argv[4]);
  double x2 = atof(argv[5]);
  double y = atof(argv[6]);

  
  fprintf(svg_fp, "<!--  START insert for trasladar (%03d) -->\n", n_instance);
  fprintf(js_fp, "// --  START insert for trasladar (%03d)\n", n_instance);
  fprintf(svg_fp, "<rect  id=\"trasladar_%03d\" fill=\"%s\" x=\"%lf\" y=\"%lf\" width=\"%lf\" height=\"%lf\" stroke=\"none\" stroke-width=\"0\" />\n",
                     n_instance, color, x1, y, width, width);

  char js_object_name[30];
  snprintf(js_object_name, sizeof(js_object_name), "trasladar_obj_%03d", n_instance);

  fprintf(js_fp, "var %s = new trasladar_t(\"trasladar_%03d\", %lf, %lf);\n", js_object_name, n_instance, x1, x2); 

  fprintf(svg_fp, "<!--  END insert for trasladar (%03d) -->\n", n_instance);
  fprintf(js_fp, "// --  END insert for trasladar (%03d)\n", n_instance);
  add_js_library("trasladar.js");
  add_animation_object(tag, js_object_name);

  n_instance++;
}
