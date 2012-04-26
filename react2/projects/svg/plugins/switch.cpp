
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "gen_display.h"

static int n_instance = 1;

class switch_t : public gen_plugin_base_t
{
public:
  const char *get_name(void); 
  void generate(FILE *svg_fp, FILE *svg_top_of_file_fp, FILE *js_fp, int argc, char **argv);
};

extern "C" gen_plugin_base_t *get_object(void)
{
  return new switch_t;
}

const char *switch_t::get_name(void)
{
  return "switch";
}

void switch_t::generate(FILE *svg_fp, FILE *svg_top_of_file_fp, FILE *js_fp, int argc, char **argv)
{
  const char *tag = argv[1];
  const char *on_color = argv[2];
  const char *off_color = argv[3];
  double x1 = atof(argv[4]);
  double y1 = atof(argv[5]);
  double width = atof(argv[6]);
  double font_size = width * 0.6;
  double scale_factor = width / 100;
  double cx = x1 + (width / 2.0);
  double cy = y1 + (60.0 * scale_factor);

  char js_object_name[30];
  char js_group_name[30];
  char js_on_name[30];
  char js_off_name[30];
  char js_click1_name[30];
  char js_click2_name[30];
  char js_handler_name[30];
  snprintf(js_object_name, sizeof(js_object_name), "switch_obj_%03d", n_instance);
  snprintf(js_click1_name, sizeof(js_click1_name), "switch_click1_%03d", n_instance);
  snprintf(js_click2_name, sizeof(js_click2_name), "switch_click2_%03d", n_instance);
  snprintf(js_handler_name, sizeof(js_handler_name), "switch_handler_%03d", n_instance);
  snprintf(js_group_name, sizeof(js_group_name), "switch_group_%03d", n_instance);
  snprintf(js_on_name, sizeof(js_on_name), "switch_on_%03d", n_instance);
  snprintf(js_off_name, sizeof(js_off_name), "switch_off_%03d", n_instance);

  fprintf(svg_fp, "<!--  START insert for switch (%03d) -->\n", n_instance);
  fprintf(js_fp, "// --  START insert for switch (%03d)\n", n_instance);
  /***********************/

//  <rect id="rect_001" x="0" y="0" rx="3" ry="3" width="100" height="105" fill="lightgray" stroke="darkgray" stroke-width="1"/>


  //fprintf(svg_fp, "<rect  id=\"%s\" x=\"2\" y=\"2\" rx=\"4\" ry=\"4\" width=\"40\" height=\"15\" fill=\"red\" stroke=\"black\" stroke-width=\"1\"/>\n", 
  fprintf(svg_fp, "<rect  id=\"%s\" x=\"%lf\" y=\"%lf\" rx=\"%lf\" ry=\"%lf\" width=\"%lf\" height=\"%lf\" fill=\"red\" stroke=\"black\" stroke-width=\"%lf\"/>\n", 
    js_off_name, x1 + (2.0 * scale_factor), y1 + (2.0 * scale_factor), 
    4.0 * scale_factor, 4.0 * scale_factor, 40.0 * scale_factor, 15.0 * scale_factor, scale_factor);
  //fprintf(svg_fp, "<text id=\"textxxx_001\" x=\"21\" y=\"12.5\" font-size=\"10\" fill=\"black\" text-anchor=\"middle\">Off</text>\n");
  fprintf(svg_fp, "<text id=\"textxxx_001\" x=\"%lf\" y=\"%lf\" font-size=\"%lf\" fill=\"black\" text-anchor=\"middle\">Off</text>\n",
     x1 + 21.0 * scale_factor, y1 + 12.5 * scale_factor, 10.0 * scale_factor);
  //fprintf(svg_fp, "<rect  id=\"%s\" x=\"58\" y=\"2\"  rx=\"4\" ry=\"4\" width=\"40\" height=\"15\" fill=\"gray\" stroke=\"black\" stroke-width=\"1\"/>\n", js_on_name);
  fprintf(svg_fp, "<rect  id=\"%s\" x=\"%lf\" y=\"%lf\"  rx=\"%lf\" ry=\"%lf\" width=\"%lf\" height=\"%lf\" fill=\"gray\" stroke=\"black\" stroke-width=\"%lf\"/>\n", 
    js_on_name, x1 + (58.0 * scale_factor), y1 + (2.0 * scale_factor), 
    4.0 * scale_factor, 4.0 * scale_factor, 40.0 * scale_factor, 15.0 * scale_factor, scale_factor);
  fprintf(svg_fp, "<text id=\"textxx_002\" x=\"%lf\" y=\"%lf\" font-size=\"%lf\" fill=\"black\" text-anchor=\"middle\">On</text>\n",
     x1 + 79.0 * scale_factor, y1 + 12.5 * scale_factor, 10.0 * scale_factor);
  fprintf(svg_fp, "<circle cx=\"%lf\" cy=\"%lf\" r=\"%lf\" stroke=\"darkgray\" stroke-width=\"1\" fill=\"gray\"/>\n",
     cx, cy, 40.0 * scale_factor);
  fprintf(svg_fp, "<circle cx=\"%lf\" cy=\"%lf\" r=\"%lf\" stroke=\"none\" fill=\"white\"/>\n",
     cx, cy, 38.0 * scale_factor);
  fprintf(svg_fp, "<circle cx=\"%lf\" cy=\"%lf\" r=\"%lf\" stroke=\"none\" fill=\"url(#grayRadial)\"/>\n",
     cx, cy, 37.0 * scale_factor);
  fprintf(svg_fp, "<g id=\"%s\" transform=\"rotate(-45 %lf,%lf)\">\n", js_group_name, cx, cy);
  fprintf(svg_fp, "  <circle id=\"c1\" cx=\"%lf\" cy=\"%lf\" r=\"%lf\" stroke=\"none\" fill=\"url(#grayLinearV)\"/>\n",
     cx, cy, 30.0 * scale_factor);
  fprintf(svg_fp, "  <rect id=\"r1\" x=\"%lf\" y=\"%lf\" width=\"%lf\" height=\"%lf\" rx=\"%lf\" ry=\"%lf\" fill=\"url(#grayLinearX)\" stroke=\"none\"/>\n", 
        x1 + (45.0 * scale_factor), y1 + (16.0 * scale_factor), 10.0 * scale_factor, 
           87.0 * scale_factor, 4.0 * scale_factor, 4.0 * scale_factor);
  fprintf(svg_fp, "  <circle id=\"p1\" cx=\"%lf\" cy=\"%lf\" r=\"%lf\" stroke=\"none\" fill=\"white\"/>\n",
                 cx, y1 + (22.0 * scale_factor), 2.0 * scale_factor);
  fprintf(svg_fp, "</g>\n");
  fprintf(svg_fp, "<rect id=\"rect_001\" x=\"%lf\"  y=\"%lf\" width=\"%lf\" height=\"%lf\" onclick=\"%s()\" visibility=\"hidden\" pointer-events=\"all\"/>\n", 
          x1, y1, 42.0 * scale_factor, 40.0 * scale_factor, js_click1_name);
  fprintf(svg_fp, "<rect id=\"rect_001\" x=\"%lf\" y=\"%lf\" width=\"%lf\" height=\"%lf\" onclick=\"%s()\" visibility=\"hidden\" pointer-events=\"all\"/>\n", 
          x1 + (58.0 * scale_factor), y1, (42.0 * scale_factor), 40.0 * scale_factor, js_click2_name);



  /***********************/
  
  fprintf(js_fp, "function %s()\n", js_click1_name); 
  fprintf(js_fp, "{\n");
  fprintf(js_fp, "  %s.off_clicked();\n", js_object_name);
  fprintf(js_fp, "}\n");
  fprintf(js_fp, "function %s()\n", js_click2_name); 
  fprintf(js_fp, "{\n");
  fprintf(js_fp, "  %s.on_clicked();\n", js_object_name);
  fprintf(js_fp, "}\n");
  fprintf(js_fp, "function %s()\n", js_handler_name); 
  fprintf(js_fp, "{\n");
  fprintf(js_fp, "  %s.interval_handler();\n", js_object_name);
  fprintf(js_fp, "}\n");

  fprintf(js_fp, "var %s = new switch_t(\"%s\", \"%s\", \"%s\", \"%s\", \"%s\", %lf, %lf, \"%s()\");\n", js_object_name, js_group_name, js_on_name, js_off_name, on_color, off_color, cx, cy, js_handler_name); 

  fprintf(svg_fp, "<!--  END insert for switch (%03d) -->\n", n_instance);
  fprintf(js_fp, "// --  END insert for switch (%03d)\n", n_instance);

  add_js_library("switch.js");
  add_svg_library("switch_grad.svg");
  add_animation_object(tag, js_object_name);

  n_instance++;
}

