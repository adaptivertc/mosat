
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "gen_display.h"

static int n_instance = 1;

class background_t : public gen_plugin_base_t
{
public:
  const char *get_name(void); 
  void generate(plugin_data_t d, int argc, char **argv);
};

extern "C" gen_plugin_base_t *get_object(void)
{
  return new background_t;
}

const char *background_t::get_name(void)
{
  return "background";
}

void background_t::generate(plugin_data_t d, int argc, char **argv)
{
  const char *color = argv[1];
  fprintf(d.svg_top_of_file_fp, "<!--  START insert background color -->\n");
 
  fprintf(d.svg_top_of_file_fp, "<g id=\"main_group\">\n");
  fprintf(d.svg_top_of_file_fp, "<rect fill=\"%s\" x=\"0%%\" y=\"0%%\" width=\"100%%\" height=\"100%%\" stroke=\"none\" stroke-width=\"0\"/>\n", color);
  fprintf(d.svg_top_of_file_fp, "<!--  END insert background color -->\n");
  n_instance++;
}

