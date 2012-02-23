
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "gen_display.h"

static int n_instance = 1;

class simple_contact_t : public gen_object_base_t
{
public:
  const char *get_name(void); 
  void generate(FILE *svg_fp, FILE *svg_after_header_fp, FILE *js_fp, int argc, char **argv);
};

extern "C" gen_object_base_t *get_object(void)
{
  return new simple_contact_t;
}

const char *simple_contact_t::get_name(void)
{
  return "simple_contact";
}

void simple_contact_t::generate(FILE *svg_fp, FILE *svg_after_header_fp, FILE *js_fp, int argc, char **argv)
{
  const char *tag = argv[1];
  const char *on_color = argv[2];
  const char *off_color = argv[3];
  double x = atof(argv[4]);
  double y = atof(argv[5]);
  double width = atof(argv[6]);
  double stroke_width = width * 0.10;
  double on_y2 = y + (width/2.0);
  double off_y2 = y + (width/4.0);

  fprintf(svg_fp, "<!--  START insert for simple_contact (%03d) -->\n", n_instance);
  fprintf(js_fp, "// --  START insert for simple_contact (%03d)\n", n_instance);

  fprintf(svg_fp, "<rect id=\"simple_contact_%03d\" x=\"%lf\" y=\"%lf\" width=\"%lf\" height=\"%lf\" fill=\"%s\" stroke=\"black\" stroke-width=\"%lf\"/>\n",
            n_instance, x, y, width, width, on_color, stroke_width);

  fprintf(svg_fp, "<line id=\"simple_contact_line_%03d\" x1=\"%lf\" y1=\"%lf\" x2=\"%lf\" y2=\"%lf\"\n       style=\"stroke:black;stroke-width:%lf\"/>\n", 
         n_instance, x, on_y2, x + (width * 0.75), on_y2, stroke_width);

  fprintf(svg_fp, "<line x1=\"%lf\" y1=\"%lf\" x2=\"%lf\" y2=\"%lf\"\n      style=\"stroke:black;stroke-width:%lf\"/>\n", 
         x + (width * 0.7), on_y2, x + width, on_y2, stroke_width);

  char js_object_name[30];
  snprintf(js_object_name, sizeof(js_object_name), "simple_contact_obj_%03d", n_instance);

  fprintf(js_fp, "var %s = new simple_contact_t(\"simple_contact_%03d\",\"simple_contact_line_%03d\", \"%s\", \"%s\", %lf, %lf);\n", 
       js_object_name, n_instance, n_instance, on_color, off_color, on_y2, off_y2); 

  fprintf(svg_fp, "<!--  END insert for simple_contact (%03d) -->\n", n_instance);
  fprintf(js_fp, "// --  END insert for simple_contact (%03d)\n", n_instance);

  add_js_library("simple_contact.js");
  add_update_object(tag, js_object_name);

  n_instance++;
}

