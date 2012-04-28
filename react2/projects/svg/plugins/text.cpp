
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "gen_display.h"

static int n_instance = 1;

class text_t : public gen_plugin_base_t
{
public:
  const char *get_name(void); 
  void generate(FILE *svg_fp, FILE *svg_top_of_file_fp, FILE *js_fp, int argc, char **argv);
};

extern "C" gen_plugin_base_t *get_object(void)
{
  return new text_t;
}

const char *text_t::get_name(void)
{
  return "text";
}

void text_t::generate(FILE *svg_fp, FILE *svg_top_of_file_fp, FILE *js_fp, int argc, char **argv)
{
  double x = atof(argv[1]);
  double y = atof(argv[2]);
  double height = atof(argv[3]);
  const char *color = argv[4];
  const char *anchor = argv[5];

//x|y|height|color|anchor|line1|line2|. . . |lineN
  
  fprintf(svg_fp, "<!--  START insert for text (%03d) -->\n", n_instance);
  for (int i=6; i < argc; i++)
  {
    const char *the_text = argv[i];
    fprintf(svg_fp, "<text x=\"%lf\" y=\"%lf\" font-family=\"Verdana\" font-size=\"%lf\" fill=\"%s\" text-anchor=\"%s\">%s</text>\n",
                     x, y, height, color, anchor, the_text); 
    y += height * 1.05; 
  }
  fprintf(svg_fp, "<!--  END insert for text (%03d) -->\n", n_instance);
  n_instance++;
}

