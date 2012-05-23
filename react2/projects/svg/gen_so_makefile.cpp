#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>

int main(int argc, char *argv[])
{
  DIR *dir;
  struct dirent *dent;

  const char *dir_name = "./plugins";

  char make_name[200];
  snprintf(make_name, sizeof(make_name), "%s/makefile", dir_name);

  dir = opendir(dir_name);

  if (dir == NULL)
  {
    perror(dir_name);
    return -1;
  }
  struct dirent entry;

  FILE *fp = fopen(make_name, "w");
  if (fp == NULL)
  {
    perror(make_name);
    return -1;
  }
//dent = readdir(dir);  old call, NOT thread safe!!

  char *base[500];
  int n_base = 0;

  for (readdir_r(dir, &entry, &dent); dent != NULL; readdir_r(dir, &entry, &dent))
  {
    if (dent->d_type != DT_REG)
    {
      //continue;
    }

    int len = strlen(dent->d_name);
    if (len < 5)
    {
      //printf("Skipping %s, too short\n", dent->d_name);
      continue;
    }
    char *ext = dent->d_name + (len - 4); 

    //printf("File name = %s\n", dent->d_name);
    //printf("ext = %s\n", ext);
    if (0 != strcmp(ext, ".cpp"))
    {
      continue;
    }
    //printf("Processing: %s\n", dent->d_name);
    char temp[100];
    snprintf(temp, sizeof(temp), "%s", dent->d_name);
    temp[len-4] = '\0';
    printf("Adding to makefile: base name: %s\n", temp);
    base[n_base] = strdup(temp);
    n_base++;
  
  }
  fprintf(fp, "\nexport CPATH=../\n\n");
  fprintf(fp, "all: ");
  for (int i=0; i < n_base; i++)
  {
    fprintf(fp, "%s.so ", base[i]); 
  }
  fprintf(fp, "\n\n");

  for (int i=0; i < n_base; i++)
  {
    fprintf(fp, "%s.so: %s.cpp\n", base[i], base[i]); 
    fprintf(fp, "\t g++ -Wall -shared -o %s.so %s.cpp\n\n", base[i], base[i]); 
  }

  fclose(fp);
  return 0;
}               
