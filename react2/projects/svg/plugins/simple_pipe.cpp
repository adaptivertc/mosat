
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "gen_display.h"


/*******************************************/

void gen_elbo_ru(FILE *fp, double x, double y, double pipe_width, double elbo_width)
{
  double small_width = elbo_width - pipe_width;
  fprintf(fp, "<path fill=\"url(#grRadial)\" d=\"M%lf,%lf a%lf,%lf 0 0,0 %lf,%lf h%lf a%lf,%lf 0 0,1 %lf,%lf z\"/>\n",
          /* M */ x, y + (pipe_width/2), 
          /* a */  elbo_width, elbo_width, elbo_width, -elbo_width,
          /* h */  -pipe_width,
          /* a */  small_width, small_width, -small_width, small_width);
}

/*******************************************/

void gen_elbo_rd(FILE *fp, double x, double y, double pipe_width, double elbo_width)
{
  double small_width = elbo_width - pipe_width;
  fprintf(fp, "<path fill=\"url(#grRadial)\" d=\"M%lf,%lf a%lf,%lf 0 0,1 %lf,%lf h%lf a%lf,%lf 0 0,0 %lf,%lf z\"/>\n",
          /* M */ x, y - (pipe_width/2), 
          /* a */  elbo_width, elbo_width, elbo_width, elbo_width,
          /* h */  -pipe_width,
          /* a */  small_width, small_width, -small_width, -small_width);
}

/*******************************************/

void gen_elbo_lu(FILE *fp, double x, double y, double pipe_width, double elbo_width)
{
  double small_width = elbo_width - pipe_width;
  fprintf(fp, "<path fill=\"url(#grRadial)\" d=\"M%lf,%lf a%lf,%lf 0 0,1 %lf,%lf h%lf a%lf,%lf 0 0,0 %lf,%lf z\"/>\n",
          /* M */ x, y + (pipe_width/2), 
          /* a */  elbo_width, elbo_width, -elbo_width, -elbo_width,
          /* h */  pipe_width,
          /* a */  small_width, small_width, small_width, small_width);
}

/*******************************************/

void gen_elbo_ld(FILE *fp, double x, double y, double pipe_width, double elbo_width)
{
  double small_width = elbo_width - pipe_width;
  fprintf(fp, "<path fill=\"url(#grRadial)\" d=\"M%lf,%lf a%lf,%lf 0 0,0 %lf,%lf h%lf a%lf,%lf 0 0,1 %lf,%lf z\"/>\n",
          /* M */ x, y - (pipe_width/2), 
          /* a */  elbo_width, elbo_width, -elbo_width, elbo_width,
          /* h */  pipe_width,
          /* a */  small_width, small_width, small_width, -small_width);
}
  
/*******************************************/

void gen_v_pipe(FILE *fp, double x, double y1, double y2, double width) {
  if(y1 > y2)
  {
     double tmp;
     tmp = y1;
     y1 = y2;
     y2 = tmp; 
  } 
  fprintf(fp, 
   "<rect fill=\"url(#grLinearV)\" x=\"%lf\" y=\"%lf\" width=\"%lf\" height=\"%lf\"/>\n",
          x - (width/2.0), y1, width, y2-y1);
}

/*******************************************/

void gen_h_pipe(FILE *fp, double x1, double y, double x2, double width)
{
  if(x1 > x2)
  {
     double tmp;
     tmp = x1;
     x1 = x2;
     x2 = tmp; 
  } 
  fprintf(fp, 
   "<rect fill=\"url(#grLinearH)\" x=\"%lf\" y=\"%lf\" width=\"%lf\" height=\"%lf\"/>\n",
          x1, y - (width / 2.0), x2-x1, width);
}

/*******************************************/

static int n_instance = 1;

class simple_pipe_t : public gen_plugin_base_t
{
public:
  const char *get_name(void); 
  void generate(FILE *svg_fp, FILE *svg_top_of_file_fp, FILE *js_fp, int argc, char **argv);
};

/*******************************************/

extern "C" gen_plugin_base_t *get_object(void)
{
  return new simple_pipe_t;
}

/*******************************************/

const char *simple_pipe_t::get_name(void)
{
  return "simple_pipe";
}

/*******************************************/

void simple_pipe_t::generate(FILE *svg_fp, FILE *svg_top_of_file_fp, FILE *js_fp, int argc, char **argv)
{
  double width = atof(argv[1]);
  double x1 = atof(argv[2]);
  double y1 = atof(argv[3]);
  double x2;
  double y2;

  fprintf(svg_fp, "<!-- START insert for simple_pipe (%03d) -->\n", n_instance);
  for (int i=4; i < (argc-1); i+=2)
  {
    switch (argv[i][0])
    {
      case 'h':
      case 'H':
        x2 = atof(argv[i+1]);
        y2 = y1;
        if ((i+2) < (argc-1))
          fprintf(svg_fp,  
         "<circle cx=\"%lf\" cy=\"%lf\" r=\"%lf\" fill=\"url(#grRadial)\"/>\n",
              x2, y2, width / 2.0);
        gen_h_pipe(svg_fp, x1, y1,  x2, width);
        break;
      case 'v':
      case 'V':
        y2 = atof(argv[i+1]);
        x2 = x1;
        if ((i+2) < (argc-1))
          fprintf(svg_fp,  
         "<circle cx=\"%lf\" cy=\"%lf\" r=\"%lf\" fill=\"url(#grRadial)\"/>\n",
              x2, y2, width / 2.0);
        gen_v_pipe(svg_fp, x1, y1, y2, width);
        break;
    } 
    x1 = x2;
    y1 = y2;
  }

  fprintf(svg_fp, "<!--  END insert for simple_pipe (%03d) -->\n", n_instance);
  add_svg_library("gray_gradients.svg");
  n_instance++;
}

/*******************************************/

