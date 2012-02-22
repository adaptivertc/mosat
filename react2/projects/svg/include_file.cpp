

#include <stdio.h>

void include_file(FILE *fp, const char *fname)
{
  printf("//Including file: %s\n", fname);
  FILE *fp_include = fopen(fname, "r");
  if (fp_include == NULL)
  {
    perror(fname);
    fprintf(fp, "\n<!-- Can not open file for include: %s -->\n\n", fname);
    return;
  }
  char line[200];
  while (NULL != fgets(line, sizeof(line), fp_include))
  {
    fprintf(fp, "%s", line);
  }
  fclose(fp_include);
}
