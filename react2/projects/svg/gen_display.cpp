
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include <dlfcn.h>

#include "gen_display.h"
#include "arg.h"

gen_object_base_t *get_plugin(const char *type);

/*********************************/

static int n_objs = 0;
static const char *js_objs[200];
static const char *tags[200];
static int n_libs = 0;
static const char *lib_files[200];

void add_library(const char *file_name)
{
  for (int i=0; i < n_libs; i++)
  {
    if (strcmp(lib_files[i], file_name))
    {
      return;
    }
  }
  lib_files[n_libs] = strdup(file_name);
  n_libs++;
}

void add_update_object(const char *the_tag, const char *the_js_object)
{
  //printf("Adding, tag: %s, object: %s\n", the_tag, the_js_object);
  tags[n_objs] = strdup(the_tag);  
  js_objs[n_objs] = strdup(the_js_object);
  n_objs++;
}

void gen_calls(FILE *js_fp)
{
  fprintf(js_fp, "var react_update_hrf = \"http://\" + location.hostname + \"/helloworld/tag?");
  for (int i=0; i < n_objs; i++)
  {
    fprintf(js_fp, "%s%s", i==0?"":"+", tags[i]);
  }
  fprintf(js_fp, "\";\n"); 

  fprintf(js_fp, "var update_objs = new Array(");
  for (int i=0; i < n_objs; i++)
  {
    fprintf(js_fp, "%s%s", i==0?"":",", js_objs[i]);
  }
  fprintf(js_fp, ");\n"); 


  fprintf(js_fp, "var sim_val = 20;\n");
  fprintf(js_fp, "function on_update_response()\n");
  fprintf(js_fp, "{\n");
  fprintf(js_fp, "  if (update_xReq.readyState != 4)  { return; }\n");
  fprintf(js_fp, "  var val = JSON.parse(update_xReq.responseText);\n");
  fprintf(js_fp, "  console.log(\"response: \" + update_xReq.responseText);\n");
  fprintf(js_fp, "  for (var i=0; i < update_objs.length; i++)\n");
  fprintf(js_fp, "  {\n");
  fprintf(js_fp, "    //update_objs[i].update(val[i]);\n");
  fprintf(js_fp, "    console.log(\"update_objs[\" + i + \"] = \" + update_objs[i]);\n");
  fprintf(js_fp, "    update_objs[i].update(sim_val + (i * 5));\n");
  fprintf(js_fp, "    sim_val++;\n");
  fprintf(js_fp, "    if (sim_val==100)\n");
  fprintf(js_fp, "    {\n");
  fprintf(js_fp, "      sim_val=0\n");
  fprintf(js_fp, "    }\n");
           
  fprintf(js_fp, "  }\n");
  fprintf(js_fp, "  update_xReq.abort(); // set the ready state back to 0\n");
  fprintf(js_fp, "}\n");
  fprintf(js_fp, "\n");

  fprintf(js_fp, "function intervalHandler()\n");
  fprintf(js_fp, "{\n");
  fprintf(js_fp, "  if (update_xReq.readyState != 0)  { return; }\n");
  fprintf(js_fp, "  update_xReq.open(\"GET\", react_update_hrf, true);\n");
  fprintf(js_fp, "  update_xReq.send(null);\n");
  fprintf(js_fp, "}\n");
  fprintf(js_fp, "\n");

  fprintf(js_fp, "function load()\n");
  fprintf(js_fp, "{\n");
  fprintf(js_fp, "  update_xReq = new XMLHttpRequest();\n");
  fprintf(js_fp, "  update_xReq.onreadystatechange = on_update_response;\n");
  fprintf(js_fp, "  var interval = setInterval(\"intervalHandler()\", 250);\n");
  fprintf(js_fp, "};\n");
  fprintf(js_fp, "// New generator!!\n");
  fprintf(js_fp, "\n");
}

void do_gen(const char *fname)
{
  const char *svg_file = "display.svg";
  FILE *svg_fp = fopen(svg_file, "w");
  if (svg_fp == NULL)
  {
    perror(svg_file);
    exit(-1);
  }
  const char *js_file = "display.js";
  FILE *js_fp = fopen(js_file, "w");
  if (svg_fp == NULL)
  {
    perror(js_file);
    exit(-1);
  }

  FILE *fp = fopen(fname, "r");
  if (fp == NULL)
  {
    perror(fname);
    exit(-1);
  }

  delim_file_t df(500, 50, '|', '#');
  df.print_lines(true);
  char **argv;
  int argc;
  int line_num;
  gen_object_base_t *obj;

  for (argv = df.first(fname, &argc, &line_num);
         (argv != NULL);
            argv = df.next(&argc, &line_num))
  {
    if (argc < 2)
    {
      printf("Wrong number of args, line %d\n", line_num);
      continue;
    }
    printf("%d: %s", line_num, argv[0]);
    for (int i=1; i < argc; i++)
    {
      printf(", %s", argv[i]);
    }
    obj = get_plugin(argv[0]);    
    if (obj == NULL)
    {
      printf("No such object type: %s\n", argv[0]);
      exit(-1);
    }
    printf("    Generating %s . . . .\n", argv[0]);
    obj->generate(svg_fp, js_fp, argc, argv);
  }

  gen_calls(js_fp);

/**
  gen_object_base_t *obj = new simple_rect_t;

  const char *params[] = {"level", "blue", "10", "20", "100"};
  obj->generate(svg_fp, js_fp, 5, params);
  const char *params2[] = {"pump1_amps", "red", "80", "10", "110"};
  obj->generate(svg_fp, js_fp, 5, params2);
  const char *params3[] = {"pump2_amps", "green", "110", "30", "50"};
  obj->generate(svg_fp, js_fp, 5, params3);
**/

}

/*********************************/

int n_plugins = 0;
gen_object_base_t *mi_objs[100];

gen_object_base_t *get_plugin(const char *type)
{
  for (int i=0; i < n_plugins; i++)
  {
    if (0 == strcmp(mi_objs[i]->get_name(), type)) return mi_objs[i];
  }
  return NULL;
}


int main(int argc, char *argv[])
{
  DIR *dir;
  struct dirent *dent;
  void *handle;
  char *error;
  //mi_base_t *mi_objs[100];
  crear_objeto_t cofn;
  int i;

  char sofile[100];

  if (argc < 2)
  {
    printf("You must specify the directory\n");
    exit(0);
  }

  const char *dir_name = argv[1]; 
  dir = opendir(dir_name);

  if (dir == NULL)
  {
    perror(dir_name);
    exit(0);
  }

  for (dent = readdir(dir); dent != NULL; dent = readdir(dir))
  {
    if (dent->d_type != DT_REG)
    {
      continue;
    }

    printf("File name = %s\n", dent->d_name);

    snprintf(sofile, sizeof(sofile), "%s/%s", dir_name, dent->d_name);

    printf("Opening plugin: %s\n", sofile);

    if (n_plugins > 99) 
    {
      printf("Maximum number of plugins exceeded!!!, stopping\n");
      break;
    }

    handle = dlopen(sofile, RTLD_LAZY);
    if (handle == NULL)
    {
      fprintf (stderr, "%s\n", dlerror());
      continue;
    }
    cofn = (crear_objeto_t) dlsym(handle, "get_object");
    if ((error = dlerror()) != NULL)
    {
      fprintf (stderr, "%s\n", error);
      continue;
    }
    mi_objs[n_plugins] = (*cofn)();
    n_plugins++;

  }
  printf ("found %d plugins!!!\n", n_plugins);

  // Ok, probamos todos de los objetos para ver que si funcionan.
  for (i=0; i < n_plugins; i++)
  {
    printf("Handles objects of type: %s\n\n", mi_objs[i]->get_name());
    //convertir_uno(mi_objs[i], "Teacher, esto es una prueba MUY MUY MUY tonta");
  }
  printf("\n\n");

  do_gen("display.txt");

  // Ok, borrar todos de los objetos para ver que funciona los destructores.
  for (i=0; i < n_plugins; i++)
  {
    delete(mi_objs[i]);
  }
  printf("\n\n");

  return 0;
}

/*********************************/

