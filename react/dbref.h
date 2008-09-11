
#ifndef __DBREF_INC__
#define __DBREF_INC__

class rt_bool_ref_t
{
public:
  virtual bool val(void) = 0;
  virtual ~rt_bool_ref_t(void){};
};

class rt_double_ref_t
{
public:
  virtual double val(void) = 0;
  virtual ~rt_double_ref_t(void){};
};

class rt_long_ref_t
{
public:
  virtual long val(void) = 0;
  virtual ~rt_long_ref_t(void){};
};

class rt_double_ptr_ref_t : public rt_double_ref_t
{
private:
  double *dptr;
public:
  rt_double_ptr_ref_t(double *a_dptr){dptr = a_dptr;};
  double val(void){return *dptr;};
  virtual ~rt_double_ptr_ref_t(void){};
};

#endif

