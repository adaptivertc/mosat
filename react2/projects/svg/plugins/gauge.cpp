
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "gen_display.h"

static int n_instance = 1;

class gauge_t : public gen_plugin_base_t
{
public:
  const char *get_name(void); 
  void generate(FILE *svg_fp, FILE *svg_top_of_file_fp, FILE *js_fp, int argc, char **argv);
};

extern "C" gen_plugin_base_t *get_object(void)
{
  return new gauge_t;
}

const char *gauge_t::get_name(void)
{
  return "gauge";
}

void gauge_t::generate(FILE *svg_fp, FILE *svg_top_of_file_fp, FILE *js_fp, int argc, char **argv)
{
  const char *tag = argv[1];
  const char *color = argv[2];
  double x = atof(argv[3]);
  double y = atof(argv[4]);
  double width = atof(argv[5]);
  //double width = height * 0.2;
  //double font_size = height * 0.1;
  //double cx = x + (width/2.0);
  //double cy = y + (height/2.0);
  //double stroke_width = height / 100.0;

  double cx = x + width / 2.0;
  double cy = y + width / 2.0;
  
  fprintf(svg_fp, "<!--  START insert for gauge (%03d) -->\n", n_instance);
  fprintf(js_fp, "// --  START insert for gauge (%03d)\n", n_instance);

  char js_object_name[30];
  snprintf(js_object_name, sizeof(js_object_name), "gauge_obj_%03d", n_instance);

  fprintf(js_fp, "var %s = new gauge_t(\"gauge_needle_%03d\", \"gauge_pv_%03d\", \"gauge_eu_%03d\", %lf, %lf, %lf);\n", 
           js_object_name, n_instance, n_instance, n_instance, x, y, width); 

  fprintf(svg_fp, "<rect x=\"%lf\" y=\"%lf\" rx=\"%lf\" ry=\"%lf\" width=\"%lf\" height=\"%lf\"\n",
               x, y, width * (1.0/15.0), width * (1.0/15.0), width, width);
  fprintf(svg_fp, "style=\"fill:#2F4F4F;stroke:black;stroke-width:%lf;opacity:1.0\"/>\n", width *(2.0/150));

  fprintf(svg_fp, "<circle cx=\"%lf\" cy=\"%lf\" r=\"%lf\" fill=\"white\"/>\n", 
             cx, cy, width * (70.0/150.0));


  /***
  fprintf(svg_fp, "<path fill=\"none\" stroke=\"yellow\" stroke-width=\"5.000000\"\n");
  fprintf(svg_fp, "  d=\"M118.388164,23.292000 A67.500000,67.500000 0 0,1 141.474523,63.278748\"/>\n");

  fprintf(svg_fp, "<path fill=\"none\" stroke=\"red\" stroke-width=\"5.000000\"\n");
  fprintf(svg_fp, "   d=\"M141.474523,63.278748 A67.500000,67.500000 0 0,1 122.729708,122.729708\"/>\n");
***/


  fprintf(svg_fp, "  <circle cx=\"%lf\" cy=\"%lf\" r=\"%lf\" fill=\"cornsilk\"\n", 
                          cx, cy, width * (65.0/150.0));
  fprintf(svg_fp, "         style=\"stroke:rgb(30,30,30);stroke-width:%lf\"/>\n", width / 150.0);

  double screw_cx = width * 0.1;
  double screw_cy = width * 0.1; 
  double screw_x1 = width * (13.0/150.0);
  double screw_y1 = width * 0.1;
  double screw_x2 = width * (17.0/150.0);
  double screw_y2 = width * 0.1;

  double screw_r = width * (3.0/150.0);
  double screw_stroke_width = width * (1.2/150.0);

  fprintf(svg_fp, "  <circle cx=\"%lf\" cy=\"%lf\" r=\"%lf\" stroke=\"none\" stroke-width=\"0\" fill=\"black\"/>\n",
                  x + screw_cx, y + screw_cy, screw_r);
  fprintf(svg_fp, "  <line x1=\"%lf\" y1=\"%lf\" x2=\"%lf\" y2=\"%lf\" transform=\"rotate(20 %lf %lf)\"\n", 
                      x + screw_x1, y + screw_y1, x + screw_x2, y + screw_y2, x + screw_cx, y + screw_cy);
  fprintf(svg_fp, "      style=\"stroke:rgb(64,65,65);stroke-width:%lf\"/>\n", screw_stroke_width);
  fprintf(svg_fp, "  <line x1=\"%lf\" y1=\"%lf\" x2=\"%lf\" y2=\"%lf\" transform=\"rotate(110 %lf %lf)\"\n",
                      x + screw_x1, y + screw_y1, x + screw_x2, y + screw_y2, x + screw_cx, y + screw_cy);
  fprintf(svg_fp, "      style=\"stroke:rgb(64,65,65);stroke-width:%lf\"/>\n", screw_stroke_width);

  screw_cx = width - screw_cx;
  screw_x1 = width - screw_x1;
  screw_x2 = width - screw_x2;

  fprintf(svg_fp, "  <circle cx=\"%lf\" cy=\"%lf\" r=\"%lf\" stroke=\"none\" stroke-width=\"0\" fill=\"black\"/>\n",
                  x + screw_cx, y + screw_cy, screw_r);
  fprintf(svg_fp, "  <line x1=\"%lf\" y1=\"%lf\" x2=\"%lf\" y2=\"%lf\" transform=\"rotate(-10 %lf %lf)\"\n", 
                      x + screw_x1, y + screw_y1, x + screw_x2, y + screw_y2, x + screw_cx, y + screw_cy);
  fprintf(svg_fp, "      style=\"stroke:rgb(64,65,65);stroke-width:%lf\"/>\n", screw_stroke_width);
  fprintf(svg_fp, "  <line x1=\"%lf\" y1=\"%lf\" x2=\"%lf\" y2=\"%lf\" transform=\"rotate(80 %lf %lf)\"\n",
                      x + screw_x1, y + screw_y1, x + screw_x2, y + screw_y2, x + screw_cx, y + screw_cy);
  fprintf(svg_fp, "      style=\"stroke:rgb(64,65,65);stroke-width:%lf\"/>\n", screw_stroke_width);

  screw_cy = width - screw_cy;
  screw_y1 = width - screw_y1;
  screw_y2 = width - screw_y2;

  fprintf(svg_fp, "  <circle cx=\"%lf\" cy=\"%lf\" r=\"%lf\" stroke=\"none\" stroke-width=\"0\" fill=\"black\"/>\n",
                  x + screw_cx, y + screw_cy, screw_r);
  fprintf(svg_fp, "  <line x1=\"%lf\" y1=\"%lf\" x2=\"%lf\" y2=\"%lf\" transform=\"rotate(17 %lf %lf)\"\n", 
                      x + screw_x1, y + screw_y1, x + screw_x2, y + screw_y2, x + screw_cx, y + screw_cy);
  fprintf(svg_fp, "      style=\"stroke:rgb(64,65,65);stroke-width:%lf\"/>\n", screw_stroke_width);
  fprintf(svg_fp, "  <line x1=\"%lf\" y1=\"%lf\" x2=\"%lf\" y2=\"%lf\" transform=\"rotate(107 %lf %lf)\"\n",
                      x + screw_x1, y + screw_y1, x + screw_x2, y + screw_y2, x + screw_cx, y + screw_cy);
  fprintf(svg_fp, "      style=\"stroke:rgb(64,65,65);stroke-width:%lf\"/>\n", screw_stroke_width);

  screw_cx = width - screw_cx;
  screw_x1 = width - screw_x1;
  screw_x2 = width - screw_x2;

  fprintf(svg_fp, "  <circle cx=\"%lf\" cy=\"%lf\" r=\"%lf\" stroke=\"none\" stroke-width=\"0\" fill=\"black\"/>\n",
                  x + screw_cx, y + screw_cy, screw_r);
  fprintf(svg_fp, "  <line x1=\"%lf\" y1=\"%lf\" x2=\"%lf\" y2=\"%lf\" transform=\"rotate(3 %lf %lf)\"\n", 
                      x + screw_x1, y + screw_y1, x + screw_x2, y + screw_y2, x + screw_cx, y + screw_cy);
  fprintf(svg_fp, "      style=\"stroke:rgb(64,65,65);stroke-width:%lf\"/>\n", screw_stroke_width);
  fprintf(svg_fp, "  <line x1=\"%lf\" y1=\"%lf\" x2=\"%lf\" y2=\"%lf\" transform=\"rotate(93 %lf %lf)\"\n",
                      x + screw_x1, y + screw_y1, x + screw_x2, y + screw_y2, x + screw_cx, y + screw_cy);
  fprintf(svg_fp, "      style=\"stroke:rgb(64,65,65);stroke-width:%lf\"/>\n", screw_stroke_width);

  char needle_d[200];
  double needle_r = width * (6.0/150.0);
  snprintf(needle_d, sizeof(needle_d), "d=\"M%lf,%lf A%lf,%lf 0 1,1 %lf,%lf L%lf,%lf Z\"", 
                          cx - needle_r, cy, needle_r, needle_r, cx + needle_r, cy, 
                                       cx, y + (width * (140.0/150.0))),
  fprintf(svg_fp, "<!-- d: '%s' -->\n", needle_d); 

  /**
  fprintf(svg_fp, "  <path fill=\"blue\" id=\"gauge_sneedle1_%03d\" \n", n_instance);
  fprintf(svg_fp, "      %s style=\"fill-opacity:0.3\"\n", needle_d);
  fprintf(svg_fp, "         onmouseover=\"this.title='Testing, what will happen?';\"/>\n");

  fprintf(svg_fp, "  <path fill=\"blue\" id=\"gauge_sneedle2_%03d\" \n", n_instance);
  fprintf(svg_fp, "      %s style=\"fill-opacity:0.3\"\n", needle_d);
  fprintf(svg_fp, "         onmouseover=\"this.style.cursor='crosshair';\"/>\n");
  ***/


  fprintf(svg_fp, "  <path fill=\"%s\" id=\"gauge_needle_%03d\" \n", color, n_instance);
  fprintf(svg_fp, "      %s />\n", needle_d);


  fprintf(svg_fp, "  <circle cx=\"%lf\" cy=\"%lf\" r=\"%lf\" stroke=\"none\" stroke-width=\"0\" fill=\"black\"/>\n",
            cx, cy, width * (4.0/150.0));

  fprintf(svg_fp, "  <text id=\"gauge_pv_%03d\" x=\"%lf\" y=\"%lf\" font-size=\"%lf\" style=\"fill:%s;\" \n",
                            n_instance, cx, cy + (width * (25.0/150.0)), width * (20.0/150.0), color);
  fprintf(svg_fp, "       alignment-baseline=\"middle\" text-anchor=\"middle\">300</text>\n");
  fprintf(svg_fp, "\n");

/**
  fprintf(svg_fp, "  <text id=\"gauge_range_%03d\" x=\"%lf\" y=\"%lf\" font-size=\"%lf\" style=\"fill:blue;\" \n",
                            n_instance, cx, cy + (width * (45.0/150.0)), width * (15.0/150.0));
  fprintf(svg_fp, "       alignment-baseline=\"middle\" text-anchor=\"middle\">300</text>\n");
***/

  fprintf(svg_fp, "  <text id=\"gauge_eu_%03d\" x=\"%lf\" y=\"%lf\" font-size=\"%lf\" \n",
                            n_instance, cx, cy + (width * (45.0/150.0)), width * (15.0/150.0));
  fprintf(svg_fp, "       alignment-baseline=\"middle\" text-anchor=\"middle\">cm</text>\n");

  fprintf(svg_fp, "<!--  END insert for gauge (%03d) -->\n", n_instance);
  fprintf(js_fp, "// --  END insert for gauge (%03d)\n", n_instance);
  add_js_library("scales.js");
  add_js_library("gauge.js");
  add_animation_object(tag, js_object_name);

  n_instance++;
}

