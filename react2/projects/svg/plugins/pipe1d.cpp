
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "gen_display.h"

/*******************************************/

void gen_elbo_ru(FILE *fp, double x, double y, double pipe_width, double elbo_width)
{
  double small_width = elbo_width - pipe_width;
  fprintf(fp, "<path d=\"M%lf,%lf a%lf,%lf 0 0,0 %lf,%lf h%lf a%lf,%lf 0 0,1 %lf,%lf z\"/>\n",
          /* M */ x, y + (pipe_width/2), 
          /* a */  elbo_width, elbo_width, elbo_width, -elbo_width,
          /* h */  -pipe_width,
          /* a */  small_width, small_width, -small_width, small_width);
}

/*******************************************/

void gen_elbo_rd(FILE *fp, double x, double y, double pipe_width, double elbo_width)
{
  double small_width = elbo_width - pipe_width;
  fprintf(fp, "<path d=\"M%lf,%lf a%lf,%lf 0 0,1 %lf,%lf h%lf a%lf,%lf 0 0,0 %lf,%lf z\"/>\n",
          /* M */ x, y - (pipe_width/2), 
          /* a */  elbo_width, elbo_width, elbo_width, elbo_width,
          /* h */  -pipe_width,
          /* a */  small_width, small_width, -small_width, -small_width);
}

/*******************************************/

void gen_elbo_lu(FILE *fp, double x, double y, double pipe_width, double elbo_width)
{
  double small_width = elbo_width - pipe_width;
  fprintf(fp, "<path d=\"M%lf,%lf a%lf,%lf 0 0,1 %lf,%lf h%lf a%lf,%lf 0 0,0 %lf,%lf z\"/>\n",
          /* M */ x, y + (pipe_width/2), 
          /* a */  elbo_width, elbo_width, -elbo_width, -elbo_width,
          /* h */  pipe_width,
          /* a */  small_width, small_width, small_width, small_width);
}

/*******************************************/

void gen_elbo_ld(FILE *fp, double x, double y, double pipe_width, double elbo_width)
{
  double small_width = elbo_width - pipe_width;
  fprintf(fp, "<path d=\"M%lf,%lf a%lf,%lf 0 0,0 %lf,%lf h%lf a%lf,%lf 0 0,1 %lf,%lf z\"/>\n",
          /* M */ x, y - (pipe_width/2), 
          /* a */  elbo_width, elbo_width, -elbo_width, elbo_width,
          /* h */  pipe_width,
          /* a */  small_width, small_width, small_width, -small_width);
}
  
/*******************************************/
static bool last_HR = true;
static bool last_VU = true;
static bool last_V = true;
static bool first = true;

void gen_butt_end(FILE *fp, double width, double butt_width, double x2, double y2)
{
  printf("butt end - width: %lf, bwidth: %lf, x2: %lf, y2: %lf\n",
         width, butt_width, x2, y2);
  if (last_V && last_VU)
  {
    // draw the butt end up
    printf("butt up\n");
    fprintf(fp, 
    "<polygon points=\"%lf,%lf %lf,%lf %lf,%lf\"/>\n",
          x2 - (width/2.0), y2, x2 + (width/2.0), y2, x2, y2 - (butt_width/2.0));
  }
  else if (last_V && !last_VU)
  {
    // draw the butt end down 
    printf("butt down\n");
    fprintf(fp, 
    "<polygon points=\"%lf,%lf %lf,%lf %lf,%lf\"/>\n",
          x2 - (width/2.0), y2, x2 + (width/2.0), y2, x2, y2 + (butt_width/2.0));
  }
  else if (!last_V && last_HR)
  {
    // draw the butt end right 
    printf("butt right\n");
    fprintf(fp, 
    "<polygon points=\"%lf,%lf %lf,%lf %lf,%lf\"/>\n",
          x2, y2 - (width/2.0), x2, y2 + (width/2.0), x2 + (butt_width/2.0), y2);
  }
  else if (!last_V && !last_HR)
  {
    // draw the butt end left 
    printf("butt left\n");
    fprintf(fp, 
    "<polygon points=\"%lf,%lf %lf,%lf %lf,%lf\"/>\n",
          x2, y2 - (width/2.0), x2, y2 + (width/2.0), x2 - (butt_width/2.0), y2);
  }
  
}

/*******************************************/

void gen_v_pipe(FILE *fp, double x, double y1, double y2, double width) 
{
  double x1 = x - (width / 2.0);
  double x2 = x + (width / 2.0);

  
  last_V = true;
  if(y1 > y2)
  {
    double tmp;
    tmp = y1;
    y1 = y2;
    y2 = tmp; 
    last_VU = true;
  } 
  else
  {
    last_VU = false;
  }

  printf("V - x1: %lf, y1: %lf, x2: %lf, y2: %lf\n", x1, y1, x2, y2);

  if (first)
  {
    first = false;
    fprintf(fp, 
     "<rect x=\"%lf\" y=\"%lf\" width=\"%lf\" height=\"%lf\"/>\n",
          x - (width/2.0), y1, width, y2-y1);
  }
  else if (last_HR && last_VU)
  {
    // case last was to the right, next is up 
    fprintf(fp, 
    "<polygon points=\"%lf,%lf %lf,%lf %lf,%lf %lf,%lf %lf,%lf\"/>\n",
          x, y2, x2, y2, x2, y1, x1, y1, x1, y2 - (width * 0.5));
  }
  else if (!last_HR && last_VU)
  {
    // case last was to the left, next is up 
    fprintf(fp, 
    "<polygon points=\"%lf,%lf %lf,%lf %lf,%lf %lf,%lf %lf,%lf\"/>\n",
          x, y2, x1, y2, x1, y1, x2, y1, x2, y2 - (width * 0.5));
  }
  else if (last_HR && !last_VU)
  {
    // case last was to the right, next is down
    fprintf(fp, 
    "<polygon points=\"%lf,%lf %lf,%lf %lf,%lf %lf,%lf %lf,%lf\"/>\n",
          x, y1, x2, y1, x2, y2, x1, y2, x1, y1 + (width * 0.5));
  }
  else if (!last_HR && !last_VU)
  {
    // case last was to the left, next is down
    fprintf(fp, 
    "<polygon points=\"%lf,%lf %lf,%lf %lf,%lf %lf,%lf %lf,%lf\"/>\n",
          x, y1, x1, y1, x1, y2, x2, y2, x2, y1 + (width * 0.5));
  }
}

/*******************************************/

void gen_h_pipe(FILE *fp, double x1, double y, double x2, double width)
{
  double y1 = y - (width / 2.0);
  double y2 = y + (width / 2.0);

  last_V = false;
  if(x1 > x2)
  {
    double tmp;
    tmp = x1;
    x1 = x2;
    x2 = tmp; 
    last_HR = false; 
  } 
  else
  {
    last_HR = true; 
  }
  printf("H - x1: %lf, y1: %lf, x2: %lf, y2: %lf\n", x1, y1, x2, y2);

  if (first)
  {
    first = false;
    fprintf(fp, 
     "<rect x=\"%lf\" y=\"%lf\" width=\"%lf\" height=\"%lf\"/>\n",
          x1, y - (width / 2.0), x2-x1, width);
  }
  else if (last_HR && last_VU)
  {
    // case last was up, next is right 
    fprintf(fp, 
   "<polygon points=\"%lf,%lf %lf,%lf %lf,%lf %lf,%lf %lf,%lf\"/>\n",
          x1, y1, x2 , y1, x2, y2, x1 + (width * 0.5), y2, x1, y);
  }
  else if (!last_HR && last_VU)
  {
    // case last was up, next is left 
    fprintf(fp, 
   "<polygon points=\"%lf,%lf %lf,%lf %lf,%lf %lf,%lf %lf,%lf\"/>\n",
          x2, y1, x1 , y1, x1, y2, x2 - (width * 0.5), y2, x2, y);
  }
  else if (last_HR && !last_VU)
  {
    // case last was down, next is right 
    fprintf(fp, 
    "<polygon points=\"%lf,%lf %lf,%lf %lf,%lf %lf,%lf %lf,%lf\"/>\n",
          x1, y2, x2 , y2, x2, y1, x1 + (width * 0.5), y1, x1, y);
  }
  else if (!last_HR && !last_VU)
  {
    // case last was down, next is left 
    fprintf(fp, 
    "<polygon points=\"%lf,%lf %lf,%lf %lf,%lf %lf,%lf %lf,%lf\"/>\n",
          x2, y2, x1 , y2, x1, y1, x2 - (width * 0.5), y1, x2, y);
  }
}

/*******************************************/

static int n_instance = 1;

class pipe1d_t : public gen_plugin_base_t
{
public:
  const char *get_name(void); 
  void generate(plugin_data_t d, int argc, char **argv);
};

/*******************************************/

extern "C" gen_plugin_base_t *get_object(void)
{
  return new pipe1d_t;
}

/*******************************************/

const char *pipe1d_t::get_name(void)
{
  return "pipe1d";
}

/*******************************************/

void pipe1d_t::generate(plugin_data_t d, int argc, char **argv)
{
  const char *the_tag = argv[1];
  const char *on_color = argv[2];
  const char *off_color = argv[3];
  double width = atof(argv[4]);
  double x1 = atof(argv[5]);
  double y1 = atof(argv[6]);
  double x2;
  double y2;
  double butt_width;

  printf("Pipe color is: %s\n", on_color);
  printf("Tag is: %s\n", the_tag);
  first = true;

  fprintf(d.svg_fp, "<!-- START insert for pipe1d (%03d) -->\n", n_instance);
/***/
  char js_object_name[30];
  char js_group_name[30];
  snprintf(js_object_name, sizeof(js_object_name), "pipe1d_obj_%03d", n_instance);
  snprintf(js_group_name, sizeof(js_group_name), "pipe1d_group_obj_%03d", n_instance);
  fprintf(d.svg_fp, "<g id=\"%s\" fill=\"%s\" stroke=\"black\" stroke-width=\"%lf\">\n",
       js_group_name, on_color, 0.1 * width);
/***/
  for (int i=7; i < (argc-1); i+=2)
  {
    switch (argv[i][0])
    {
      case 'h':
      case 'H':
        x2 = atof(argv[i+1]);
        printf("Making Horizontal section to %lf\n", x2);
        y2 = y1;
        if (((i+2) < (argc-1)) && (argv[i+2][0] != 'b') && (argv[i+2][0] != 'B'))
        {
          fprintf(d.svg_fp,  
         "<circle cx=\"%lf\" cy=\"%lf\" r=\"%lf\" />\n",
              x2, y2, width / 2.0);
        }
        gen_h_pipe(d.svg_fp, x1, y1,  x2, width);
        break;
      case 'v':
      case 'V':
        y2 = atof(argv[i+1]);
        printf("Making Vertical section to %lf\n", y2);
        x2 = x1;
        if (((i+2) < (argc-1)) && (argv[i+2][0] != 'b') && (argv[i+2][0] != 'B'))
        {
          fprintf(d.svg_fp,  
         "<circle cx=\"%lf\" cy=\"%lf\" r=\"%lf\" />\n",
              x2, y2, width / 2.0);
        }
        gen_v_pipe(d.svg_fp, x1, y1, y2, width);
        break;
      case 'b':
      case 'B':
        printf("Making piece to butt against a pipe %lf, %lf\n", x2, y2);
        butt_width = atof(argv[i+1]);
        gen_butt_end(d.svg_fp, width, butt_width, x2, y2);
        break;
      default:
        printf("Error: %s, must be either 'h' or 'v' or 'b' for the direction\n", argv[i]);
        break;
    } 
    x1 = x2;
    y1 = y2;
  }
  fprintf(d.svg_fp, "</g>\n");
  fprintf(d.js_fp, "var %s = new pump_t(\"%s\", \"%s\", \"%s\");\n",
                       js_object_name, js_group_name, on_color, off_color);


  fprintf(d.svg_fp, "<!--  END insert for pipe1d (%03d) -->\n", n_instance);
  if ((strlen(the_tag) > 0) && (0 != strcmp(the_tag, "null")))
  {
    add_js_library("pump.js");
    add_animation_object(the_tag, js_object_name);
  }

  n_instance++;
}

/*******************************************/

