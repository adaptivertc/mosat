
#include <stdio.h>
#include <stdlib.h>

#include "rtcommon.h"
#include "arg.h"
#include "ap_config.h"

#include "msg_queue.h"
#include "tcontrol.h"
#include "event_alg.h"

#include "n_section_reader.h"



/********************************************************/

int section_reader_t::get_n_sections(void)
{
  return n_sections;
} 

/********************************************************/

int section_reader_t::get_n_sensors(int section_number)
{
  if ((section_number < 0) || (section_number >= n_sections))
  {
    printf("%s line %d: Out of range index (%d) to access section data\n",
       __FILE__, __LINE__, section_number);
    exit(1);
  }
  return sections[section_number].n_sensors;
}

/********************************************************/

int section_reader_t::get_section_time(int section_number)
{
  if ((section_number < 0) || (section_number >= n_sections))
  {
    printf("%s line %d: Out of range index (%d) to access section data\n",
       __FILE__, __LINE__, section_number);
    exit(1);
  }
  return sections[section_number].section_time;
}

/********************************************************/

int section_reader_t::get_sensor_loc(int section_number, 
                                               int sensor_number)
{
  if ((section_number < 0) || (section_number >= n_sections))
  {
    printf("%s line %d: Out of range index (%d) to access section data\n",
       __FILE__, __LINE__, section_number);
    exit(1);
  }
  if ((sensor_number < 0) || 
        (sensor_number >= sections[section_number].n_sensors))
  {
    printf("%s line %d: Out of range index (%d) to access sensor loc for section %d\n",
       __FILE__, __LINE__, sensor_number, section_number);
    exit(1);
  }
  return (sections[section_number].sensor_location[sensor_number]);
}

/********************************************************/
int section_reader_t::get_departure_sensor_loc(int section_number)
{
  if ((section_number < 0) || (section_number >= n_sections))
  {
    printf("%s line %d: Out of range index (%d) to access section data\n",
       __FILE__, __LINE__, section_number);
    exit(1);
  }
  return sections[section_number].sensor_location[0];
}

/********************************************************/

int section_reader_t::get_arival_sensor_loc(int section_number)
{
  if ((section_number < 0) || (section_number >= n_sections))
  {
    printf("%s line %d: Out of range index (%d) to access section data\n",
       __FILE__, __LINE__, section_number);
    exit(1);
  }
  int n_sensors = sections[section_number].n_sensors;
  return sections[section_number].sensor_location[n_sensors-1];
}

/********************************************************/

int section_reader_t::get_time_to_start(int section_number)
{
  if ((section_number < 0) || (section_number >= n_sections))
  {
    printf("%s line %d: Out of range index (%d) to access section data\n",
       __FILE__, __LINE__, section_number);
    exit(1);
  }
  return sections[section_number].time_to_start;
}

/********************************************************/

const char *section_reader_t::get_station_name(int section_number)
{
  if ((section_number < 0) || (section_number >= n_sections))
  {
    printf("%s line %d: Out of range index (%d) to access section data\n",
       __FILE__, __LINE__, section_number);
    exit(1);
  }
  return sections[section_number].station;
}

/********************************************************/

const tsecdata_t *section_reader_t::get_section_data(int section_number)
{
  if ((section_number < 0) || (section_number >= n_sections))
  {
    printf("%s line %d: Out of range index (%d) to access section data\n",
       __FILE__, __LINE__, section_number);
    exit(1);
  }
  return &sections[section_number];
}

/********************************************************/


void section_reader_t::read_section_file(void)
{
  int max = TP_MAX_SECTIONS;
  this->n_global_sensors = 0;

  const char *sections_file = ap_config.get_config("SECTIONS_FILE");
  if (sections_file == NULL)
  {
    printf("%s line %d: SECTIONS_FILE not defined\n",
       __FILE__, __LINE__);
    exit(1);
  }


  char line[300];

  FILE *fp = fopen(sections_file, "r");
  if (fp == NULL)
  {
    perror(sections_file);
    printf("%s line %d: Error opening file\n",
       __FILE__, __LINE__);
    exit(1);
  }

  int n_lines = 0;
  int total_time = 0;
  this->n_global_sensors = 0;
  for (int i=0; NULL != fgets(line, sizeof(line), fp); i++)
  {
    char tmp[300];
    int argc;
    char *argv[25];

    //printf("--- %s\n", line);
    safe_strcpy(tmp, line, sizeof(tmp));
    argc = get_delim_args(tmp, argv, '\t', 4);
    if ((argc < 3) || (argc > (TP_MAX_SENSORS + 3)))
    {
      printf("Wrong number of args, must be between 3 and %d: %s\n", TP_MAX_SENSORS + 3, line);
      continue;
    }
    tsecdata_t *this_section = &sections[n_lines];
    
    this_section->n_sensors = argc - 3;

    safe_strcpy(this_section->station, argv[0], sizeof(this_section->station));
    this_section->section_time = atol(argv[1]);
    sections[n_lines].dwell_time = atol(argv[2]);
    for (int i=0; i < (argc-3); i++)
    {
      this_section->sensor_location[i] = atol(argv[i+3]);

      sensors[n_global_sensors].section = n_lines;
      sensors[n_global_sensors].num_in_section = i;
      sensors[n_global_sensors].global_loc = 
          this_section->sensor_location[i] + total_time +
          this_section->sensor_location[i];
      n_global_sensors++;
    }

    sections[n_lines].time_to_start = total_time;
    printf("%20s(%d): %d, %d, %d, sensors: [",
      sections[n_lines].station,
      n_lines, 
      sections[n_lines].time_to_start, 
      sections[n_lines].section_time,
      sections[n_lines].dwell_time); 
    for (int i=0; i <  sections[n_lines].n_sensors; i++) 
    { 
      if (i != 0) printf(", ");
      printf("%d", sections[n_lines].sensor_location[i]); 
    } 
    printf("]\n"); 
    //sections[n_lines].sensor_location[0], 
    //sections[n_lines].sensor_location[1], 
    total_time += sections[n_lines].section_time; 
    if (n_lines != 0) total_time += RT_DWELL_TIME;
    n_lines++;
    if (n_lines >= max)
    {
      break;
    }
  }
  n_sections = n_lines;
}

/*********************************************************/

int section_reader_t::get_sensor_loc(int global_sensor_number)
{
  if ((global_sensor_number < 0) || (global_sensor_number >= n_global_sensors))
  {
    printf("%s line %d: Out of range index (%d) to access sensor data\n",
       __FILE__, __LINE__, global_sensor_number);
    exit(1);
  }
  return sensors[global_sensor_number].global_loc;
}

/*********************************************************/

int section_reader_t::get_dwell_time(int section_number)
{
  if ((section_number < 0) || (section_number >= n_sections))
  {
    printf("%s line %d: Out of range index (%d) to access section data\n",
       __FILE__, __LINE__, section_number);
    exit(1);
  }
  return sections[section_number].dwell_time;
}

/*********************************************************/

const sensor_info_t *section_reader_t::get_sensor_info(int global_sensor_number)
{
  if ((global_sensor_number < 0) || (global_sensor_number >= n_global_sensors))
  {
    printf("%s line %d: Out of range index (%d) to access sensor data\n",
       __FILE__, __LINE__, global_sensor_number);
    exit(1);
  }
  return &sensors[global_sensor_number];
}

/*********************************************************/

int section_reader_t::get_n_global_sensors(void)
{
  return n_global_sensors;
}

/*********************************************************/

section_reader_t sections;
ap_config_t ap_config;

int main(int argc, char *argv[])
{
  ap_config.read_file("test_config.txt");
  sections.read_section_file();

  return 0;
}

/*********************************************************/