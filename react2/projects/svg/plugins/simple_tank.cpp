
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "gen_display.h"


/******************************************/

void gen_cone_tank(FILE *fp, double x, double y, double width, double height, double cap_height, 
              double cone_height, double cone_width)
{
  fprintf(fp, "  <g stroke=\"#000000\" stroke-width=\"0\">\n");
  fprintf(fp, "    <rect fill=\"url(#grLinearV)\"\n");
  fprintf(fp, "      x=\"%f\" y=\"%f\" width=\"%f\" height=\"%f\"/>\n", 
                          x, y + cap_height, width, height - cone_height - cap_height);
  fprintf(fp, "    <path fill=\"url(#grRadial)\"\n");
  fprintf(fp, "      d=\"M%f,%f A%f,%f 0 1,1 %f,%f z M%f,%f L%f,%f\"/>\n",
                     //d="M0,50 A125,50 0 1,1 250,50 M250,100 L250,100"/>
                          x, y + cap_height, 
                          width / 2.0, cap_height, 
                          x + width, y + cap_height,
                          x, y + (cap_height * 2.0),
                          x, y + (cap_height * 2.0)); 
  fprintf(fp, "    <path fill=\"url(#grRadial)\"\n");
  fprintf(fp, "d=\"M%f,%f h%f l%f,%f h%f z m 0 %f h 0\"/>\n",
                     //d="M0,400 h200 l-50,100 h-100 z "/>
                          x, y + height - cone_height,
                          width, 
                          (width - cone_width)/-2.0, cone_height, 
                          cone_width * -1.0,
                          cone_height * -1.0);
  fprintf(fp, "  </g>\n");
}

/******************************************/

void gen_pressure_tank(FILE *fp, double x, double y, double width, double height, double top_height)
{
  fprintf(fp, "  <g stroke=\"#000000\" stroke-width=\"0\">\n");
  fprintf(fp, "    <rect fill=\"url(#grLinearV)\"\n");
  fprintf(fp, "      x=\"%f\" y=\"%f\" width=\"%f\" height=\"%f\"/>\n", 
                          x, y + top_height, width, height - (2 * top_height));
  fprintf(fp, "    <path fill=\"url(#grRadial)\"\n");
  fprintf(fp, "      d=\"M%f,%f A%f,%f 0 1,1 %f,%f z M%f,%f L%f,%f\"/>\n",
                     //d="M0,50 A125,50 0 1,1 250,50 M250,100 L250,100"/>
                          x, y + top_height, 
                          width / 2.0, top_height, 
                          x + width, y + top_height,
                          x, y + (top_height * 2.0),
                          x, y + (top_height * 2.0)); 
  fprintf(fp, "    <path fill=\"url(#grRadial)\"\n");
  fprintf(fp, "      d=\"M%f,%f A%f,%f 0 1,0 %f,%f z M%f,%f L%f,%f\"/>\n",
                      //d="M0,450 A125,50 0 1,0 250,450 M250,400 L250,400"/>
                          x, y + height - (top_height), 
                          (width / 2.0), (top_height), 
                          x + width, y + height - (top_height),
                          x, y + height - (top_height * 2.0),
                          x, y + height - (top_height * 2.0)); 
  fprintf(fp, "\n  </g>\n");
}

/******************************************/

static int n_instance = 1;

class simple_tank_t : public gen_plugin_base_t
{
public:
  const char *get_name(void); 
  void generate(FILE *svg_fp, FILE *svg_top_of_file_fp, FILE *js_fp, int argc, char **argv);
};

/**********************************************/

extern "C" gen_plugin_base_t *get_object(void)
{
  return new simple_tank_t;
}

/**********************************************/

const char *simple_tank_t::get_name(void)
{
  return "simple_tank";
}

/**********************************************/

void simple_tank_t::generate(FILE *svg_fp, FILE *svg_top_of_file_fp, FILE *js_fp, int argc, char **argv)
{
  double x = 50;
  double y = 50;
  double width = 800;
  double height = 600; 
  double cap_height = 50;
  double cone_height = 200;
  double cone_width = 30;

  fprintf(svg_fp, "<!--  START insert for simple_tank (%03d) -->\n", n_instance);
  if ((argc == 9) && (argv[1][0] == 'c'))
  {
    // /gen_tank c 0 0 300 400 20 50 20
    x = atof(argv[2]);
    y = atof(argv[3]);
    width = atof(argv[4]);
    height = atof(argv[5]);
    cap_height = atof(argv[6]);
    cone_height = atof(argv[7]);
    cone_width = atof(argv[8]);
    printf("cone tank:\n");
    printf("x = %lf\n", x);
    printf("y = %lf\n", y);
    printf("width = %lf\n", width);
    printf("height = %lf\n", height);
    printf("cap_height = %lf\n", cap_height);
    printf("cone_height = %lf\n", cone_height);
    printf("cone_width = %lf\n", cone_width);
    gen_cone_tank(svg_fp, x, y, width, height, cap_height, cone_height, cone_width);
  }
  else if ((argc == 7) && (argv[1][0] == 'p'))
  {
    // /gen_tank p 0 0 300 400 20 
    x = atof(argv[2]);
    y = atof(argv[3]);
    width = atof(argv[4]);
    height = atof(argv[5]);
    cap_height = atof(argv[6]);
    printf("pressure tank:\n");
    printf("x = %lf\n", x);
    printf("y = %lf\n", y);
    printf("width = %lf\n", width);
    printf("height = %lf\n", height);
    printf("cap_height = %lf\n", cap_height);
    gen_pressure_tank(svg_fp, x, y, width, height, cap_height);
  }
  else
  {
    printf("Incorrect arguments for tank, examples:\n");
    printf("    |simple_tank|c|0|0|300|400|20|50|20|\n");
    printf("    |simple_tank|p|0|0|300|400|20|\n");
  }
  fprintf(svg_fp, "<!--  END insert for simple_tank (%03d) -->\n", n_instance);
  add_svg_library("gray_gradients.svg");
  n_instance++;
}
