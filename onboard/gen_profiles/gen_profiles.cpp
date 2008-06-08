
/************************************************************
 * gen_profiles.cpp
 *
 * This program is designed to automatically generate the 
 * piecewise linear profiles for light rail passenger service,
 * based on a list of velocity restrictions for curves, at grade
 * crossings, passenger stations, contstruction, etc. 
 *
 * This assumes that the trains have embedded control for a fixed
 * aceleration / deceleration of for instance 0.5 m/s² or 1.0 m/s².
 * This program must determine when restrictions overlap and 
 * generate the transitions between them. There are many little 
 * details that must be handled, such as that you can not switch
 * immediately from acceleration to deceleration. Also, if distances
 * are short enough between restrictions, it may not be worth the
 * electricity to accelerate to maximum speed between restrictions.
 *
 * This is designed for the rules and restrictions at the light rail
 * system in Guadalajar Mexico, though we plan to adapt it to other 
 * light rail systems.
 *
 * This will also be used to generate the informattion needed for 
 * driver cues such as telling them when to decelerate.
 *
 * **********************************************************/


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>

#include "rtcommon.h"
#include "restriction_reader.h"

#define RT_MAX_PROFILE_POINTS (500)
#define GEN_DIST (20.0)

class gen_data_t
{
private:
  char station[50];
  double distance_to_next;
  int n;
  double x[RT_MAX_PROFILE_POINTS];
  double desired[RT_MAX_PROFILE_POINTS];
  double low[RT_MAX_PROFILE_POINTS];
  double high[RT_MAX_PROFILE_POINTS];
  double very_high[RT_MAX_PROFILE_POINTS];
public:
  gen_data_t(void);
  void set_station(const char *name) {safe_strcpy(station, name, sizeof(station));};
  void set_distance_to_next(double d) {distance_to_next = d;};
  void gen_decel(double startv, double endv, double end_distance, double accel);
  void gen_accel(double startv, double endv, double start_distance, double accel);
  void print(void);
  void plot(void);
};

/***************************************************************/


/***************************************************************/

void gen_data_t::print(void)
{
  printf("%s|%0.1lf|\n", station, distance_to_next);
  for (int i=0; i < n; i++)
  {
    printf("%0.1lf|", x[i]);
  }
  printf("\n");
  for (int i=0; i < n; i++)
  {
    printf("%0.1lf|", desired[i] * 3.6);
  }
  printf("\n");
}

/***************************************************************/

void gen_data_t::plot(void)
{
  FILE *fp = fopen("plot.txt", "w");
  for (int i=0; i < n; i++)
  {
    fprintf(fp, "%0.1lf\t%0.1lf\n", x[i], desired[i] * 3.6);
  }
  fclose(fp);

  const char *base_name = "plot";
  const char *gtitle = station;
  bool window = true;
  
  const char *fname = "gnuplotoptions.txt";
  fp = fopen(fname, "w");
  if (fp == NULL)
  {
    perror(fname);
    return;
  }

  fprintf(fp, "set   autoscale\n");
  fprintf(fp, "unset log\n");
  fprintf(fp, "unset label\n");
  fprintf(fp, "set xtic auto\n");
  fprintf(fp, "set ytic auto\n");
  fprintf(fp, "set grid\n");
  fprintf(fp, "set xlabel \"meters \"\n");
  fprintf(fp, "set ylabel \"km/h\"\n");
  fprintf(fp, "set title \"%s\"\n", gtitle);

  if (!window)
  {
    fprintf(fp, "set terminal png size 1000,720\n"); 
  }
  fprintf(fp, "%s\n", "set output\n");
  fprintf(fp, "plot \"%s.txt\" using 1:2 with lines lw 2 title \"deseable\"", base_name);
  //fprintf(fp, ", \"%s.txt\" using 1:3 with lines lw 2 title \"deseable\"", base_name);
  //fprintf(fp, ", \"%s.txt\" using 1:4 with lines lw 2 title \"low\"", base_name);
  //fprintf(fp, ", \"%s.txt\" using 1:5 with lines lw 2 title \"high\"", base_name);
  //fprintf(fp, ", \"%s.txt\" using 1:6 with lines lw 2 title \"vhigh\"", base_name);
  //fprintf(fp, "\n");

  fclose(fp);
  char command[500];
  if (window)
  {
    snprintf(command, sizeof(command), "gnuplot -persist %s", fname);
  }
  else
  {
    snprintf(command, sizeof(command), "gnuplot %s > %s.png", fname, base_name);
  }
  system(command);
}

/***************************************************************/

gen_data_t::gen_data_t(void)
{
  n = 0;
  station[0] = '\0';
  distance_to_next = 0.0;
}

/***************************************************************/

void gen_data_t::gen_decel(double startv, double endv, double end_distance, double accel)
{
  double dist = fabs(((startv * startv) - (endv * endv)) / (2.0 * -accel));
  int idx = this->n;
  int n_points = 1 + int(dist / GEN_DIST);
  // Here we generate one point every GEN_DIST meters. 
  for (int i=0; i < n_points; i++)
  {
    double location = double(i) * GEN_DIST;
    double vel = sqrt(((2.0 * -accel) * location) + (startv * startv));
    this->x[idx + i] = end_distance - dist + location;    
    this->desired[idx + i] = vel;
    this->n++;
  } 
  // Now generate one last point at the final point where you reach the velocity.
  this->x[idx + n_points] = end_distance;    
  this->desired[idx + n_points] = endv;
  this->n++;
}

/***************************************************************/

void gen_data_t::gen_accel(double startv, double endv, double start_distance, double accel)
{
  double dist = fabs(((startv * startv) - (endv * endv)) / (2.0 * accel));
  int n_points = 1 + int(dist / GEN_DIST);
  int idx = this->n;
  // Here we generate one point every GEN_DIST meters. 
  for (int i=0; i < n_points; i++)
  {
    double location = double(i) * GEN_DIST;
    double vel = sqrt(((2.0 * accel) * location) + (startv * startv));
    this->x[idx + i] = start_distance + location;    
    this->desired[idx + i] = vel;
    this->n++;
  } 
  // Now generate one last point at the final point where you reach the velocity.
  this->x[idx + n_points] = start_distance + dist;    
  this->desired[idx + n_points] = endv;
  this->n++;
}

/***************************************************************/

int main(int argc, char *argv[])
{
  restriction_reader_t rr;
  gen_data_t gd;

  const char *fname = "profile_out.txt";
  FILE *fp = fopen(fname, "w");
  if (fp == NULL)
  {
    perror(fname);
    exit(0);
  }

  rr.read_restrictions();

  for (int i=0; i < rr.get_n_restrictions(); i++)
  {
    const restriction_def_t *res = rr.get_restriction(i);
  }
  gd.set_station("PerSur");
  gd.set_distance_to_next(1099.0);
  gd.gen_accel(0.0, 70.0 / 3.6, 0.0, 0.5);
  gd.gen_decel(70.0 / 3.6, 50.0 / 3.6, 320.0, 1.0);
  gd.gen_accel(50.0 / 3.6, 70.0 / 3.6, 650, 1.0);
  gd.gen_decel(70.0 / 3.6, 0.0 / 3.6, 1099.0, 1.0);
  gd.print();
  gd.plot();
}

/***************************************************************/

