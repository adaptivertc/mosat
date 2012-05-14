
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "gen_display.h"

static int n_instance = 1;

class pendulo_t : public gen_plugin_base_t
{
public:
  const char *get_name(void); 
  void generate(plugin_data_t d, int argc, char **argv);
};

extern "C" gen_plugin_base_t *get_object(void)
{
  return new pendulo_t;
}

const char *pendulo_t::get_name(void)
{
  return "pendulo";
}

void pendulo_t::generate(plugin_data_t d, int argc, char **argv)
{
  const char *tag = argv[1];
  const char *color = argv[2];
  double x = atof(argv[3]);
  double y = atof(argv[4]);
  double height = atof(argv[5]);
  double r = height / 10.0;
  char obj_name[100];
  double font_size = height * 0.2;
  
  fprintf(d.svg_fp, "<!--  START insert for pendulo (%03d) -->\n", n_instance);

  //<path fill="green" id="red_needle"
   //   d="M69,75 A6,6 0 1,1 81,75 L75,140 Z" />

  fprintf(d.js_fp, "// --  START insert for pendulo (%03d)\n", n_instance);
  fprintf(d.svg_fp, "<path fill=\"%s\" id=\"pendulo_%03d\"\n",
           color, n_instance);
         
  fprintf(d.svg_fp, "    d=\"M%lf,%lf A%lf,%lf 0 1,1 %lf,%lf L%lf,%lf Z\" />\n",
           x - r, y, r, r,  x + r, y, x, y + height);

  fprintf(d.svg_fp, "<text id=\"pendulo_pv_%03d\" x=\"%lf\" y=\"%lf\" font-family=\"Verdana\" font-size=\"%lf\" fill=\"black\" text-anchor=\"middle\">0</text>\n",
                     n_instance, x, y - r - (font_size * 1.1), font_size);


  snprintf(obj_name, sizeof(obj_name), "pendulo_obj_%03d", n_instance);

  fprintf(d.js_fp, "var %s = new pendulo_t(\"pendulo_%03d\", \"pendulo_pv_%03d\", %lf, %lf);\n", 
               obj_name, n_instance, n_instance, x, y);

  fprintf(d.svg_fp, "<!--  END insert for pendulo (%03d) -->\n", n_instance);
   fprintf(d.js_fp, "// --  END insert for pendulo (%03d)\n", n_instance);


  add_js_library("pendulo.js");
  add_animation_object(tag, obj_name);

  n_instance++;
}

