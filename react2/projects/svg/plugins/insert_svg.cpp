
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "gen_display.h"

static int n_instance = 1;

class insert_svg_t : public gen_plugin_base_t
{
public:
  const char *get_name(void); 
  void generate(plugin_data_t d, int argc, char **argv);
};

extern "C" gen_plugin_base_t *get_object(void)
{
  return new insert_svg_t;
}

const char *insert_svg_t::get_name(void)
{
  return "insert_svg";
}

void insert_svg_t::generate(plugin_data_t d, int argc, char **argv)
{
  const char *the_svg = argv[1];
  
  fprintf(d.svg_fp, "<!--  START insert for insert_svg (%03d) -->\n", n_instance);
  fprintf(d.svg_fp, "%s\n", the_svg); 
  fprintf(d.svg_fp, "<!--  END insert for insert_svg (%03d) -->\n", n_instance);
  n_instance++;
}

