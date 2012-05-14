
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "gen_display.h"

static int n_instance = 1;

class motor_t : public gen_plugin_base_t
{
public:
  const char *get_name(void); 
  void generate(plugin_data_t d, int argc, char **argv);
};

extern "C" gen_plugin_base_t *get_object(void)
{
  return new motor_t;
}

const char *motor_t::get_name(void)
{
  return "motor";
}

void motor_t::generate(plugin_data_t d, int argc, char **argv)
{
  const char *tag = argv[1];
  const char *on_color = argv[2];
  const char *off_color = argv[3];
  //double x1 = atof(argv[4]);
  //double y1 = atof(argv[5]);
  double cx = atof(argv[4]);
  double cy = atof(argv[5]);
  double width = atof(argv[6]);
  int angle = atoi(argv[7]);
  double scale_factor = width / 100.0;
  double height = 54.0 * scale_factor;

  //double cx = x1 + (width / 2.0);
  //double cy = y1 + (height / 2.0);
  double x1 = cx;
  double y1 = cy - (height / 2.0);

//motor|tag|on_color|off_collor|x1|y1|width|rotation(0,90,180,270)|

  char js_object_name[30];
  char js_group_name[30];
  snprintf(js_object_name, sizeof(js_object_name), "motor_obj_%03d", n_instance);
  snprintf(js_group_name, sizeof(js_group_name), "motor_group_obj_%03d", n_instance);

  char str[200];
  if (angle == 0) str[0] = '\0';
  else snprintf(str, sizeof(str), "transform=\"rotate(%d %lf,%lf)\"", angle, cx, cy);

  fprintf(d.svg_fp, "<!--  START insert for motor (%03d) -->\n", n_instance);
  fprintf(d.js_fp, "// --  START insert for motor (%03d)\n", n_instance);

  fprintf(d.svg_fp, "<g  %s >\n", str);
  fprintf(d.svg_fp, "<rect id=\"%s\" x=\"%lf\" y=\"%lf\" rx=\"%lf\" ry=\"%lf\" width=\"%lf\" height=\"%lf\" fill=\"%s\" stroke=\"black\" stroke-width=\"%lf\"/>\n",
       js_group_name, x1, y1, 5.0 * scale_factor, 8.0 * scale_factor, width, height, on_color, 0.6 * scale_factor);
  double fin_x = x1 + 20 * scale_factor; 
  double fin_width = 75.0 * scale_factor;
  double fin_height = 3.0 * scale_factor;
  double fin_rx = 1.5 * scale_factor;
  double fin_stroke_width = 0.5 * scale_factor;
  for (int i=0; i < 6; i++)
  {
    double fin_y = y1 + (((i * 10.0) + 1.0) * scale_factor);
    fprintf(d.svg_fp, "  <rect x=\"%lf\" y=\"%lf\" rx=\"%lf\" ry=\"%lf\" width=\"%lf\" height=\"%lf\"\n"
         "        fill=\"black\" stroke=\"black\" stroke-width=\"%lf\"/>\n", 
      fin_x, fin_y, fin_rx, fin_rx, fin_width, fin_height, fin_stroke_width);
  }
  fprintf(d.svg_fp, "<line x1=\"%lf\" y1=\"%lf\" x2=\"%lf\" y2=\"%lf\" stroke=\"black\" stroke-width=\"%lf\"/>\n",
          fin_x, y1, fin_x, y1 + height, 1.5 * scale_factor);
  fprintf(d.svg_fp, "<line x1=\"%lf\" y1=\"%lf\" x2=\"%lf\" y2=\"%lf\" stroke=\"black\" stroke-width=\"%lf\"/>\n", 
          fin_x + fin_width, y1, fin_x + fin_width, y1 + height, 1.5 * scale_factor);

  fprintf(d.svg_fp, "</g>\n");


//name, on_val, off_val
  fprintf(d.js_fp, "var  xxx = [{\"name\":\"%s\", \"property\":\"color\", \"on_val\":\"%s\", \"off_val\":\"%s\"}];\n",
            js_group_name, on_color, off_color);
  //for (var i=0; i < xxx.length;  i++)
  //{
   // consolo.log("name: " + this.objs[i].name + ", property: " + this.objs[i].property + ", on_val: " + this.objs[i].on_val + ", off_val: " + this.objs[i].off_val);
  //}
  fprintf(d.js_fp, "console.log(xxx)\n");
  //fprintf(d.js_fp, "var %s = new discrete_t(\"[{'name':'%s', 'property':'color', 'on_val':'%s', 'off_val':'%s'}]\");\n", 
  fprintf(d.js_fp, "var %s = new discrete_t(xxx);\n", 
            js_object_name);//, js_group_name, on_color, off_color);

  double px, py;
  find_a_place_nearby(&px, &py, x1, y1, width, width);
  fprintf(d.svg_fp, "<rect x=\"%lf\"  y=\"%lf\" width=\"%lf\" height=\"%lf\" onclick=\"show_popup(%lf,%lf,'ON', 'Off', '%s')\" visibility=\"hidden\" pointer-events=\"all\" onmouseover=\"this.style.cursor='pointer';\"/>\n",
         x1, y1, width, width, px, py, tag);


  fprintf(d.js_fp, "// --  END insert for motor (%03d)\n", n_instance);
  fprintf(d.svg_fp, "<!--  END insert for motor (%03d) -->\n", n_instance);
  add_js_library("discrete.js");
  add_animation_object(tag, js_object_name);

  n_instance++;
}

