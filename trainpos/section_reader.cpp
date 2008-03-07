
#include <stdio.h>
#include <stdlib.h>

#include "rtcommon.h"
#include "arg.h"
#include "tpconfig.h"

#include "msg_queue.h"
#include "tcontrol.h"
#include "event_alg.h"

#include "section_reader.h"

int section_reader_t::get_n_sections(void)
{
  return n_sections;
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
  return (sections[section_number].sensor_locs[sensor_number]);
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
  return sections[section_number].departure_sensor_loc;
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
  return sections[section_number].arival_sensor_loc;
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

  const char *sections_file = tpconfig.get_config("SECTIONS_FILE");
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
  for (int i=0; NULL != fgets(line, sizeof(line), fp); i++)
  {
    char tmp[300];
    int argc;
    char *argv[25];

    //printf("--- %s\n", line);
    safe_strcpy(tmp, line, sizeof(tmp));
    argc = get_delim_args(tmp, argv, '\t', 4);
    if (argc != 4)
    {
      printf("Wrong: %s\n", line);
      continue;
    }

    safe_strcpy(sections[n_lines].station, argv[0], sizeof(sections[n_lines].station));
    sections[n_lines].section_time = atol(argv[1]);
    sections[n_lines].departure_sensor_loc = atol(argv[2]);
    sections[n_lines].arival_sensor_loc = atol(argv[3]);
    sections[n_lines].time_to_start = total_time;
    printf("%20s(%d): %d %d %d %d\n",
      sections[n_lines].station,
      n_lines,
      sections[n_lines].section_time,
      sections[n_lines].departure_sensor_loc,
      sections[n_lines].arival_sensor_loc,
      sections[n_lines].time_to_start );
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
