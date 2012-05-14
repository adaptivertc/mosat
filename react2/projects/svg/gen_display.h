
struct plugin_data_t
{
  FILE *svg_fp;
  FILE *js_fp;
  //FILE *svg_after_header_fp;
  FILE *svg_top_of_file_fp;
};

class gen_plugin_base_t
{
public:
  virtual const char *get_name(void)=0;
  virtual void generate(plugin_data_t d, int argc, char **argv)=0;
};

void add_js_library(const char *file_name);
void add_svg_library(const char *file_name);
void add_animation_object(const char *the_tag, const char *the_js_object);
void set_viewbox(double x1, double y1, double x2, double y2);
void find_a_place_nearby(double *x,double *y,
                         double screen_width, double screen_height,
                         double item_x, double item_y,
                         double item_width, double item_height,
                         double popup_width, double popup_height);
void find_a_place_nearby(double *x,double *y,
                         double item_x, double item_y,
                         double item_width, double item_height);



typedef gen_plugin_base_t * (*crear_objeto_t)(void);



