
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "gen_display.h"

static int n_instance = 1;

class triangle_t : public gen_plugin_base_t
{
public:
  const char *get_name(void); 
  void generate(FILE *svg_fp, FILE *svg_top_of_file_fp, FILE *js_fp, int argc, char **argv);
};

extern "C" gen_plugin_base_t *get_object(void)
{
  return new triangle_t;
}

const char *triangle_t::get_name(void)
{
  return "triangle";
}

void triangle_t::generate(FILE *svg_fp, FILE *svg_top_of_file_fp, FILE *js_fp, int argc, char **argv)
{
  double cx = atof(argv[1]);
  double cy = atof(argv[2]);
  double width = atof(argv[3]);
  const char *color = argv[4];
  int angle = atoi(argv[5]);
  double height = width * 1.2;

  // triangle|cx|cy|width|color|angle|
  
  char str[200];
  if (angle == 0) str[0] = '\0';
  else snprintf(str, sizeof(str), "transform=\"rotate(%d %lf,%lf)\"", angle, cx, cy);
  printf("Tringle: %s\n", str);


  fprintf(svg_fp, "<!--  START insert for triangle (%03d) -->\n", n_instance);

  fprintf(svg_fp, "<polygon points=\"%lf,%lf %lf,%lf %lf,%lf \" stroke=\"black\" stroke-width=\"%lf\" fill=\"%s\" %s/>\n",
             cx - (width / 2.0), cy, cx + (width / 2.0), cy, cx, cy - height, 0.05 * width, color, str);

  fprintf(svg_fp, "<!--  END insert for triangle (%03d) -->\n", n_instance);
  n_instance++;
}

