
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "gen_display.h"

static int n_instance = 1;

class svg_header_t : public gen_plugin_base_t
{
public:
  const char *get_name(void); 
  void generate(FILE *svg_fp, FILE *svg_top_of_file_fp, FILE *js_fp, int argc, char **argv);
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

void svg_header_t::generate(FILE *svg_fp, FILE *svg_top_of_file_fp, FILE *js_fp, int argc, char **argv)
{
  const char *title = argv[1];
  const char *x1 = argv[2];
  const char *y1 = argv[3];
  const char *x2 = argv[4];
  const char *y2 = argv[5];

  fprintf(svg_top_of_file_fp, "<!--  START insert svg header -->\n");

  fprintf(svg_top_of_file_fp, "<svg xmlns=\"http://www.w3.org/2000/svg\"\n");
  fprintf(svg_top_of_file_fp, "     xmlns:xlink=\"http://www.w3.org/1999/xlink\"\n");
  fprintf(svg_top_of_file_fp, "     width=\"100%%\" height=\"100%%\" viewBox=\"%s %s %s %s\" onload=\"load()\">\n",
          x1, y1, x2, y2);
  fprintf(svg_top_of_file_fp, "\n");
  fprintf(svg_top_of_file_fp, "  <title>%s</title>\n", title);
  fprintf(svg_top_of_file_fp, "  <metadata>author: Don Carr</metadata>\n");
 
  fprintf(svg_top_of_file_fp, "<!--  END insert svg header -->\n");
  n_instance++;
}

