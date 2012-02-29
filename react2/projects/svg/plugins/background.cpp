
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "gen_display.h"

static int n_instance = 1;

class background_t : public gen_plugin_base_t
{
public:
  const char *get_name(void); 
  void generate(FILE *svg_fp, FILE *svg_top_of_file_fp, FILE *js_fp, int argc, char **argv);
};

extern "C" gen_plugin_base_t *get_object(void)
{
  return new background_t;
}

const char *background_t::get_name(void)
{
  return "background";
}

void background_t::generate(FILE *svg_fp, FILE *svg_top_of_file_fp, FILE *js_fp, int argc, char **argv)
{
  const char *color = argv[1];
  fprintf(svg_top_of_file_fp, "<!--  START insert background color -->\n");
 
  fprintf(svg_top_of_file_fp, "<rect fill=\"%s\" x=\"0%%\" y=\"0%%\" width=\"100%%\" height=\"100%%\" stroke=\"none\" stroke-width=\"0\"/>\n", color);
  fprintf(svg_top_of_file_fp, "<!--  END insert background color -->\n");
  n_instance++;
}

