

#include <stdio.h>

void include_file(FILE *fp, const char *dirname, const char *fname)
{
//'int asprintf(char **strp, const char *fmt, ...);
  char *thepath;
  asprintf(&thepath, "%s/%s", dirname, fname);
  printf("//Including file: %s\n", thepath);
  FILE *fp_include = fopen(thepath, "r");
  if (fp_include == NULL)
  {
    perror(thepath);
    fprintf(stderr, "Can not open file for include: %s\n", thepath);
    return;
  }
  char line[200];
  while (NULL != fgets(line, sizeof(line), fp_include))
  {
    fprintf(fp, "%s", line);
  }
  fclose(fp_include);
}
