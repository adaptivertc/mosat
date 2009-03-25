#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>

#include "rtcommon.h"
#include "arg.h"

enum field_type_t {RT_FLOAT, RT_INT, RT_BOOL, RT_STRING, RT_SELECT};

struct db_field_t
{
  field_type_t type;
  int length;
  char *prompt;
  char *name;
  int n_select;
  char **select_prompts;
  char **select_names;
  
};


struct gen_names_t
{
  const char *table_name;
  const char *field_file_name;
  const char *data_file_name;
  db_field_t *dbfs[500];
  int nf;
};

/************************************/

const char *field_type_to_string(field_type_t f)
{
  switch(f)
  {
    case RT_FLOAT:
      return "RT_FLOAT";
    case RT_INT:
      return "RT_INT";
    case RT_BOOL:
      return "RT_BOOL";
    case RT_STRING:
      return "RT_STRING";
    case RT_SELECT:
      return "RT_SELECT";
  }
  return "";
}


const char *gen_value(db_field_t *dbf, int i, char *buf, int sz)
{
  const char *indent = "          ";
  switch(dbf->type)
  {
    case RT_FLOAT:
      snprintf(buf, sz, ",\n%sargv[%d]", indent, i);
      break;
    case RT_INT:
      snprintf(buf, sz, ",\n%sargv[%d]", indent, i);
      break;
    case RT_BOOL:
      snprintf(buf, sz, 
         ",\n%s((argv[%d][0] == 't') || (argv[%d][0] == 'T') || (argv[%d][0] == 'y') || (argv[%d][0] == 'Y') || (argv[%d][0] == '1')) ? \"'T'\" : \"'F'\"", 
         indent, i, i, i, i, i);
      break;
    case RT_STRING:
      snprintf(buf, sz, ",\n%sargv[%d]", indent, i);
      break;
    case RT_SELECT:
      snprintf(buf, sz, ",\n%sargv[%d]", indent, i);
      break;
  }
  return buf;
}

const char *gen_format(db_field_t *dbf, char *buf, int sz)
{
  switch(dbf->type)
  {
    case RT_FLOAT:
      snprintf(buf, sz, "%%s");
      break;
    case RT_INT:
      snprintf(buf, sz, "%%s");
      break;
    case RT_BOOL:
      snprintf(buf, sz, "%%s");
      break;
    case RT_STRING:
      snprintf(buf, sz, "'%%s'");
      break;
    case RT_SELECT:
      snprintf(buf, sz, "'%%s'");
      break;
  }
  return buf;
}

static char xspec[100];

const char *field_spec(db_field_t *dbf)
{
  switch(dbf->type)
  {
    case RT_FLOAT:
      snprintf(xspec, sizeof(xspec), "%s FLOAT", dbf->name);
      break;
    case RT_INT:
      snprintf(xspec, sizeof(xspec), "%s INT", dbf->name);
      break;
    case RT_BOOL:
      snprintf(xspec, sizeof(xspec), "%s BOOLEAN", dbf->name);
      break;
    case RT_STRING:
      snprintf(xspec, sizeof(xspec), "%s VARCHAR(%d)", dbf->name, dbf->length);
      break;
    case RT_SELECT:
      snprintf(xspec, sizeof(xspec), "%s VARCHAR(%d)", dbf->name, dbf->length);
      break;
  }
  return xspec;
}

void print_field(db_field_t *dbf)
{
  printf("%s|%s|%s|%d|\n",  
            field_type_to_string(dbf->type),
            dbf->prompt, dbf->name, dbf->length);
}

/**************************************/

db_field_t *create_field(int argc, char *argv[])
{
  db_field_t *f = new(db_field_t);
  if (0 == strcasecmp(argv[0], "string"))
  {
    f->type = RT_STRING;
    f->prompt = strdup(argv[1]); 
    f->name = strdup(argv[2]); 
    f->length = atoi(argv[3]);
  }
  else if (0 == strcasecmp(argv[0], "double"))
  {
    f->type = RT_FLOAT;
    f->prompt = strdup(argv[1]); 
    f->name = strdup(argv[2]); 
    f->length = 0;
  }
  else if (0 == strcasecmp(argv[0], "int"))
  {
    f->type = RT_INT;
    f->prompt = strdup(argv[1]); 
    f->name = strdup(argv[2]); 
    f->length = 0;
  }
  else if (0 == strcasecmp(argv[0], "bool"))
  {
    f->type = RT_BOOL;
    f->prompt = strdup(argv[1]); 
    f->name = strdup(argv[2]); 
    f->length = 0;
  }
  else if (0 == strcasecmp(argv[0], "dropdown"))
  {
    if (argc != 5)
    {
      printf("Wrong number of args for dropdown\n");
      return NULL;
    }
    delim_separator_t ds_label(100, 20, ',');
    delim_separator_t ds_value(100, 20, ',');
    char **label_argv;
    int label_argc;
    char **value_argv;
    int value_argc;

    f->type = RT_SELECT;
    f->prompt = strdup(argv[1]); 
    f->name = strdup(argv[2]); 
    label_argv = ds_label.separate(&label_argc, argv[3]);
    value_argv = ds_value.separate(&value_argc, argv[4]);
    f->n_select = label_argc;
    f->select_prompts = new char*[label_argc];
    f->select_names = new char*[value_argc];
    int lmax = 0;
    for (int i=0; i < label_argc; i++)
    {
      f->select_prompts[i] = strdup(label_argv[i]);
      f->select_names[i] = strdup(value_argv[i]);
      int len = strlen(f->select_names[i]);
      if (len > lmax) lmax = len;
    }
    f->length = lmax;
  }
  else if (0 == strcasecmp(argv[0], "separator"))
  {
    return NULL;
  }
  else if (0 == strcasecmp(argv[0], "table"))
  {
    return NULL;
  }
  else
  {
    printf("Invalid type: %s\n", argv[0]);
    return NULL;
  }
  return(f);
}

/**************************************/

void gen_read_dat(FILE *fp_out, gen_names_t *gnames)
{
  fprintf(fp_out, "\n\n/***************************/\n\n");
  fprintf(fp_out, "int read_dat_file(sqlite3 *sqdb, const char *fname)\n");
  fprintf(fp_out, "{\n");
  fprintf(fp_out, "  FILE *fp = fopen(fname, \"r\");\n");
  fprintf(fp_out, "  if (fp == NULL)\n");
  fprintf(fp_out, "  {\n");
  fprintf(fp_out, "    perror(fname);\n");
  fprintf(fp_out, "    return -1;\n");
  fprintf(fp_out, "  }\n");
  fprintf(fp_out, "  char line[300];\n");

  fprintf(fp_out, "  for (int i=0; NULL != fgets(line, sizeof(line), fp); i++)\n");
  fprintf(fp_out, "  {\n");
  fprintf(fp_out, "    int argc;\n");
  fprintf(fp_out, "    char *argv[25];\n");
  fprintf(fp_out, "    char tmp[300];\n");
  fprintf(fp_out, "    safe_strcpy(tmp, line, sizeof(tmp));\n");
  fprintf(fp_out, "    argc = get_delim_args(tmp, argv, '|', 25);\n");
  fprintf(fp_out, "    if (argc == 0)\n");
  fprintf(fp_out, "    {\n");
  fprintf(fp_out, "      continue;\n");
  fprintf(fp_out, "    }\n");
  fprintf(fp_out, "    else if (argv[0][0] == '#')\n");
  fprintf(fp_out, "    {\n");
  fprintf(fp_out, "      continue;\n");
  fprintf(fp_out, "    }\n");
  fprintf(fp_out, "    printf(\"%%s\\n\", line);\n");
  fprintf(fp_out, "    write_one_%s(sqdb, argc, argv);\n", gnames->table_name);
  fprintf(fp_out, "  }\n");
  fprintf(fp_out, "  return 0;\n");
  fprintf(fp_out, "}\n");
}

/**************************************/

void  gen_header(FILE *fp_out)
{
  fprintf(fp_out, "/***************************\n");
  fprintf(fp_out, "This is an auto-generated file, do NOT edit!!\n");
  fprintf(fp_out, "***************************/\n");
  fprintf(fp_out, "#include <stdio.h>\n");
  fprintf(fp_out, "#include <sqlite3.h>\n");
  fprintf(fp_out, "#include \"rtcommon.h\"\n");
  fprintf(fp_out, "#include \"arg.h\"\n");
}

/**************************************/

void write_main_start(FILE *fp_out)
{
  fprintf(fp_out, "#include \"gen_common.h\"\n");

  fprintf(fp_out, "int main(int argc, char *argv[])\n");
  fprintf(fp_out, "{\n");
  fprintf(fp_out, "  sqlite3 *sqdb;\n");
  fprintf(fp_out, "  int retval;\n");
  fprintf(fp_out, "  char *errmsg;\n");
  fprintf(fp_out, "\n");
  fprintf(fp_out, "  retval = sqlite3_open_v2(\"react_def.sqlite\",  &sqdb,\n");
  fprintf(fp_out, "     SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, NULL);\n");
  fprintf(fp_out, "  if (retval != SQLITE_OK)\n");
  fprintf(fp_out, "  {\n");
  fprintf(fp_out, "    printf(\"Can not open db, error = %%d\\n\", retval);\n");
  fprintf(fp_out, "    printf(\"Err = %%s\\n\", sqlite3_errmsg(sqdb));\n");
  fprintf(fp_out, "    return 0;\n");
  fprintf(fp_out, "  }\n");
  fprintf(fp_out, "  \n");
}

/**************************************/

void write_main_end(FILE *fp_out)
{
  fprintf(fp_out, "  sqlite3_close(sqdb);\n");
  fprintf(fp_out, "}\n");
}

/**************************************/

void xprocess_file(FILE *fp_out, gen_names_t *gnames)
{
  char **argv;
  int argc;
  int line_num;

  gnames->nf = 0;

  delim_file_t df(300, 20, '|', '#');
  df.print_lines(true);


  for (argv = df.first(gnames->field_file_name, &argc, &line_num);
         (argv != NULL); 
            argv = df.next(&argc, &line_num))
  {
    if (argc < 2)
    {
      printf("Wrong number of args, line %d\n", line_num);
      continue;
    }
    printf("%d: ", line_num);
    for (int i=0; i < argc; i++)
    {
      printf("%s ", argv[i]);
    }  
    printf("\n");
    if (0 == strcasecmp(argv[0], "table"))
    {
      gnames->table_name = strdup(argv[2]);
    }
    db_field_t *dbf; 
    dbf = create_field(argc, argv);
    if (dbf != NULL)
    {
      gnames->dbfs[gnames->nf] = dbf;
      gnames->nf++;
    }
  }
  printf("Done parsing file, %d fields found\n", gnames->nf);
}

/**************************************/

void process_file(FILE *fp_out, gen_names_t *gnames)
{
  char **argv;
  int argc;
  int line_num;

  gnames->nf = 0;

  delim_file_t df(300, 20, '|', '#');
  df.print_lines(true);


  for (argv = df.first(gnames->field_file_name, &argc, &line_num);
         (argv != NULL); 
            argv = df.next(&argc, &line_num))
  {
    if (argc < 2)
    {
      printf("Wrong number of args, line %d\n", line_num);
      continue;
    }
    printf("%d: %s", line_num, argv[2]);
    for (int i=1; i < argc; i++)
    {
      printf(", %s", argv[i]);
    }  
    printf("\n");
    if (0 == strcasecmp(argv[0], "table"))
    {
      gnames->table_name = strdup(argv[2]);
    }
    db_field_t *dbf; 
    dbf = create_field(argc, argv);
    if (dbf != NULL)
    {
      gnames->dbfs[gnames->nf] = dbf;
      gnames->nf++;
    }
  }
  printf("Done parsing file, %d fields found\n", gnames->nf);

  char qs[5000];
  snprintf(qs, sizeof(qs), "create table %s(", gnames->table_name);
 
// "create table tbl2(a_string varchar(10), a_number smallint);",
  int first = true;
  for (int i=0; i < gnames->nf; i++)
  {
    if (!first) safe_strcat(qs, ", ", sizeof(qs));
    safe_strcat(qs, field_spec(gnames->dbfs[i]), sizeof(qs));
    first = false;
  }
  safe_strcat(qs, ", PRIMARY KEY (tag)", sizeof(qs)); 
  safe_strcat(qs, ");", sizeof(qs));
  printf("%s\n", qs);
  printf("%s\n", gnames->table_name);

  fprintf(fp_out, "\n\n/***************************/\n\n");
  fprintf(fp_out, "int write_one_%s(sqlite3 *sqdb, int argc, char *argv[]);\n",
            gnames->table_name);
  fprintf(fp_out, "int read_dat_file(sqlite3 *sqdb, const char *fname);\n");
  fprintf(fp_out, "\n");
  fprintf(fp_out, "int main(int argc, char *argv[])\n");
  fprintf(fp_out, "{\n");
  fprintf(fp_out, "  sqlite3 *sqdb;\n");
  fprintf(fp_out, "  int retval;\n");
  fprintf(fp_out, "  char *errmsg;\n");
  fprintf(fp_out, "\n");
  fprintf(fp_out, "  retval = sqlite3_open_v2(\"react_%s.sqlite\",  &sqdb,\n", 
         gnames->table_name);
  fprintf(fp_out, "     SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, NULL);\n");
  fprintf(fp_out, "  if (retval != SQLITE_OK)\n");
  fprintf(fp_out, "  {\n");
  fprintf(fp_out, "    printf(\"Can not open db, error = %%d\\n\", retval);\n");
  fprintf(fp_out, "    printf(\"Err = %%s\\n\", sqlite3_errmsg(sqdb));\n");
  fprintf(fp_out, "    return 0;\n"); 
  fprintf(fp_out, "  }\n");
  fprintf(fp_out, "  \n");
  fprintf(fp_out, "  retval = sqlite3_exec(sqdb,\n");
  fprintf(fp_out, "      \"%s\",\n", qs);
  fprintf(fp_out, "             NULL, NULL, &errmsg);\n");
  fprintf(fp_out, "  if (retval != SQLITE_OK)\n");
  fprintf(fp_out, "  {\n");
  fprintf(fp_out, "    printf(\"Can not execute query, error = %%d\\n\", retval);\n");
  fprintf(fp_out, "    if (errmsg != NULL)\n");
  fprintf(fp_out, "    {\n");
  fprintf(fp_out, "      printf(\"errmsg: %%s\\n\", errmsg);\n");
  fprintf(fp_out, "      sqlite3_free(errmsg);\n");
  fprintf(fp_out, "    }\n");
  fprintf(fp_out, "    sqlite3_close(sqdb);\n");
  fprintf(fp_out, "    return 0;\n");
  fprintf(fp_out, "  }\n");
  fprintf(fp_out, "  read_dat_file(sqdb, \"%s\");\n", gnames->data_file_name);
  fprintf(fp_out, "  sqlite3_close(sqdb);\n");
  fprintf(fp_out, "}\n");
}

/**************************************/

void gen_write_one(FILE *fp_out, gen_names_t *gnames)
{
  fprintf(fp_out, "\n\n/***************************/\n\n");
  fprintf(fp_out, "int write_one_%s(sqlite3 *sqdb, int argc, char *argv[])\n", 
          gnames->table_name);

  fprintf(fp_out, "{\n");
  //snprintf(str, sizeof(str), "insert into '%s' values(\n"); '%s',%d);",
  //         ddata[i].str, ddata[i].num);

  fprintf(fp_out, "  char qs[1000];");
  fprintf(fp_out, "  int retval;\n");
  fprintf(fp_out, "  char *errmsg;\n");

  char str[5000];
  bool first = true;
  snprintf(str, sizeof(str), 
   "  snprintf(qs, sizeof(qs), \"insert into \\\"%s\\\" values(", 
            gnames->table_name);
  for (int i=0; i < gnames->nf; i++)
  {
    char buf[200];
    if (!first) safe_strcat(str, ",", sizeof(str)); 
    safe_strcat(str, 
           gen_format(gnames->dbfs[i], buf, sizeof(buf)), sizeof(str));
    first = false;
  }
  safe_strcat(str, ");\"", sizeof(str)); 

  for (int i=0; i < gnames->nf; i++)
  {
    char buf[200];
    safe_strcat(str, 
        gen_value(gnames->dbfs[i], i, buf, sizeof(buf)), sizeof(str));
  }
  safe_strcat(str, ");", sizeof(str)); 
  fprintf(fp_out, "  %s\n", str);

  
  fprintf(fp_out, "  printf(\"qs =%%s\\n\", qs);\n");
  fprintf(fp_out, "  retval = sqlite3_exec(sqdb, qs,\n");
  fprintf(fp_out, "             NULL, NULL, &errmsg);\n");
  fprintf(fp_out, "  if (retval != SQLITE_OK)\n");
  fprintf(fp_out, "  {\n");
  fprintf(fp_out, "    printf(\"Can not execute query, error = %%d\\n\", retval);\n");
  fprintf(fp_out, "    if (errmsg != NULL)\n");
  fprintf(fp_out, "    {\n");
  fprintf(fp_out, "      printf(\"errmsg: %%s\\n\", errmsg);\n");
  fprintf(fp_out, "      sqlite3_free(errmsg);\n");
  fprintf(fp_out, "    }\n");
  fprintf(fp_out, "    return -1;\n");
  fprintf(fp_out, "  }\n");
  fprintf(fp_out, "  return 0;\n");

  fprintf(fp_out, "}\n");
}
  
/**************************************/

void process_dat_file(FILE *fp_out, gen_names_t *gnames)
{
  char **argv;
  int argc;
  int line_num;

  delim_file_t df(300, 20, '|', '#');
  df.print_lines(true);


  for (argv = df.first(gnames->field_file_name, &argc, &line_num);
         (argv != NULL);
            argv = df.next(&argc, &line_num))
  {
    if (argc < 2)
    {
    }
  }
}


/**************************************/

void gen_for_one_config(FILE *fp_out, gen_names_t *gnames) 
{
  process_file(fp_out, gnames);
  gen_write_one(fp_out, gnames);
  gen_read_dat(fp_out, gnames);
}

/**************************************/
int xmain(char *dir_name);

int xxxmain(int argc, char *argv[])
{
  struct gen_names_t gnames;

  if (argc < 3)
  {
    printf("You must specify the file definition file, and input file - exiting . . .\n"); exit(0);
  }

  gnames.field_file_name = argv[1];
  gnames.data_file_name = argv[2];

  const char *out_temp = "out.cpp";
  FILE *fp_out = fopen(out_temp, "w");
  if (fp_out == NULL)
  {
    perror(out_temp);
    exit(0);
  }

  gen_header(fp_out);
  gen_for_one_config(fp_out, &gnames);

  fclose(fp_out);

  char cmd[200];
  snprintf(cmd, sizeof(cmd), "mv %s out_%s.cpp", out_temp, gnames.table_name);
  printf("Executing: %s\n", cmd);
  system(cmd);
  //xmain("../dbfiles");
  return 0;
}

/**************************************/

void gen_create_table(FILE *fp, gen_names_t *gnames)
{
  char qs[5000];
  snprintf(qs, sizeof(qs), "create table %s(", gnames->table_name);

  int first = true;
  for (int i=0; i < gnames->nf; i++)
  {
    if (!first) safe_strcat(qs, ", ", sizeof(qs));
    safe_strcat(qs, field_spec(gnames->dbfs[i]), sizeof(qs));
    first = false;
  }
  safe_strcat(qs, ", PRIMARY KEY (tag)", sizeof(qs));
  safe_strcat(qs, ");", sizeof(qs));

  fprintf(fp, "  retval = sqlite3_exec(sqdb,\n");
  fprintf(fp, "      \"%s\",\n", qs);
  fprintf(fp, "             NULL, NULL, &errmsg);\n");
  fprintf(fp, "  if (retval != SQLITE_OK)\n");
  fprintf(fp, "  {\n");
  fprintf(fp, "    printf(\"Can not execute query, error = %%d\\n\", retval);\n");
  fprintf(fp, "    if (errmsg != NULL)\n");
  fprintf(fp, "    {\n");
  fprintf(fp, "      printf(\"errmsg: %%s\\n\", errmsg);\n");
  fprintf(fp, "      sqlite3_free(errmsg);\n");
  fprintf(fp, "    }\n");
  fprintf(fp, "    sqlite3_close(sqdb);\n");
  fprintf(fp, "    return 0;\n");
  fprintf(fp, "  }\n");

}

/**************************************/
void gen_insert_call(FILE *fp, gen_names_t *gnames)
{
  fprintf(fp, "  extern int write_one_%s(sqlite3 *sqdb, int argc, char *argv[]);\n", 
            gnames->table_name);
  fprintf(fp, "  insert_from_dat_file(sqdb, \"%s\", write_one_%s);\n", 
            gnames->data_file_name, gnames->table_name);
}

/**************************************/

void xgen_for_one_config(FILE *fp_out, FILE *fp_main, gen_names_t *gnames) 
{
  xprocess_file(fp_out, gnames);
  gen_create_table(fp_main, gnames);
  gen_insert_call(fp_main, gnames);
  gen_write_one(fp_out, gnames);
  //gen_read_dat(fp_out, gnames);
}

/**************************************/

int main(int argc, char *argv[])
{
  char *dir_name;
  DIR *dir;
  struct dirent *dent;
  char base_name[100];
  char config_name[100];
  char dat_name[100];
  struct gen_names_t gnames;

  if (argc < 2)
  {
    printf("You must specify a directory\n");
    return -1;
  }

  dir_name = argv[1];

  dir = opendir(dir_name);

  if (dir == NULL)
  {
    perror(dir_name);
    return -1;
  }

  const char *out_temp = "gen_one_fns.cpp";
  FILE *fp_out = fopen(out_temp, "w");
  if (fp_out == NULL)
  {
    perror(out_temp);
    exit(0);
  }

  const char *out_main = "outmain.cpp";
  FILE *fp_main = fopen(out_main, "w");
  if (fp_main == NULL)
  {
    perror(out_main);
    exit(0);
  }

  gen_header(fp_main);
  write_main_start(fp_main);

  gen_header(fp_out);
  
  dent = readdir(dir);
  while (dent != NULL)
  {
    int len = strlen(dent->d_name);
    if (0 == strncmp(".config", dent->d_name + len - 7, 7)) 
    {
      snprintf(config_name, sizeof(config_name), "%s/%s", 
                    dir_name, dent->d_name);
      snprintf(base_name, sizeof(base_name), "%s", dent->d_name);
      base_name[len-7] = '\0';
      snprintf(dat_name, sizeof(dat_name), "%s/%s.dat", 
                    dir_name, base_name);
      printf("%s\n", base_name); 
      printf("%s\n", config_name); 
      printf("%s\n", dat_name); 
      gnames.field_file_name = config_name;
      gnames.data_file_name = dat_name;
      xgen_for_one_config(fp_out, fp_main, &gnames);
    }
    dent = readdir(dir);
  }
  write_main_end(fp_main);
  fclose(fp_out);
  fclose(fp_main);
}