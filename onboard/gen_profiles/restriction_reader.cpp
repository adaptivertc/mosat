
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "rtcommon.h"
#include "arg.h"

#include "restriction_reader.h"

/*******************************************************/

restriction_reader_t::restriction_reader_t(void)
{
  n_restrictions = 0;
}

/*******************************************************/

void restriction_reader_t::print_restriction( restriction_def_t res)
{
  printf("%s: start: %lf, end: %lf, max speed: %lf, normal stop point: %lf\n", 
       restriction_string(res.type), res.start, res.end, res.max_speed, res.normal_stop_point);
}

/*******************************************************/

const char *restriction_reader_t::restriction_string(restriction_type_t type)
{
  switch (type)
  {
    case RESTRICT_CURVE: return "CURVE";
    case RESTRICT_STATION: return "STATION";
    case RESTRICT_CROSSING: return "CROSSING";
    case RESTRICT_CONSTRUCTION: return "CONSTRUCTION";
  }
  return "ERROR";
}

/*******************************************************/

int restriction_reader_t::read_restrictions(void)
{
  int argc, line_num;
  char **argv;
  n_restrictions = 0;
  delim_file_t df(600, 50, '\t', '#');
  argv = df.first("restrictions.txt", &argc, &line_num);
  for (int i=0; (argv != NULL) && (i < RT_MAX_RESTRICTIONS); i++)
  {
    if (argv == NULL)
    {
      break;
    }
    if (argc < 4)
    {
      printf("Wrong number of args: %d, line %d\n", argc, line_num);
      exit(0);
    }
    printf("%s, %s, %s, %s\n", argv[0], argv[1], argv[2], argv[3]);
    const char *tp_string = argv[0];
    safe_strcpy(restriction[i].description, argv[1], sizeof(restriction[i]));
    restriction[i].start = atof(argv[2]);
    restriction[i].end = atof(argv[3]);
    restriction[i].max_speed = atof(argv[4]);
    restriction[i].normal_stop_point = atof(argv[5]);
    if (0 == strcasecmp(tp_string, "CURVE"))
    {
      restriction[i].type = RESTRICT_CURVE;
    }
    else if (0 == strcasecmp(tp_string, "STATION"))
    {
      restriction[i].type = RESTRICT_STATION;
    }
    else if (0 == strcasecmp(tp_string, "CROSSING"))
    {
      restriction[i].type = RESTRICT_CROSSING;
    }
    else if (0 == strcasecmp(tp_string, "CONSTRUCTION"))
    {
      restriction[i].type = RESTRICT_CONSTRUCTION;
    }
    else
    {
      printf("Invalid restriction type on line %d: '%s'\n", line_num, tp_string);
      continue;
    }
    print_restriction(restriction[i]);
    n_restrictions++;
    argv = df.next(&argc, &line_num);
  }
  return n_restrictions;
}


/*********************************************************************/

