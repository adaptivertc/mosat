
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "gen_display.h"

static int n_instance = 1;

class only_text_t : public gen_object_base_t
{
public:
  const char *get_name(void); 
  void generate(FILE *svg_fp, FILE *svg_after_header_fp, FILE *js_fp, int argc, char **argv);
};

extern "C" gen_object_base_t *get_object(void)
{
  return new only_text_t;
}

const char *only_text_t::get_name(void)
{
  return "only_text";
}

void only_text_t::generate(FILE *svg_fp, FILE *svg_after_header_fp, FILE *js_fp, int argc, char **argv)
{
  const char *the_text = argv[1];
  const char *color = argv[2];
  double x = atof(argv[3]);
  double y = atof(argv[4]);
  double height = atof(argv[5]);
  const char *anchor = argv[6];
  
  fprintf(svg_fp, "<!--  START insert for only_text (%03d) -->\n", n_instance);
  fprintf(svg_fp, "<text id=\"only_text_%03d\" x=\"%lf\" y=\"%lf\" font-family=\"Verdana\" font-size=\"%lf\" fill=\"%s\" text-anchor=\"%s\">%s</text>\n",
                     n_instance, x, y, height, color, anchor, the_text); 
  fprintf(svg_fp, "<!--  END insert for only_text (%03d) -->\n", n_instance);
  n_instance++;
}

