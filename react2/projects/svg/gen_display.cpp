
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include <dlfcn.h>
#include <time.h>

#include "gen_display.h"
#include "arg.h"
#include "react_svg.h"

static gen_plugin_base_t *get_plugin(const char *type);
static const int RT_SIM_MODE = 0;
static const int RT_REACT_MODE = 1;
static int run_mode = RT_SIM_MODE;
static bool popup_on = false;
static const char *sim_file="sim_pump.js";


/*********************************/

static int n_objs = 0;
static const char *js_objs[200];
static const char *tags[200];
static int n_js_libs = 0;
static const char *js_lib_files[200];
static int n_svg_libs = 0;
static const char *svg_lib_files[200];

struct obj_list_t 
{
  const char *obj;
  obj_list_t *next;
};

static int max_argn = -1;
static obj_list_t *arg_objs[50] = {
NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL
}; 
static int arg_count[50];

/*********************************/
void find_a_place_nearby(double *x,double *y, 
                         double item_x, double item_y, 
                         double item_width, double item_height)
{
  find_a_place_nearby(x, y, 
                      300, 150, 
                      item_x, item_y, 
                      item_width, item_height,
                      80, 40); 
}

void find_a_place_nearby(double *x,double *y, 
                         double screen_width, double screen_height, 
                         double item_x, double item_y, 
                         double item_width, double item_height,
                         double popup_width, double popup_height) 
{
  // First try below
  if ((item_y + item_height + popup_height) <= screen_height) 
  {
    *y = item_y + item_height; // put it just below the item
  }
  // Now try above
  else if ((item_y - item_height - popup_height) >= 0.0) 
  {
    *y = item_y - popup_height; // put it just above the item
  }
  // Ok, if above the mid point, put it as low as possible 
  else if ((item_y + item_height) < (screen_height / 2.0))
  {
    *y = screen_height - popup_height; // put it at the bottom of the screen 
  }
  // Otherwise put it all the way at the top 
  else
  {
    *y = 0.0; // put it at the top of the screen
  }
  double x1 = (item_x + (item_width / 2.0) - (popup_width / 2.0)); 
  double x2 = (item_x + (item_width / 2.0) + (popup_width / 2.0)); 
  // first try centered with the object
  if ((x1 >= 0.0) && (x2 <= screen_width))
  {
    *x = x1; // centered under the item 
    return;
  }
  // If off the screen left, just put it all the way left 
  else if (x1 < 0.0)
  {
     *x = 0.0; // as far left as possible
     return;
  }
  // Ok, it had to be off the screen right, put it all the way right 
  else
  {
    *x = screen_width - popup_width; // as far right as possible
  }
}

/***************************************************************/

void add_arg_object(int argn, const char *obj)
{
  if (argn >= 50)
  {
    printf("Max args exceeded (%d), exiting . . . \n", argn);
    exit(-1);
  }
  if (argn < 1)
  {
    printf("Args can NOT be less than 1, (%d), exiting . . . \n", argn);
    exit(-1);
  }
  argn--; // Java script arrays start with zero
  arg_count[argn]++;
  printf("Adding, object: %s, to be updated by the tag at position %d\n", obj, argn);
  obj_list_t *tmp = new obj_list_t;
  tmp->next = arg_objs[argn];
  tmp->obj = strdup(obj);
  arg_objs[argn] = tmp;
  if (argn > max_argn) max_argn = argn;
}

/*********************************/

void add_js_library(const char *file_name)
{
  for (int i=0; i < n_js_libs; i++)
  {
    if (0 == strcmp(js_lib_files[i], file_name))
    {
      return;
    }
  }
  if (n_js_libs >= 200)
  {
    printf("Max javascript libraries exceeded, exiting . . . \n");
    exit(-1);
  }
  js_lib_files[n_js_libs] = strdup(file_name);
  n_js_libs++;
}

/*********************************/

void add_svg_library(const char *file_name)
{
  for (int i=0; i < n_svg_libs; i++)
  {
    if (0 == strcmp(svg_lib_files[i], file_name))
    {
      return;
    }
  }
  if (n_svg_libs >= 200)
  {
    printf("Max SVG libraries exceeded, exiting . . . \n");
    exit(-1);
  }
  svg_lib_files[n_svg_libs] = strdup(file_name);
  n_svg_libs++;
}


/*********************************/

void add_animation_object(const char *the_tag, const char *the_js_object)
{
  // First check to see if the tagname will come from the command line.
  if (the_tag[0] == '$')
  {
    add_arg_object(atoi(the_tag+1), the_js_object);
    return;
  }
  // Now make sure we have not exceeded the MAX.
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

static void gen_simulation(FILE *js_fp)
{
  fprintf(js_fp, "// -- START insert simulation code --\n");

  fprintf(js_fp, "const update_objs = new Array(");
  for (int i=0; i < n_objs; i++)
  {
    fprintf(js_fp, "%s%s", i==0?"":",", js_objs[i]);
  }
  fprintf(js_fp, ");\n"); 

  fprintf(js_fp, "const update_tags = new Array(");
  for (int i=0; i < n_objs; i++)
  {
    fprintf(js_fp, "%s\"%s\"", i==0?"":",", tags[i]);
  }
  fprintf(js_fp, ");\n"); 

  fprintf(js_fp, "function update_objects()\n");
  fprintf(js_fp, "{\n");
  fprintf(js_fp, "  for (var i=0; i < update_objs.length; i++)\n");
  fprintf(js_fp, "  {\n");
  fprintf(js_fp, "    update_objs[i].update(sim.get_pv(update_tags[i]));\n");
  fprintf(js_fp, "  }\n");
  fprintf(js_fp, "}\n");
  fprintf(js_fp, "\n");

  fprintf(js_fp, "function send_output(tag, new_value)\n");
  fprintf(js_fp, "{\n");
  fprintf(js_fp, "  console.log(\"send_output, tag: \" + tag + \": \" + new_value);\n");
  fprintf(js_fp, "  sim.set_pv(tag, new_value);\n");
  fprintf(js_fp, "}\n");
  fprintf(js_fp, "\n");

  fprintf(js_fp, "function config_objects()\n");
  fprintf(js_fp, "{\n");
  fprintf(js_fp, "  for (var i=0; i < update_objs.length; i++)\n");
  fprintf(js_fp, "  {\n");
  fprintf(js_fp, "    console.log(\"config tag: \" + update_tags[i]);\n");
  fprintf(js_fp, "    update_objs[i].init(sim.get_cfg(update_tags[i]));\n");
  fprintf(js_fp, "  }\n");
  fprintf(js_fp, "}\n");
  fprintf(js_fp, "\n");

  fprintf(js_fp, "var sim_now=0.0;\n");
  fprintf(js_fp, "function intervalHandler()\n");
  fprintf(js_fp, "{\n");
  fprintf(js_fp, "  sim_now+=0.1\n");
  fprintf(js_fp, "  sim.update(sim_now);\n");
  fprintf(js_fp, "  update_objects();\n");
  fprintf(js_fp, "}\n");
  fprintf(js_fp, "\n");

  fprintf(js_fp, "function load()\n");
  fprintf(js_fp, "{\n");
  fprintf(js_fp, "  config_objects();\n");
  fprintf(js_fp, "  update_objects();\n");
  //fprintf(js_fp, "  pump_1_timeout();\n");
  //fprintf(js_fp, "  pump_2_timeout();\n");
  //fprintf(js_fp, "  pump_3_timeout();\n");
  //fprintf(js_fp, "  pump_1_timeout();\n");
  //fprintf(js_fp, "  pump_2_timeout();\n");
  //fprintf(js_fp, "  pump_3_timeout();\n");

  fprintf(js_fp, "  var interval = setInterval(\"intervalHandler()\", 100);\n");

  if (popup_on)
  {
    fprintf(js_fp, "  show_main();\n");
    fprintf(js_fp, "  show_popup(50,50, 'Extend', 'Retract');\n");
  }

  fprintf(js_fp, "};\n");
  fprintf(js_fp, "\n");
  fprintf(js_fp, "// -- END insert simulation code --\n");
}

/*********************************/

static void gen_ajax_animation(FILE *js_fp)
{
  fprintf(js_fp, "// -- START insert AJAX animation code --\n");
  if (n_objs > 0)
  {
    fprintf(js_fp, 
    "const react_update_hrf = \"http://\" + location.hostname + \"/react/tag?");
    for (int i=0; i < n_objs; i++)
    {
      fprintf(js_fp, "%s%s", i==0?"":"+", tags[i]);
    }
    fprintf(js_fp, "\";\n"); 

    fprintf(js_fp, "const update_objs = new Array(");
    for (int i=0; i < n_objs; i++)
    {
      fprintf(js_fp, "%s%s", i==0?"":",", js_objs[i]);
    }
    fprintf(js_fp, ");\n"); 

    fprintf(js_fp, "const update_tags = new Array(");
    for (int i=0; i < n_objs; i++)
    {
      fprintf(js_fp, "%s\"%s\"", i==0?"":",", tags[i]);
    }
    fprintf(js_fp, ");\n"); 
  }

  if (max_argn >= 0)
  {

    // Only generate the array of arrays of objects if there is at least one argument. 
    fprintf(js_fp, "var arg_objs = new Array(");
    for (int i=0; i <= max_argn; i++)
    {
      fprintf(js_fp, "%s\n    [", i==0?"":",");
      obj_list_t *tmp = arg_objs[i]; 
      for (int j=0; tmp != NULL; j++)
      {
        fprintf(js_fp, "%s%s", j==0?"":",", tmp->obj);
        tmp = tmp->next;
      }
      fprintf(js_fp, "]"); 
    }
    fprintf(js_fp, ");\n"); 
    fprintf(js_fp, "arg_update_hrf = \"http://\" + location.hostname + \"/react/tag\" + location.search;\n");
    fprintf(js_fp, "arg_config_hrf = \"http://\" + location.hostname + \"/react/config?\";\n");
    fprintf(js_fp, "var arg_update_xReq;\n");
    fprintf(js_fp, "var arg_config_xReq;\n");
    fprintf(js_fp, "var arg_tags;\n"); 
    fprintf(js_fp, "var n_arg_cfg = 0;\n");
  }


  if (n_objs > 0)
  {
    fprintf(js_fp, 
     "var react_config_hrf = \"http://\" + location.hostname + \"/react/config?\";\n");
    fprintf(js_fp, "var update_xReq;\n");
    fprintf(js_fp, "var config_xReq;\n");
    fprintf(js_fp, "var n_cfg = 0;\n");

    fprintf(js_fp, "function send_output(tag, val)\n");
    fprintf(js_fp, "{\n");
    fprintf(js_fp, "  var path = \"react/output?\" + tag + \"+\" + val;\n");
    fprintf(js_fp, "  console.log(\"send_output(\" + tag + \", \" + val + \")\");\n");
    fprintf(js_fp, "  console.log(path);\n");
    fprintf(js_fp, "  var post_request;\n");
    fprintf(js_fp, "  post_request = new XMLHttpRequest();\n");
    //fprintf(js_fp, "  post_request.open(\"POST\",\"react/send\",true);\n");
    fprintf(js_fp, "  post_request.open(\"POST\",path,true);\n");
    fprintf(js_fp, "  post_request.setRequestHeader(\"Content-type\",\n");
    fprintf(js_fp, "           \"application/x-www-form-urlencoded\");\n");
    fprintf(js_fp, "  post_request.send();\n");
    fprintf(js_fp, "}\n");

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
    fprintf(js_fp, "  console.log(\"config tag: \" + update_tags[n_cfg])\n");
    fprintf(js_fp, 
       "  config_xReq.open(\"GET\", react_config_hrf + update_tags[n_cfg], true);\n");
    fprintf(js_fp, "  config_xReq.send(null);\n");
    fprintf(js_fp, "}\n");
  }

  if (max_argn >= 0)
  {
    fprintf(js_fp, "function on_arg_config_response()\n");
    fprintf(js_fp, "{\n");
    fprintf(js_fp, "  if (arg_config_xReq.readyState != 4)  { return; }\n");
    fprintf(js_fp, "  console.log(\"arg config response: \" + arg_config_xReq.responseText);\n");
    fprintf(js_fp, "  var val = JSON.parse(arg_config_xReq.responseText);\n");
    fprintf(js_fp, "    var aobjs = arg_objs[n_arg_cfg]\n");
    fprintf(js_fp, "    for (var j=0; j < aobjs.length; j++)\n");
    fprintf(js_fp, "    {\n");
    fprintf(js_fp, "      aobjs[j].init(val);\n");
    fprintf(js_fp, "    }\n");
  
    fprintf(js_fp, "  n_arg_cfg++;\n");
    fprintf(js_fp, "  if (n_arg_cfg >= arg_tags.length) \n");
    fprintf(js_fp, "  {\n");
    fprintf(js_fp, "    arg_config_xReq.abort();\n");
    fprintf(js_fp, "    return;\n"); 
    fprintf(js_fp, "  }\n"); 
    fprintf(js_fp, 
     "  arg_config_xReq.open(\"GET\", arg_config_hrf + arg_tags[n_arg_cfg], true);\n");
    fprintf(js_fp, "  arg_config_xReq.send(null);\n");
    fprintf(js_fp, "}\n");

    fprintf(js_fp, "function update_arg_objects(objs, vals)\n");
    fprintf(js_fp, "{\n");
    fprintf(js_fp, "  for (var i=0; (i < vals.length) && (i < objs.length); i++)\n");
    fprintf(js_fp, "  {\n");
    fprintf(js_fp, "    var aobjs = arg_objs[i]\n");
    fprintf(js_fp, "    for (var j=0; j < aobjs.length; j++)\n");
    fprintf(js_fp, "    {\n");
    fprintf(js_fp, "      aobjs[j].update(vals[i]);\n");
    fprintf(js_fp, "    }\n");
    fprintf(js_fp, "  }\n");
    fprintf(js_fp, "}\n");
    fprintf(js_fp, "function on_arg_update_response()\n");
    fprintf(js_fp, "{\n");
    fprintf(js_fp, "  if (arg_update_xReq.readyState != 4)  { return; }\n");
    fprintf(js_fp, "  var val = JSON.parse(arg_update_xReq.responseText);\n");
    fprintf(js_fp, "  update_arg_objects(arg_objs, val);\n");
    fprintf(js_fp, "  arg_update_xReq.abort(); // set the ready state back to 0\n");
    fprintf(js_fp, "}\n");
  }
  

  if (n_objs > 0)
  {
    //fprintf(js_fp, "var sim_val = 20;\n");
    fprintf(js_fp, "function on_update_response()\n");
    fprintf(js_fp, "{\n");
    fprintf(js_fp, "  if (update_xReq.readyState != 4)  { return; }\n");
    fprintf(js_fp, "  var val = JSON.parse(update_xReq.responseText);\n");
    fprintf(js_fp, "  //console.log(\"response: \" + update_xReq.responseText);\n");
    fprintf(js_fp, "  for (var i=0; i < update_objs.length; i++)\n");
    fprintf(js_fp, "  {\n");
    //fprintf(js_fp, "    //console.log(\"update_objs[\" + i + \"] = \" + update_objs[i] + \"sim_val: \" + sim_val);\n");
    //fprintf(js_fp, "    //update_objs[i].update(sim_val + (i * 5));\n");
    //fprintf(js_fp, "    //console.log(\"val: \" + val[i]);\n");
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
  }

  fprintf(js_fp, "function intervalHandler()\n");
  fprintf(js_fp, "{\n");
  if (n_objs > 0)
  {
    fprintf(js_fp, "  if (update_xReq.readyState == 0)\n");  
    fprintf(js_fp, "  {\n");
    fprintf(js_fp, "    update_xReq.open(\"GET\", react_update_hrf, true);\n");
    fprintf(js_fp, "    update_xReq.send(null);\n");
    fprintf(js_fp, "  }\n");
  }
  if (max_argn >= 0)
  {
    fprintf(js_fp, "  if (arg_update_xReq.readyState == 0)\n");  
    fprintf(js_fp, "  {\n");
    fprintf(js_fp, "    arg_update_xReq.open(\"GET\", arg_update_hrf, true);\n");
    fprintf(js_fp, "    arg_update_xReq.send(null);\n");
    fprintf(js_fp, "  }\n");
  }
  fprintf(js_fp, "}\n");
  fprintf(js_fp, "\n");

  fprintf(js_fp, "function load()\n");
  fprintf(js_fp, "{\n");
  if (n_objs > 0)
  {
    fprintf(js_fp, "  update_xReq = new XMLHttpRequest();\n");
    fprintf(js_fp, "  update_xReq.onreadystatechange = on_update_response;\n");
    fprintf(js_fp, "  if (update_tags.length > 0)\n"); 
    fprintf(js_fp, "  {\n");
    fprintf(js_fp, "    console.log(\"config tag: \" + update_tags[0])\n");
    fprintf(js_fp, "    config_xReq = new XMLHttpRequest();\n");
    fprintf(js_fp, "    config_xReq.onreadystatechange = on_config_response;\n");
    fprintf(js_fp, "    config_xReq.open(\"GET\", react_config_hrf + update_tags[0], true);\n");
    fprintf(js_fp, "    config_xReq.send(null);\n");
    fprintf(js_fp, "  }\n");
  }
  if (max_argn >= 0)
  {
    fprintf(js_fp, "  arg_update_xReq = new XMLHttpRequest();\n");
    fprintf(js_fp, "  arg_update_xReq.onreadystatechange = on_arg_update_response;\n");
    fprintf(js_fp, "  console.log(\"arg_update_hrf: \" + arg_update_hrf)\n");
    fprintf(js_fp, "  var argtmp = location.search.substr(1);\n");
    fprintf(js_fp, "  arg_tags = argtmp.split('+');\n");
    fprintf(js_fp, "  for (var i=0; i < arg_tags.length; i++)\n");
    fprintf(js_fp, "  {\n");
    fprintf(js_fp, "    console.log(\"arg_tags[\" + i +  \"]: \" + arg_tags[i])\n");
    fprintf(js_fp, "  }\n");
    fprintf(js_fp, "  for (var i=0; (i < arg_objs.length); i++)\n");
    fprintf(js_fp, "  {\n");
    fprintf(js_fp, "    var aobjs = arg_objs[i]\n");
    fprintf(js_fp, "    for (var j=0; j < aobjs.length; j++)\n");
    fprintf(js_fp, "    {\n");
    fprintf(js_fp, "      console.log(\"arg objs[\" + i +\"][\" + j + \"]: \" + aobjs[j]);\n");
    fprintf(js_fp, "    }\n");
    fprintf(js_fp, "  }\n");
    fprintf(js_fp, "  if (arg_tags.length > 0)\n"); 
    fprintf(js_fp, "  {\n");
    fprintf(js_fp, "    arg_config_xReq = new XMLHttpRequest();\n");
    fprintf(js_fp, "    arg_config_xReq.onreadystatechange = on_arg_config_response;\n");
    fprintf(js_fp, "    arg_config_xReq.open(\"GET\", arg_config_hrf + arg_tags[0], true);\n");
    fprintf(js_fp, "    arg_config_xReq.send(null);\n");
    fprintf(js_fp, "  }\n");
  }
  fprintf(js_fp, "  console.log(\"react_update_hrf = \" + react_update_hrf);\n");
  fprintf(js_fp, "  console.log(\"react_config_hrf = \" + react_config_hrf);\n");

  fprintf(js_fp, "  var interval = setInterval(\"intervalHandler()\", 200);\n");
  fprintf(js_fp, "};\n");
  fprintf(js_fp, "\n");
  fprintf(js_fp, "// -- END insert AJAX animation code --\n");
}

/*********************************/

static void do_gen(const char *fname)
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

  const char *top_of_file = "_tmp_top_of_file.svg";
  FILE *svg_top_of_file_fp = fopen(top_of_file, "w");
  if (svg_top_of_file_fp == NULL)
  {
    perror(top_of_file);
    exit(-1);
  }

  plugin_data_t pdata;
  pdata.svg_fp = svg_fp; 
  pdata.js_fp = js_fp; 
  //pdata.svg_after_header_fp = 
  pdata.svg_top_of_file_fp = svg_top_of_file_fp;


  delim_file_t df(500, 50, '|', '#');
  df.print_lines(true);
  char **argv;
  int argc;
  int line_num;
  gen_plugin_base_t *obj;

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
    obj->generate(pdata, argc, argv);
  }

  if (run_mode == RT_REACT_MODE)
  {
    printf("Generating connection to REACT . . . \n");
    gen_ajax_animation(js_fp);
  }
  else
  {
    //add_js_library("sim_pump.js");
    add_js_library(sim_file);
    printf("Generating simulator . . . \n");
    gen_simulation(js_fp);
  }


  fclose(js_fp);
  fclose(svg_fp);
  fclose(svg_top_of_file_fp);
}

/*********************************/
static double vb_x1 = 0.0, vb_y1 = 0.0, vb_x2 = 300.0, vb_y2 = 150.0;

void set_viewbox(double x1, double y1, double x2, double y2)
{
  vb_x1 = x1;
  vb_y1 = y1;
  vb_x2 = x2;
  vb_y2 = y2;
}

/*********************************/


static void gen_final_file(const char *fname)
{
  FILE *fp = fopen(fname, "w");
  if (fp == NULL)
  {
    perror(fname);
    exit(-1);
  }

  time_t now;
  struct tm now_tm;
  char buf[40];
  
  fprintf(fp, "<!--\n");
  fprintf(fp, "Auto generated file DO NOT EDIT\n");
  fprintf(fp, "Generated by: %s, at ", __FILE__);
  now = time(NULL);
  localtime_r(&now, &now_tm);
  strftime(buf, sizeof(buf), "%F %T %z", &now_tm);
  fprintf(fp, "%s\n", buf);

  fprintf(fp, "-->\n");

  include_file(fp, ".", "_tmp_top_of_file.svg");

  for (int i=0; i < n_svg_libs; i++)
  {
    fprintf(fp, "<!-- START insert svg library: %s-->\n", svg_lib_files[i]);
    include_file(fp, "./svgplugins", svg_lib_files[i]);
    fprintf(fp, "<!-- END insert svg library: %s -->\n", svg_lib_files[i]);
  }

  include_file(fp, ".", "_tmp_display.svg");

  fprintf(fp, "</g>\n");
  if (popup_on)
  {

    include_file(fp, ".", "popup.svg");
  }

  fprintf(fp, "<script type=\"text/ecmascript\"><![CDATA[\n");
     // max one output every 250 ms
  fprintf(fp, "const MAX_OUTPUT_RATE = 250;\n"); 
    // delay update of output widgets after output for 1000 ms
    // This allows time for the new output to get to REACT to
    // avoid updating with the old value
  fprintf(fp, "const UPDATE_DELAY = 1000;\n"); 
  fprintf(fp, "const svgNS = \"http://www.w3.org/2000/svg\";\n");
  fprintf(fp, "var reactmainobj=document.getElementById(\"main_group\");\n");

  if (popup_on)
  {
    fprintf(fp, "var popupobj=document.getElementById(\"popup\");\n");
    fprintf(fp, "function show_main()\n");
    fprintf(fp, "{\n");
    fprintf(fp, "  reactmainobj.parentNode.appendChild(reactmainobj);\n");
    fprintf(fp, "}\n");
    fprintf(fp, "function show_popup(x,y,text_on, text_off, text_tag)\n");
    fprintf(fp, "{\n");
    fprintf(fp, "  popupobj.setAttribute(\"transform\", \"translate(\" + x +\",\" + y +\")\");\n");
    fprintf(fp, "  popupobj.parentNode.appendChild(popupobj);\n");
    fprintf(fp, "  var onobj=document.getElementById(\"popup_on\");\n");
    fprintf(fp, "  onobj.textContent=text_on;\n");
    fprintf(fp, "  var offobj=document.getElementById(\"popup_off\");\n");
    fprintf(fp, "  offobj.textContent=text_off;\n");
    fprintf(fp, "  var tagobj=document.getElementById(\"popup_tag\");\n");
    fprintf(fp, "  tagobj.textContent=text_tag;\n");
    fprintf(fp, "}\n");
  }

  for (int i=0; i < n_js_libs; i++)
  {
    fprintf(fp, "// -- START insert js library: %s\n", js_lib_files[i]);
    include_file(fp, "./jsplugins", js_lib_files[i]);
    fprintf(fp, "// -- END insert js library: %s\n", js_lib_files[i]);
  }

  include_file(fp, ".", "_tmp_display.js");

  fprintf(fp, "]]></script>\n");

  fprintf(fp, "</svg>\n"); 

  fclose(fp);
}

/********************************/

#define MAX_PLUGINS (100)
int n_plugins = 0;
gen_plugin_base_t *mi_objs[MAX_PLUGINS];

static gen_plugin_base_t *get_plugin(const char *type)
{
  for (int i=0; i < n_plugins; i++)
  {
    if (0 == strcmp(mi_objs[i]->get_name(), type)) return mi_objs[i];
  }
  return NULL;
}

/*********************************/

void print_help()
{
  printf("Usage: gen_display [-f <config-file-name>] [-d <plugin-director>]\n");
  printf("                   [-o <output-file-name>] [-react] [-sim]\n");
  printf("Run the display builder (gen_display)\n\n");
  printf("  -d   use an alternate directory for the plugin directory.\n");
  printf("       The -d command must be followed by a plugin directory.\n");
  printf("       This will be used as the plugin directory, in place of\n");
  printf("       the default of \"./plugins/\".\n");
  printf("  -f   use an alternate configuration file for the display.\n");
  printf("       The -f command must be followed by a file name.\n");
  printf("       This will be used as the configuration file name, in place of\n");
  printf("       the default of \"display.txt\".\n");
  printf("  -o   use an alternate output file name for the final output.\n");
  printf("       The -o command must be followed by a file name.\n");
  printf("       This will be used as the output file name, in place of\n");
  printf("       the default of \"dtest.svg\".\n");
  printf("  -react   option tells gen_display to generate Ajax code to connect to\n");
  printf("           REACT (via Apache)  for configuration and point value information\n");
  printf("  -sim     option tells gen_display to generate a simulator\n");
  printf("           for configuration and point value information\n");
  printf("             default sim file is: sim_pump.js\n");
  printf("  -sim_file same as -sim but, with the sim file specified in \n");
  printf("            the next argument\n");
  printf("  -popup   option enables popups for valves, pumps, etc.\n");
  printf("\n");
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

  const char *dir_name = "./plugins/";
  const char *config_name = "display.txt"; 
  const char *output_name = "dtest.svg"; 
  popup_on = false;

  for (int current_arg=1; current_arg < argc; current_arg++)
  {
    if (0 == strcmp(argv[current_arg], "-react"))
    {
      run_mode = RT_REACT_MODE;
    }
    else if (0 == strcmp(argv[current_arg], "-sim"))
    {
      run_mode = RT_SIM_MODE;
    }
    else if (0 == strcmp(argv[current_arg], "-popup"))
    {
      popup_on = true;
    }
    else if (0 == strcmp(argv[current_arg], "-d"))
    {
      if (argc > (current_arg + 1))
      {
        current_arg++;
        dir_name = argv[current_arg];
      }
      else
      {
        print_help();
        printf("For -d option, you MUST specify the directory, %d, %d\n", argc, current_arg);
        exit(1);
      }
    }
    else if (0 == strcmp(argv[current_arg], "-sim_file"))
    {
      if (argc > (current_arg + 1))
      {
        current_arg++;
        sim_file = argv[current_arg];
        run_mode = RT_SIM_MODE;
      }
      else
      {
        print_help();
        printf("For -sim_file option, you MUST specify the javascript sim file name, %d, %d\n", argc, current_arg);
        exit(1);
      }
    }
    else if (0 == strcmp(argv[current_arg], "-f"))
    {
      if (argc > (current_arg + 1))
      {
        current_arg++;
        config_name = argv[current_arg];
      }
      else
      {
        print_help();
        printf("For -f option, you MUST specify the configuration file name, %d, %d\n", argc, current_arg);
        exit(1);
      }
    }
    else if (0 == strcmp(argv[current_arg], "-o"))
    {
      if (argc > (current_arg + 1))
      {
        current_arg++;
        output_name = argv[current_arg];
      }
      else
      {
        print_help();
        printf("For -o option, you MUST specify the output file name, %d, %d\n", argc, current_arg);
        exit(1);
      }
    }
    else if (0 == strcmp(argv[current_arg], "--help"))
    {
      print_help();
      exit(0);
    }
    else if (argv[current_arg][0] == '-')
    {
      print_help();
      exit(1);
    }
    else
    {
      break;
    }

  }


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
      //continue;
    }

    int len = strlen(dent->d_name);
    if (len < 4)
    {
      //printf("Skipping %s, too short\n", dent->d_name);
      continue;
    }
    char *ext = dent->d_name + (len - 3);

    //printf("File name = %s\n", dent->d_name);
    if (0 != strcmp(ext, ".so"))
    {
      // Not a shared object file, IGNORE
      continue;
    }

    //printf("ext = %s\n", ext);
    //printf("File name = %s\n", dent->d_name);

    snprintf(sofile, sizeof(sofile), "%s/%s", dir_name, dent->d_name);

    printf("Opening plugin[%d]: %s\n", n_plugins + 1,  sofile);

    if (n_plugins > (MAX_PLUGINS - 1)) 
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
    mi_objs[n_plugins] = cofn();
    n_plugins++;

  }
  printf ("found %d plugins!!!\n", n_plugins);

  // Ok, probamos todos de los objetos para ver que si funcionan.
  for (i=0; i < n_plugins; i++)
  {
    printf("plugin[%d]-handles objects of type: %s\n", i+1, mi_objs[i]->get_name());
    //convertir_uno(mi_objs[i], "Teacher, esto es una prueba MUY MUY MUY tonta");
  }
  printf("\n");

  printf("Config file is: %s\n", config_name);
  printf("Output file is: %s\n", output_name);

  do_gen(config_name);
  if (max_argn >= 0)
  {
    bool args_ok = true;
    for (int i=0; i <= max_argn; i++)
    {
      printf("$%d: %d object(s)", i, arg_count[i]);
      if (arg_count[i] == 0)
      {
        args_ok = false;
        printf(" ERROR");
      }
      printf("\n");
    }
    if (!args_ok)
    {
      printf("There must be at least one object using each argument\n");
      exit(-1);
    }
  }

  gen_final_file(output_name);

  //char cmd[300];
  //snprintf(cmd, sizeof(cmd), "sudo cp %s /var/www/", output_name); 
  //printf("%s\n", cmd);
  //system(cmd);

  // Ok, borrar todos de los objetos para ver que funciona los destructores.
  for (i=0; i < n_plugins; i++)
  {
    delete(mi_objs[i]);
  }
  printf("\n\n");

  return 0;
}

/*********************************/

