

#include <stdio.h>
#include <stdlib.h>

#include "../include/rtcommon.h"
#include "../include/arg.h"
#include "onboard.h"
#include "sim_reader.h"

char *sim_names2[] =
{
  "Tetlan-Aurora",
  "Aurora-SanJacinto",
  "SanJacinto-SanAndres",
  "SanAndres-Cristobal",
  "Cristobal-Oblatos",
  "Oblatos-Belisario",
  "Belisario-SnJuan",
  "SnJuan-PUniver",
  "PUniver-Juarez",
  "Juarez-AreaManiobras",
  "AreaManiobras-Juarez",
  "Juarez-PUniver",
  "PUniver-SnJuan",
  "SnJuan-Belisario",
  "Belisario-Oblatos",
  "Oblatos-Cristobal",
  "Cristobal-SanAndres",
  "SanAndres-SanJacinto",
  "SanJacinto-Aurora",
  "Aurora-Tetlan",
  "Tetlan-AreaManiobras"
};

char *sim_names1[] =
{
  "PerifericoSur-Tesoro",
  "Tesoro-Espana",
  "Espana-PatriaSur",
  "PatriaSur-IslaRaza",
  "IslaRaza-18deMarzo",
  "18deMarzo-Urdaneta",
  "Urdaneta-UnidadDeportivo",
  "UnidadDeportivo-SantaFilomena",
  "SantaFilomena-Washington",
  "Washington-Mexicaltzingo",
  "Mexicaltzingo-Juarez",
  "Juarez-ElRefugio",
  "ElRefugio-Mezquitan",
  "Mezquitan-AvilaComacho",
  "AvilaComacho-DivisionNorte",
  "DivisionNorte-Atemajac",
  "Atemajac-Dermatologico",
  "Dermatologico-PerifericoNorte",
  "PerifericoNorte-Dermatologico",
  "Dermatologico-Atemajac",
  "Atemajac-DivisionNorte",
  "DivisionNorte-AvilaComacho",
  "AvilaComacho-Mezquitan",
  "Mezquitan-ElRefugio",
  "ElRefugio-Juarez",
  "Juarez-Mexicaltzingo",
  "Mexicaltzingo-Washington",
  "Washington-SantaFilomena",
  "SantaFilomena-UnidadDeportivo",
  "UnidadDeportivo-Urdaneta",
  "Urdaneta-18deMarzo",
  "18deMarzo-IslaRaza",
  "IslaRazaPatriaSur",
  "PatriaSur-Espana",
  "Espana-Tesoro",
  "Tesoro-PerifericoSur"
};

/*********************************************************************/

sim_reader_t::sim_reader_t(void)
{
  the_line = 1;
} 

/*********************************************************************/

double sim_reader_t::get_total_dist(int section)
{
  int i = sim_data[section].n - 1;
  return sim_data[section].dist[i] - sim_data[section].dist[0];
}

/*********************************************************************/

void sim_reader_t::get_sim_speed_dist(int section, int t, double *dist, double *speed)
{
  if (t >= sim_data[section].n)
  {
    *dist = sim_data[section].dist[sim_data[section].n-1] - sim_data[section].dist[0];
    *speed = 0.0;
    return;
  }
  *dist = sim_data[section].dist[t] - sim_data[section].dist[0];
  *speed = sim_data[section].speed[t];
}

/*********************************************************************/

void sim_reader_t::read_sim_data(void)
{
  if (the_line == 1)
  {
    n_sections = sizeof(sim_names1) / sizeof(sim_names1[0]);
  }
  else
  {
    n_sections = sizeof(sim_names2) / sizeof(sim_names2[0]);
  }
  delim_file_t df(300, 5, '\t', '#');
  for (int i=0; i < n_sections; i++)
  {
    char **argv;
    int argc, line_num;
    char fname[200];
    if (the_line == 1)
    {
      snprintf(fname, sizeof(fname), "vfiles_line1/%s.txt", sim_names1[i]);
    }
    else
    {
      snprintf(fname, sizeof(fname), "vfiles_line2/%s.txt", sim_names2[i]);
    }
    printf("Reading: %s\n", fname);

    argv = df.first(fname, &argc, &line_num);
    sim_data[i].n = 0;
    for (int j=0; argv != NULL; j++)
    {
      if (argc == 2)
      {
        sim_data[i].dist[j] = atof(argv[0]);
        sim_data[i].speed[j] = atof(argv[1]);
        sim_data[i].n++;
        //printf("'%lf' '%lf'\n", sim_data[i].dist[j], sim_data[i].speed[j]); 
      }
      else
      {
        printf("Wrong number of args: %d, line %d\n", argc, line_num);
      }
      argv = df.next(&argc, &line_num);
    }
  }
}

/*********************************************************************/


/*

int main(int argc, char *argv[])
{
  sim_reader_t sr;
}
*/


