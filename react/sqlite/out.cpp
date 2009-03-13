/***************************
This is an auto-generated file, do NOT edit!!
***************************/
#include <stdio.h>
#include <sqlite3.h>
#include "../../include/rtcommon.h"
#include "../../include/arg.h"


/***************************/

int write_one_AI(sqlite3 *sqdb, int argc, char *argv[]);
int read_dat_file(sqlite3 *sqdb, const char *fname);

int main(int argc, char *argv[])
{
  sqlite3 *sqdb;
  int retval;
  char *errmsg;

  retval = sqlite3_open_v2("react_AI.sqlite",  &sqdb,
     SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, NULL);
  if (retval != SQLITE_OK)
  {
    printf("Can not open db, error = %d\n", retval);
    printf("Err = %s\n", sqlite3_errmsg(sqdb));
    return 0;
  }
  
  retval = sqlite3_exec(sqdb,
      "create table AI(TAG VARCHAR(25), DESCRIPTION VARCHAR(50), EU_LABEL VARCHAR(12), DRIVER VARCHAR(25), CARD VARCHAR(25), CHANNEL INT, EU_LO FLOAT, EU_HI FLOAT, RAW_LO FLOAT, RAW_HI FLOAT, DECIMAL_PLACES INT, ZERO_CUTOFF FLOAT, ALARM_LO FLOAT, CAUTION_LO FLOAT, CAUTION_HI FLOAT, ALARM_HI FLOAT, ALARM_DEADBAND FLOAT, ALARM_LO_ENABLE BOOLEAN, CAUTION_LO_ENABLE BOOLEAN, CAUTION_HI_ENABLE BOOLEAN, ALARM_HI_ENABLE BOOLEAN, PRIMARY KEY (TAG));",
             NULL, NULL, &errmsg);
  if (retval != SQLITE_OK)
  {
    printf("Can not execute query, error = %d\n", retval);
    if (errmsg != NULL)
    {
      printf("errmsg: %s\n", errmsg);
      sqlite3_free(errmsg);
    }
    sqlite3_close(sqdb);
    return 0;
  }
   //const char *args[] = { "AI1", "First Analog Input", "PSI", "modbus", "0", "5", "0", "500", "0", "4095", "1", "0.5", "0", "0", "0", "0", "0", "F", "F", "F", "F", "true"};
  //write_one_AI(sqdb, 22, args);
  read_dat_file(sqdb, "../dbfiles/ai.dat");
  sqlite3_close(sqdb);
}


/***************************/

int write_one_AI(sqlite3 *sqdb, int argc, char *argv[])
{
  char qs[1000];  int retval;
  char *errmsg;
    snprintf(qs, sizeof(qs), "insert into \"AI\" values('%s','%s','%s','%s','%s',%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s);",
          argv[0],
          argv[1],
          argv[2],
          argv[3],
          argv[4],
          argv[5],
          argv[6],
          argv[7],
          argv[8],
          argv[9],
          argv[10],
          argv[11],
          argv[12],
          argv[13],
          argv[14],
          argv[15],
          argv[16],
          ((argv[17][0] == 't') || (argv[17][0] == 'T') || (argv[17][0] == 'y') || (argv[17][0] == 'Y') || (argv[17][0] == '1')) ? "'T'" : "'F'",
          ((argv[18][0] == 't') || (argv[18][0] == 'T') || (argv[18][0] == 'y') || (argv[18][0] == 'Y') || (argv[18][0] == '1')) ? "'T'" : "'F'",
          ((argv[19][0] == 't') || (argv[19][0] == 'T') || (argv[19][0] == 'y') || (argv[19][0] == 'Y') || (argv[19][0] == '1')) ? "'T'" : "'F'",
          ((argv[20][0] == 't') || (argv[20][0] == 'T') || (argv[20][0] == 'y') || (argv[20][0] == 'Y') || (argv[20][0] == '1')) ? "'T'" : "'F'");
  printf("qs =%s\n", qs);
  retval = sqlite3_exec(sqdb, qs,
             NULL, NULL, &errmsg);
  if (retval != SQLITE_OK)
  {
    printf("Can not execute query, error = %d\n", retval);
    if (errmsg != NULL)
    {
      printf("errmsg: %s\n", errmsg);
      sqlite3_free(errmsg);
    }
    return -1;
  }
  return 0;
}


/***************************/

int read_dat_file(sqlite3 *sqdb, const char *fname)
{
  FILE *fp = fopen(fname, "r");
  if (fp == NULL)
  {
    perror(fname);
    return -1;
  }
  char line[300];
  for (int i=0; NULL != fgets(line, sizeof(line), fp); i++)
  {
    int argc;
    char *argv[25];
    char tmp[300];
    safe_strcpy(tmp, line, sizeof(tmp));
    argc = get_delim_args(tmp, argv, '|', 25);
    if (argc == 0)
    {
      continue;
    }
    else if (argv[0][0] == '#')
    {
      continue;
    }
    printf("%s\n", line);
    write_one_AI(sqdb, argc, argv);
  }
  return 0;
}
