
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include <dlfcn.h>

#include "gen_display.h"
#include "arg.h"
#include "react_svg.h"

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
    if (0 == strcmp(lib_files[i], file_name))
    {
      return;
    }
  }
  if (n_libs >= 200)
  {
    printf("Max libraries exceeded, exiting . . . \n");
    exit(-1);
  }
  lib_files[n_libs] = strdup(file_name);
  n_libs++;
}

/*********************************/

void add_update_object(const char *the_tag, const char *the_js_object)
{
  if (n_objs >= 200)
  {
    printf("Max objects exceeded, exiting . . . \n");
    exit(-1);
  }
  //printf("Adding, tag: %s, object: %s\n", the_tag, the_js_object);
  tags[n_objs] = strdup(the_tag);  
  js_objs[n_objs] = strdup(the_js_object);
  n_objs++;
}

/*********************************/

void gen_calls(FILE *js_fp)
{
  fprintf(js_fp, "// -- START insert AJAX animation code --\n");
  fprintf(js_fp, 
    "var react_update_hrf = \"http://\" + location.hostname + \"/helloworld/tag?");
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

  fprintf(js_fp, "var update_tags = new Array(");
  for (int i=0; i < n_objs; i++)
  {
    fprintf(js_fp, "%s\"%s\"", i==0?"":",", tags[i]);
  }
  fprintf(js_fp, ");\n"); 

  fprintf(js_fp, 
     "var react_config_hrf = \"http://\" + location.hostname + \"/helloworld/config?\";\n");
  fprintf(js_fp, "var update_xReq;\n");
  fprintf(js_fp, "var config_xReq;\n");
  fprintf(js_fp, "var n_cfg = 0;\n");

  fprintf(js_fp, "function on_config_response()\n");
  fprintf(js_fp, "{\n");
  fprintf(js_fp, "  if (config_xReq.readyState != 4)  { return; }\n");
  fprintf(js_fp, "  console.log(\"config response: \" + config_xReq.responseText);\n");
  fprintf(js_fp, "  var val = JSON.parse(config_xReq.responseText);\n");
  fprintf(js_fp, "  update_objs[n_cfg].init(val);\n");
  
  fprintf(js_fp, "  n_cfg++;\n");
  fprintf(js_fp, "  if (n_cfg >= update_tags.length) \n");
  fprintf(js_fp, "  {\n");
  fprintf(js_fp, "    config_xReq.abort();\n");
  fprintf(js_fp, "    return;\n"); 
  fprintf(js_fp, "  }\n"); 
  fprintf(js_fp, 
     "  config_xReq.open(\"GET\", react_config_hrf + update_tags[n_cfg], true);\n");
  fprintf(js_fp, "  config_xReq.send(null);\n");
  fprintf(js_fp, "}\n");


  //fprintf(js_fp, "var sim_val = 20;\n");
  fprintf(js_fp, "function on_update_response()\n");
  fprintf(js_fp, "{\n");
  fprintf(js_fp, "  var val = JSON.parse(update_xReq.responseText);\n");
  fprintf(js_fp, "  console.log(\"response: \" + update_xReq.responseText);\n");
  fprintf(js_fp, "  for (var i=0; i < update_objs.length; i++)\n");
  fprintf(js_fp, "  {\n");
  //fprintf(js_fp, "    //console.log(\"update_objs[\" + i + \"] = \" + update_objs[i] + \"sim_val: \" + sim_val);\n");
  //fprintf(js_fp, "    //update_objs[i].update(sim_val + (i * 5));\n");
  fprintf(js_fp, "    console.log(\"val: \" + val[i]);\n");
  fprintf(js_fp, "    update_objs[i].update(val[i]);\n");
  //fprintf(js_fp, "    sim_val++;\n");
  //fprintf(js_fp, "    if (sim_val==100)\n");
  //fprintf(js_fp, "    {\n");
  //fprintf(js_fp, "      sim_val=0\n");
  //fprintf(js_fp, "    }\n");
           
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
  fprintf(js_fp, "  if (update_tags.length > 0)\n"); 
  fprintf(js_fp, "  {\n");
  fprintf(js_fp, "    config_xReq = new XMLHttpRequest();\n");
  fprintf(js_fp, "    config_xReq.onreadystatechange = on_config_response;\n");
  fprintf(js_fp, "    config_xReq.open(\"GET\", react_config_hrf + update_tags[0], true);\n");
  fprintf(js_fp, "    config_xReq.send(null);\n");
  fprintf(js_fp, "  }\n");
  fprintf(js_fp, "  var interval = setInterval(\"intervalHandler()\", 2000);\n");
  fprintf(js_fp, "};\n");
  fprintf(js_fp, "\n");
  fprintf(js_fp, "// -- END insert AJAX animation code --\n");
}

/*********************************/

void do_gen(const char *fname)
{
  const char *svg_file = "_tmp_display.svg";
  FILE *svg_fp = fopen(svg_file, "w");
  if (svg_fp == NULL)
  {
    perror(svg_file);
    exit(-1);
  }
  const char *js_file = "_tmp_display.js";
  FILE *js_fp = fopen(js_file, "w");
  if (svg_fp == NULL)
  {
    perror(js_file);
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

  fclose(js_fp);
  fclose(svg_fp);
}

/*********************************/

void gen_svg_header(FILE *fp, const char *title, int vb_x1, int vb_y1, int vb_x2, int vb_y2)
{
  fprintf(fp, "<!--\n");
  fprintf(fp, "Auto generated file DO NOT EDIT\n");
  fprintf(fp, "Generated by: %s\n", __FILE__);
  fprintf(fp, "-->\n");

  fprintf(fp, "<svg xmlns=\"http://www.w3.org/2000/svg\"\n");
  fprintf(fp, "     xmlns:xlink=\"http://www.w3.org/1999/xlink\"\n");
  fprintf(fp, "     width=\"100%%\" height=\"100%%\" viewBox=\"%d %d %d %d\" onload=\"load()\">\n",
          vb_x1, vb_y1, vb_x2, vb_y2);
  fprintf(fp, "\n");
  fprintf(fp, "  <title>%s</title>\n", title);
  fprintf(fp, "  <metadata>author: Don Carr</metadata>\n");
}

/*********************************/

void gen_final_file(const char *fname)
{
  FILE *fp = fopen(fname, "w");
  if (fp == NULL)
  {
    perror(fname);
    exit(-1);
  }
  gen_svg_header(fp, "zzTesting", 0, 0, 300, 150);
  include_file(fp, "_tmp_display.svg");
  fprintf(fp, "<script type=\"text/ecmascript\"><![CDATA[\n");
  for (int i=0; i < n_libs; i++)
  {
    fprintf(fp, "// -- START insert js library: %s\n", lib_files[i]);
    include_file(fp, lib_files[i]);
    fprintf(fp, "// -- END insert js library: %s\n", lib_files[i]);
  }
  include_file(fp, "_tmp_display.js");
  fprintf(fp, "]]></script>\n");
  fprintf(fp, "</svg>\n"); 
  fclose(fp);
}

/********************************/

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

/*********************************/

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
  struct dirent entry;

//dent = readdir(dir);  old call, NOT thread safe!!

  for (readdir_r(dir, &entry, &dent); dent != NULL; readdir_r(dir, &entry, &dent))
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
  gen_final_file("dtest.svg");

  const char *cmd = "sudo cp dtest.svg /var/www/";
  printf("%s\n", cmd);
  system(cmd);

  // Ok, borrar todos de los objetos para ver que funciona los destructores.
  for (i=0; i < n_plugins; i++)
  {
    delete(mi_objs[i]);
  }
  printf("\n\n");

  return 0;
}

/*********************************/

