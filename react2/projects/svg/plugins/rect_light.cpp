
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "gen_display.h"

static int n_instance = 1;

class rect_light_t : public gen_plugin_base_t
{
public:
  const char *get_name(void); 
  void generate(plugin_data_t d, int argc, char **argv);
};

extern "C" gen_plugin_base_t *get_object(void)
{
  return new rect_light_t;
}

const char *rect_light_t::get_name(void)
{
  return "rect_light";
}

void rect_light_t::generate(plugin_data_t d, int argc, char **argv)
{
  const char *tag = argv[1];
  const char *on_color = argv[2];
  const char *off_color = argv[3];
  double x = atof(argv[4]);
  double y = atof(argv[5]);
  double width = atof(argv[6]);
  double height = atof(argv[7]);
  char text_location = toupper(argv[8][0]);
  double stroke_width = ((width + height) / 2) * 0.1;
  double font_size = ((width + height) / 2 )  * 0.6;
  double text_x, text_y;
  const char *text_align;
  bool has_text=true;
  
  switch (text_location)
   {
	case 'B': text_x =  x + (width/2);
                  text_y =  y+height + (font_size * 1.1);
                  text_align = "middle";
                  break;
 	case 'A': text_x =  x + (width/2);
                  text_y =  y - (font_size  / 3 );
                  text_align = "middle";
                  break;
 	case 'L': text_x =  x - (font_size * 0.2); 
                  text_y =  y + (font_size  * 0.7);
                  text_align = "end";
                  break;
 	case 'R': text_x =  x + width + (font_size * 0.2);
                  text_y =  y + (font_size * 0.7);
                  text_align = "start";
                  break;
  	default:  text_x =  0;
                  text_y =  0;
                  text_align = NULL;
                  has_text = false;
                  break;
 }
                

  fprintf(d.svg_fp, "<!--  START insert for rect_light (%03d) -->\n", n_instance);
  fprintf(d.js_fp, "// --  START insert for rect_light (%03d)\n", n_instance);

  fprintf(d.svg_fp, "<rect id=\"rect_light_%03d\" x=\"%lf\" y=\"%lf\" width=\"%lf\" height=\"%lf\" fill=\"%s\" stroke=\"black\" stroke-width=\"%lf\"/>\n",
            n_instance, x, y, width, height, on_color, stroke_width);
  if (has_text)
  	fprintf(d.svg_fp, "<text id=\"rect_light_pv_%03d\" x=\"%lf\" y=\"%lf\" font-family=\"Verdana\" font-size=\"%lf\" fill=\"black\" text-anchor=\"%s\">0</text>\n",
                     n_instance, text_x, text_y, font_size, text_align);


  char js_object_name[30];
  char js_pv_name[30];
  snprintf(js_object_name, sizeof(js_object_name), "rect_light_%03d", n_instance);
  if (has_text)
     snprintf(js_pv_name, sizeof(js_pv_name), "\"rect_light_pv_%03d\"", n_instance);
  else 
     snprintf(js_pv_name, sizeof(js_pv_name), "null");

  fprintf(d.js_fp, "var %s = new rect_light_t(\"rect_light_%03d\", %s, \"%s\", \"%s\");\n", js_object_name, n_instance, js_pv_name, on_color, off_color); 

  fprintf(d.svg_fp, "<!--  END insert for rect_light (%03d) -->\n", n_instance);
  fprintf(d.js_fp, "// --  END insert for rect_light (%03d)\n", n_instance);

  add_js_library("rect_light.js");
  add_animation_object(tag, js_object_name);

  n_instance++;
}

