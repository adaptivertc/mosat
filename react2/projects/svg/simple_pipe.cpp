
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "gen_display.h"

static int n_instance = 1;

class simple_pipe_t : public gen_object_base_t
{
public:
  const char *get_name(void); 
  void generate(FILE *svg_fp, FILE *js_fp, int argc, char **argv);
};

extern "C" gen_object_base_t *get_object(void)
{
  return new simple_pipe_t;
}

const char *simple_pipe_t::get_name(void)
{
  return "simple_pipe";
}

void simple_pipe_t::generate(FILE *svg_fp, FILE *js_fp, int argc, char **argv)
{
  double x1 = atof(argv[1]);
  double x2 = atof(argv[2]);
  double y = atof(argv[3]);
  double width = atof(argv[4]);
  
  fprintf(svg_fp, "<!--  START insert for simple_pipe (%03d) -->\n", n_instance);
  fprintf(svg_fp, 
   "<rect fill=\"url(#grLinearH)\" x=\"%lf\" y=\"%lf\" width=\"%lf\" height=\"%lf\"/>\n",
                      x1, y, x2-x1, width);
  fprintf(svg_fp, "<!--  END insert for simple_pipe (%03d) -->\n", n_instance);
  add_svg_library("gray_gradients.svg");
  n_instance++;
}

