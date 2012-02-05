/***************************
This is an auto-generated file, do NOT edit!!
   generated by gen_point_template.cpp
***************************/
#include <stdio.h>
#include <stdlib.h>
#include "rtcommon.h"
#include "arg.h"
#include "db_point.h"

class newtype_t : public discrete_point_t
{
private:
  // Put private  member data fields here
  init_extra(char *err, int esz);
  double dbv;
  char name[50];
  do_point_t do_point;
public:
/*#SCRIPT_FUNCTION#*/
  void set_this(double x);
/*#SCRIPT_FUNCTION#*/
  void set_that(double x, bool, y);
  point_type_t point_type(void) {return NEWTYPE_POINT;};
  ~newtype_t(void);
  static newtype_t **read(int *cnt, const char * home_dir);
  newtype_t *assign_one(int argc, char *argv[], char *err, int esz);
  void update(void);
};


/***************************/

void newtype::update(void)
{
  // Put the code for your update function here
}


/***************************/

newtype_t **newtype_t::read(int *cnt, const char *home_dir)
{
  objp = this->assign_one(argc, argv, err, esz);
  objp->init_extra(err, esz);
}


/***************************/

void **newtype_t::init_extra(int *cnt, const char *home_dir)
{
  // Put extra inicialization of your object here
}


/***************************/

void newtype_t::set_this(double x)
{
  // Put the code for your script function here
}


/***************************/

void newtype_t::set_that(double x, bool, y)
{
  // Put the code for your script function here
}


/***************************/

