
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "gen_display.h"

static int n_instance = 1;

class pvalve3way_t : public gen_plugin_base_t
{
public:
  const char *get_name(void); 
  void generate(plugin_data_t d, int argc, char **argv);
};

extern "C" gen_plugin_base_t *get_object(void)
{
  return new pvalve3way_t;
}

const char *pvalve3way_t::get_name(void)
{
  return "pvalve3way";
}

void pvalve3way_t::generate(plugin_data_t d, int argc, char **argv)
{
  const char *tag = argv[1];
  const char *on_color = argv[2];
  const char *off_color = argv[3];
  double cx = atof(argv[4]);
  double cy = atof(argv[5]);
  double width = atof(argv[6]);
  int type = atoi(argv[7]);
  int angle = atoi(argv[8]);

  double scale_factor = width / 50.0;
  double x1 = cx - (width / 2.0);
  double x2 = cx + (width / 2.0);
  double y1 = cy - (30.0 * scale_factor);
  double width_factor = 17.5;

//tag|on_color|off_collor|x1|y1|width|type(1-2)|rotation(0,90,180,270)|

  if (type > 2) type = 2;
  if (type < 1) type = 1;

  char js_object_name[30];
  char js_out1_name[30];
  char js_out2_name[30];
  snprintf(js_object_name, sizeof(js_object_name),   "pvalve3way_obj_%03d",  n_instance);
  snprintf(js_out1_name,   sizeof(js_object_name),   "pvalve3way_out1_%03d", n_instance);
  snprintf(js_out2_name,   sizeof(js_object_name),   "pvalve3way_out2_%03d", n_instance);

  char transform_str[200];
  if (angle == 0) transform_str[0] = '\0';
  else snprintf(transform_str, sizeof(transform_str), "transform=\"rotate(%d %lf,%lf)\"", angle, cx, cy);

  fprintf(d.svg_fp, "<!--  START insert for pvalve3way (%03d) -->\n", n_instance);


  fprintf(d.svg_fp, "<g stroke=\"black\" stroke-width=\"%lf\" %s>\n", 
       0.5 * scale_factor, transform_str);
  if (type == 1)
  {
    fprintf(d.svg_fp, "  <path  fill=\"%s\" \n", on_color);
    fprintf(d.svg_fp, "     d=\"M%lf,%lf A%lf,%lf 0 1,1 %lf,%lf Z\"/>\n",
           x1 + (10.0 * scale_factor), y1 + (10.0 * scale_factor), 
           5.0 * scale_factor, 3.0 * scale_factor, 
           x1 + (40.0 * scale_factor), y1 + (10.0 * scale_factor));
  }
  else if (type == 2)
  {
    fprintf(d.svg_fp, "  <rect fill=\"%s\" x=\"%lf\" y=\"%lf\" width=\"%lf\" height=\"%lf\"/>\n",
                 on_color, x1 + (10.0 * scale_factor), y1, 
                 30.0 * scale_factor, 10.0 * scale_factor); 
  }
  fprintf(d.svg_fp, "  <polygon fill=\"%s\" points=\"%lf,%lf %lf,%lf %lf,%lf\"/>\n",
                on_color,
                x2, cy + (width_factor * scale_factor), //y1 + (10.0 * scale_factor),
                x2, cy - (width_factor * scale_factor), //y2, 
                cx, cy);

  fprintf(d.svg_fp, "  <polygon fill=\"%s\" points=\"%lf,%lf %lf,%lf %lf,%lf\"/>\n",
                off_color,
                x1, cy + (width_factor * scale_factor), //y1 + (10.0 * scale_factor),
                x1, cy - (width_factor * scale_factor), //y2, 
                cx, cy);
  fprintf(d.svg_fp, "  <polygon id=\"pvalve3way_out1_%03d\" fill=\"%s\" points=\"%lf,%lf %lf,%lf %lf,%lf\"/>\n",
                n_instance, on_color,
                x1, cy + (width_factor * scale_factor), //y1 + (10.0 * scale_factor),
                x1, cy - (width_factor * scale_factor), //y2, 
                cx, cy);

  fprintf(d.svg_fp, "  <polygon fill=\"%s\" points=\"%lf,%lf %lf,%lf %lf,%lf\"/>\n",
                off_color,
                cx - (width_factor * scale_factor), cy + (width/2.0),
                cx + (width_factor * scale_factor), cy + (width/2.0),
                cx, cy);
  fprintf(d.svg_fp, "  <polygon id=\"pvalve3way_out2_%03d\" fill=\"%s\" points=\"%lf,%lf %lf,%lf %lf,%lf\"/>\n",
                n_instance, on_color,
                cx - (width_factor * scale_factor), cy + (width/2.0),
                cx + (width_factor * scale_factor), cy + (width/2.0),
                cx, cy);

  fprintf(d.svg_fp, "  <line x1=\"%lf\" y1=\"%lf\" x2=\"%lf\" y2=\"%lf\" \n",
               cx, y1 + (10.0 * scale_factor), cx, cy);
  fprintf(d.svg_fp, "     stroke =\"black\" stroke-width=\"%lf\"/>\n",
           1.5 * scale_factor);
  fprintf(d.svg_fp, "</g>\n");

//function pvalve3way_t(out1_name, out2_name, on_color, off_color, cx, cy, d, x1, x3, y1, y2)
 fprintf(d.js_fp, "var %s = new pvalve3way_t(\"%s\", \"%s\", \"%s\", \"%s\",\n",
             js_object_name, js_out1_name, js_out2_name, on_color, off_color);
 fprintf(d.js_fp, "         %lf, %lf, %lf, %lf, %lf, %lf, %lf);\n",
             cx, cy, width/2.0, 
             cx - (width_factor * scale_factor), 
             cx + (width_factor * scale_factor),
             cy + (width_factor * scale_factor),
             cy - (width_factor * scale_factor) );

  double px, py;
  find_a_place_nearby(&px, &py, x1, y1, width, width);
  fprintf(d.svg_fp, "<rect x=\"%lf\"  y=\"%lf\" width=\"%lf\" height=\"%lf\" onclick=\"show_popup(%lf,%lf,'Open', 'Close', '%s')\" visibility=\"hidden\" pointer-events=\"all\" onmouseover=\"this.style.cursor='pointer';\"/>\n",
         x1, y1, width, width, px, py, tag);


  fprintf(d.js_fp, "// --  END insert for pvalve3way (%03d)\n", n_instance);
  fprintf(d.svg_fp, "<!--  END insert for pvalve3way (%03d) -->\n", n_instance);
  n_instance++;

  add_js_library("pvalve3way.js");
  add_animation_object(tag, js_object_name);
}

