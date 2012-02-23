
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "gen_display.h"


/*******************************************/

void gen_elbo_ru(FILE *fp, double x, double y, double pipe_width, double elbo_width)
{
  double small_width = elbo_width - pipe_width;
  fprintf(fp, "<path fill=\"url(#grRadial)\" d=\"M%lf,%lf a%lf,%lf 0 0,0 %lf,%lf h%lf a%lf,%lf 0 0,1 %lf,%lf z\"/>",
          /* M */ x, y + (pipe_width/2), 
          /* a */  elbo_width, elbo_width, elbo_width, -elbo_width,
          /* h */  -pipe_width,
          /* a */  small_width, small_width, -small_width, small_width);
}

/*******************************************/

void gen_elbo_rd(FILE *fp, double x, double y, double pipe_width, double elbo_width)
{
  double small_width = elbo_width - pipe_width;
  fprintf(fp, "<path fill=\"url(#grRadial)\" d=\"M%lf,%lf a%lf,%lf 0 0,1 %lf,%lf h%lf a%lf,%lf 0 0,0 %lf,%lf z\"/>",
          /* M */ x, y - (pipe_width/2), 
          /* a */  elbo_width, elbo_width, elbo_width, elbo_width,
          /* h */  -pipe_width,
          /* a */  small_width, small_width, -small_width, -small_width);
}

/*******************************************/

void gen_elbo_lu(FILE *fp, double x, double y, double pipe_width, double elbo_width)
{
  double small_width = elbo_width - pipe_width;
  fprintf(fp, "<path fill=\"url(#grRadial)\" d=\"M%lf,%lf a%lf,%lf 0 0,1 %lf,%lf h%lf a%lf,%lf 0 0,0 %lf,%lf z\"/>",
          /* M */ x, y + (pipe_width/2), 
          /* a */  elbo_width, elbo_width, -elbo_width, -elbo_width,
          /* h */  pipe_width,
          /* a */  small_width, small_width, small_width, small_width);
}

/*******************************************/

void gen_elbo_ld(FILE *fp, double x, double y, double pipe_width, double elbo_width)
{
  double small_width = elbo_width - pipe_width;
  fprintf(fp, "<path fill=\"url(#grRadial)\" d=\"M%lf,%lf a%lf,%lf 0 0,0 %lf,%lf h%lf a%lf,%lf 0 0,1 %lf,%lf z\"/>",
          /* M */ x, y - (pipe_width/2), 
          /* a */  elbo_width, elbo_width, -elbo_width, elbo_width,
          /* h */  pipe_width,
          /* a */  small_width, small_width, small_width, -small_width);
}

/*******************************************/

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
                      x1, y - (width / 2.0), x2-x1, width);
  gen_elbo_ru(svg_fp, x2, y, width, width * 1.3);
  gen_elbo_rd(svg_fp, x2, y-(width * 1.6) , width, width * 1.3);
  gen_elbo_ld(svg_fp, x1, y, width, width * 1.3);
  gen_elbo_lu(svg_fp, x1, y+(width*1.6), width, width * 1.3);
  fprintf(svg_fp, "<!--  END insert for simple_pipe (%03d) -->\n", n_instance);
  add_svg_library("gray_gradients.svg");
  n_instance++;
}

