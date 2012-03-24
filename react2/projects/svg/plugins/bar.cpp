
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "gen_display.h"

static int n_instance = 1;

class bar_t : public gen_plugin_base_t
{
public:
  const char *get_name(void); 
  void generate(FILE *svg_fp, FILE *svg_top_of_file_fp, FILE *js_fp, int argc, char **argv);
};

extern "C" gen_plugin_base_t *get_object(void)
{
  return new bar_t;
}

const char *bar_t::get_name(void)
{
  return "bar";
}

void bar_t::generate(FILE *svg_fp, FILE *svg_top_of_file_fp, FILE *js_fp, int argc, char **argv)
{
  const char *bar_color = "blue";
  const char *tag = argv[1];
  const char *the_color = argv[2];
  double x = atof(argv[3]);
  double y = atof(argv[4]);
  double height = atof(argv[5]);
  double width = height * 0.12;
  double font_size = height * 0.1;
  double cx = x + (width/2.0);
  double cy = y + (height/2.0);
  double stroke_width = height / 100.0;

  if (0 == strcasecmp(the_color, "yellow")) 
  {
    bar_color = "yellow";
    add_svg_library("yellow_gradients.svg");
  }
  else if (0 == strcasecmp(the_color, "black")) 
  {
    bar_color = "black";
    add_svg_library("black_gradients.svg");
  }
  else if (0 == strcasecmp(the_color, "orange")) 
  {
    bar_color = "orange";
    add_svg_library("orange_gradients.svg");
  }
  else if (0 == strcasecmp(the_color, "brown")) 
  {
    bar_color = "brown";
    add_svg_library("brown_gradients.svg");
  }
  else if (0 == strcasecmp(the_color, "white")) 
  {
    bar_color = "gray"; // Can NOT be white, no contrast with above bar
    add_svg_library("gray_gradients.svg");
  }
  else if (0 == strcasecmp(the_color, "red")) 
  {
    bar_color = "red";
    add_svg_library("red_gradients.svg");
  }
  else if (0 == strcasecmp(the_color, "blue")) 
  {
    bar_color = "blue";
    add_svg_library("blue_gradients.svg");
  }
  else if (0 == strcasecmp(the_color, "green")) 
  {
    bar_color = "green";
    add_svg_library("green_gradients.svg");
  }
  else if (0 == strcasecmp(the_color, "gray")) 
  {
    bar_color = "gray";
    add_svg_library("gray_gradients.svg");
  }
  else if (0 == strcasecmp(the_color, "grey")) 
  {
    bar_color = "gray";
    add_svg_library("gray_gradients.svg");
  }
  else // Anything else, use blue 
  {
    bar_color = "blue";
    add_svg_library("blue_gradients.svg");
  }
  add_svg_library("white_gradients.svg");

  
  fprintf(svg_fp, "<!--  START insert for bar (%03d) -->\n", n_instance);
  fprintf(js_fp, "// --  START insert for bar (%03d)\n", n_instance);

//----------
  fprintf(svg_fp, "  <rect fill=\"url(#whiteLinearV)\" x=\"%lf\" y=\"%lf\" width=\"%lf\" height=\"%lf\" stroke=\"none\" stroke-width=\"0\"/>\n",
                         x, y, width, height);
  fprintf(svg_fp, "  <rect fill=\"url(#%sLinearV)\" id=\"bar_%03d\" x=\"%lf\" y=\"%lf\" width=\"%lf\" height=\"%lf\" stroke=\"none\" stroke-width=\"0\" transform=\"rotate(180 %lf %lf)\"/>\n",
                         bar_color,
                         n_instance, x, y, width, height, cx, cy);
  fprintf(svg_fp, "  <rect x=\"%lf\" y=\"%lf\" width=\"%lf\" height=\"%lf\" fill=\"none\" stroke=\"#000000\" stroke-width=\"%lf\" />\n",
                         x, y, width, height, stroke_width);

//----------
//  fprintf(svg_fp, "<rect  fill=\"cornsilk\" x=\"%lf\" y=\"%lf\" width=\"%lf\" height=\"%lf\" stroke=\"black\" stroke-width=\"%lf\"/>\n",
 //                    x, y, width, height, stroke_width);
  //fprintf(svg_fp, "<rect  id=\"bar_%03d\" fill=\"%s\" x=\"%lf\" y=\"%lf\" width=\"%lf\" height=\"%lf\" stroke=\"none\" stroke-width=\"0\" transform=\"rotate(180 %lf %lf)\"/>\n",
//                     n_instance, color, (x + stroke_width / 2.0), 
 //                    y, width-stroke_width, height, cx, cy);
  fprintf(svg_fp, "<text id=\"bar_pv_%03d\" x=\"%lf\" y=\"%lf\" font-family=\"Verdana\" font-size=\"%lf\" fill=\"black\" text-anchor=\"middle\">0</text>\n",
                    n_instance, cx, y + height + (font_size * 1.1), font_size); 
//---------------

  char js_object_name[30];
  snprintf(js_object_name, sizeof(js_object_name), "bar_obj_%03d", n_instance);

  fprintf(js_fp, "var %s = new bar_t(\"bar_%03d\", \"bar_pv_%03d\", %lf, %lf, %lf, %lf, %lf);\n", 
               js_object_name, n_instance, n_instance, x, y, x+width, y+height, stroke_width); 

  fprintf(svg_fp, "<!--  END insert for bar (%03d) -->\n", n_instance);
  fprintf(js_fp, "// --  END insert for bar (%03d)\n", n_instance);
  add_js_library("scales.js");
  add_js_library("bar.js");
  //add_svg_library("bar_lib.svg");
  add_animation_object(tag, js_object_name);

  n_instance++;
}

/******
void gen_analog_bar(FILE *fp, const char *id, double x, double y, double width, double height)
{
  fprintf(fp, "  <rect fill=\"url(#grBarGrey)\" x=\"%lf\" y=\"%lf\" width=\"%lf\" height=\"%lf\" stroke=\"none\" stroke-width=\"0\"/>\n",
                         x, y, width, height);
  fprintf(fp, "  <rect fill=\"url(#grBarBlue)\" id=\"%s\" x=\"%lf\" y=\"%lf\" width=\"%lf\" height=\"%lf\" stroke=\"none\" stroke-width=\"0\" transform=\"rotate(180 %lf %lf)\"/>\n",
                         id, x, y, width, height, x + (width * 0.5), y + (height * 0.5));
  fprintf(fp, "  <rect x=\"%lf\" y=\"%lf\" width=\"%lf\" height=\"%lf\" fill=\"none\" stroke=\"#000000\" stroke-width=\"1\" />\n",
                         x, y, width, height);
}
*********/
