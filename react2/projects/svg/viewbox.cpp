
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "gen_display.h"

static int n_instance = 1;

class viewbox_t : public gen_object_base_t
{
public:
  const char *get_name(void); 
  void generate(FILE *svg_fp, FILE *svg_after_header_fp, FILE *js_fp, int argc, char **argv);
};

extern "C" gen_object_base_t *get_object(void)
{
  return new viewbox_t;
}

const char *viewbox_t::get_name(void)
{
  return "viewbox";
}

void viewbox_t::generate(FILE *svg_fp, FILE *svg_after_header_fp, FILE *js_fp, int argc, char **argv)
{
  double x1 = atof(argv[1]);
  double y1 = atof(argv[2]);
  double x2 = atof(argv[3]);
  double y2 = atof(argv[4]);
  set_viewbox(x1, y1, x2, y2);
  
  n_instance++;
}

