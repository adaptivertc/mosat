

class gen_object_base_t
{
public:
  virtual const char *get_name(void)=0;
  virtual void generate(FILE *svg_fp, FILE *svg_after_header_fp, FILE *js_fp, 
                       int argc, char **argv)=0;
};

void add_js_library(const char *file_name);
void add_svg_library(const char *file_name);
void add_update_object(const char *the_tag, const char *the_js_object);

typedef gen_object_base_t * (*crear_objeto_t)(void);



