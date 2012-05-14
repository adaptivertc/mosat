
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "gen_display.h"

static int n_instance = 1;

class rgauge_t : public gen_plugin_base_t
{
public:
  const char *get_name(void); 
  void generate(plugin_data_t d, int argc, char **argv);
};

extern "C" gen_plugin_base_t *get_object(void)
{
  return new rgauge_t;
}

const char *rgauge_t::get_name(void)
{
  return "rgauge";
}

void rgauge_t::generate(plugin_data_t d, int argc, char **argv)
{
  int gtype = atol(argv[1]);
  const char *tag = argv[2];
  const char *rcolor = argv[3];
  double x = atof(argv[4]);
  double y = atof(argv[5]);
  double width = atof(argv[6]);

  double cx = x + width / 2.0;
  double cy = y + width / 2.0;
  double r = width / 2.0;
  double scale_factor = r / 63.5;
  
  fprintf(d.svg_fp, "<!--  START insert for rgauge (%03d) -->\n", n_instance);
  fprintf(d.js_fp, "// --  START insert for rgauge (%03d)\n", n_instance);

  char js_object_name[30];
  snprintf(js_object_name, sizeof(js_object_name), "rgauge_obj_%03d", n_instance);
  fprintf(d.js_fp, "var %s = new rgauge_t(\"rgauge_pv_%03d\", "
         "\"rgauge_eu_%03d\", \"rgauge_hi_%03d\", \"rgauge_lo_%03d\", %lf, %lf, %lf);\n", 
         js_object_name, n_instance, n_instance, n_instance, 
           n_instance, x + (3.5 * scale_factor), y + (3.5 * scale_factor), 
           width * (60.0/63.5)); 

  fprintf(d.svg_fp, "<circle cx=\"%lf\" cy=\"%lf\" r=\"%lf\" fill=\"antiquewhite\" stroke=\"black\" stroke-width=\"%lf\" filter=\"url(#filter1)\"/>\n", 
         cx, cy, r, 2.0 * scale_factor);

  fprintf(d.svg_fp, "<text id=\"rgauge_pv_%03d\" text-anchor=\"middle\" x=\"%lf\" y=\"%lf\" font-size=\"%lf\" fill=\"%s\" filter=\"url(#filter1)\">00</text>\n",
         n_instance, cx - (5 * scale_factor), cy + (15 * scale_factor), 50.0 * scale_factor, rcolor); 

  fprintf(d.svg_fp, "<line x1=\"%lf\" y1=\"%lf\" x2=\"%lf\" y2=\"%lf\" stroke =\"darkgray\" stroke-width=\"%lf\"/>\n", 
         cx - (46 * scale_factor), cy - (23 * scale_factor),
         cx + (37 * scale_factor), cy - (23 * scale_factor),
         0.6 * scale_factor);

  fprintf(d.svg_fp, "<line x1=\"%lf\" y1=\"%lf\" x2=\"%lf\" y2=\"%lf\" stroke =\"darkgray\" stroke-width=\"%lf\"/>\n",
         cx - (46.0 * scale_factor), cy + (21.0 * scale_factor),
         cx + (37.0 * scale_factor), cy + (21.0 * scale_factor),
         0.6 * scale_factor);


  fprintf(d.svg_fp, "<text id=\"rgauge_eu_%03d\" text-anchor=\"middle\" x=\"%lf\" y=\"%lf\" font-size=\"%lf\" fill=\"%s\" filter=\"url(#filter1)\">amps</text>\n",
                n_instance, cx, cy + (33.0 * scale_factor), 15 * scale_factor, rcolor); 

  fprintf(d.svg_fp, "<text id=\"rgauge_hi_%03d\" text-anchor=\"start\" x=\"%lf\" y=\"%lf\" font-size=\"%lf\" fill=\"%s\" filter=\"url(#filter1)\">0</text>\n", 
       n_instance, cx - (23.0 * scale_factor), cy + (50.0 * scale_factor), 8 * scale_factor, rcolor); 

  fprintf(d.svg_fp, "<text id=\"rgauge_lo_%03d\" text-anchor=\"end\" x=\"%lf\" y=\"%lf\" font-size=\"%lf\" fill=\"%s\" filter=\"url(#filter1)\">30</text>\n", 
        n_instance, cx + (24.0 * scale_factor), cy + (50.0 * scale_factor), 8 * scale_factor, rcolor); 

  fprintf(d.svg_fp, "<line x1=\"%lf\" y1=\"%lf\" x2=\"%lf\" y2=\"%lf\" stroke =\"darkgray\" stroke-width=\"%lf\"/>\n", 
        cx - (23.0 * scale_factor), cy + (52.0 * scale_factor), 
        cx + (25.0 * scale_factor), cy + (52.0 * scale_factor), 
        0.6 * scale_factor); 

  fprintf(d.svg_fp, "<!--  END insert for rgauge (%03d) -->\n", n_instance);
  fprintf(d.js_fp, "// --  END insert for rgauge (%03d)\n", n_instance);
  add_svg_library("filter1.svg");
  //add_js_library("scales.js");
  add_js_library("rgauge.js");
  add_animation_object(tag, js_object_name);

  n_instance++;
}

