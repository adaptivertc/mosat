
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "gen_display.h"

static int n_instance = 1;

class svg_header_t : public gen_plugin_base_t
{
public:
  const char *get_name(void); 
  void generate(plugin_data_t d, int argc, char **argv);
};

extern "C" gen_plugin_base_t *get_object(void)
{
  return new svg_header_t;
}

const char *svg_header_t::get_name(void)
{
  return "svg_header";
}

/*********************************/

void svg_header_t::generate(plugin_data_t d, int argc, char **argv)
{
  const char *title = argv[1];
  const char *x1 = argv[2];
  const char *y1 = argv[3];
  const char *x2 = argv[4];
  const char *y2 = argv[5];

  fprintf(d.svg_top_of_file_fp, "<!--  START insert svg header -->\n");

  fprintf(d.svg_top_of_file_fp, "<svg xmlns=\"http://www.w3.org/2000/svg\"\n");
  fprintf(d.svg_top_of_file_fp, "     xmlns:xlink=\"http://www.w3.org/1999/xlink\"\n");
  fprintf(d.svg_top_of_file_fp, "     width=\"100%%\" height=\"100%%\" viewBox=\"%s %s %s %s\" onload=\"load()\">\n",
          x1, y1, x2, y2);
  fprintf(d.svg_top_of_file_fp, "\n");
  fprintf(d.svg_top_of_file_fp, "  <title>%s</title>\n", title);
  fprintf(d.svg_top_of_file_fp, "  <metadata>author: Don Carr</metadata>\n");
 
  fprintf(d.svg_top_of_file_fp, "<!--  END insert svg header -->\n");
  n_instance++;
}

