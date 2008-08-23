

#include <stdio.h>
#include <unistd.h>

#include "random_tools.h"
#include "exp_sim.h"


class ac_unit_sim_t
{
private:
  double last_time;
  bool ac_is_on;
  double time_on;
  double time_off;
  double next_transition;
  double outside_temp;
  double steady_state_cold;
  double steady_state_hot;
  double cold_temp;
  double hot_temp;
  exp_simulator_t sim_hot;
  exp_simulator_t sim_cold;
public:  
  ac_unit_sim_t(double a_period, double a_fraction_on, 
                double a_start_delay, double start_time);
  bool update(double t, double *the_cold_temp, double *the_hot_temp);
  void update_on(double t);
  void update_off(double t);
};

/***********************************************************************/

ac_unit_sim_t::ac_unit_sim_t(double a_period, double a_fraction_on, double a_start_delay, double start_time)
{
  time_on = a_period * a_fraction_on;
  time_off = a_period * (1.0 - a_fraction_on);
  ac_is_on = false;
  last_time = start_time;
  next_transition = start_time + a_start_delay;
  outside_temp = 33.4;
  steady_state_cold = 4.5;
  steady_state_hot = 55.8;
  sim_cold.set_steady_state(outside_temp);
  sim_cold.set_current_value(outside_temp + 2.2);
  sim_cold.set_tau(22);
  sim_hot.set_steady_state(outside_temp);
  sim_hot.set_current_value(outside_temp - 1.8);
  sim_hot.set_tau(22);
}

/***********************************************************************/

bool ac_unit_sim_t::update(double time_now, 
            double *the_cold_temp, double *the_hot_temp)
{
  double elapsed_time = time_now - last_time;
  last_time = time_now;  
  if (ac_is_on)
  {
    update_on(time_now);
  }
  else
  {
    update_off(time_now);
  }
  cold_temp = sim_cold.get_current_value(elapsed_time);
  cold_temp = add_rand_noise(cold_temp, 100.0, 0.01);
  hot_temp = sim_hot.get_current_value(elapsed_time);
  hot_temp = add_rand_noise(hot_temp, 100.0, 0.01);
  *the_hot_temp = hot_temp;
  *the_cold_temp = cold_temp;
  return ac_is_on;
}

/***********************************************************************/

void ac_unit_sim_t::update_on(double time_now)
{
  printf("ON: next: %0.1lf\n", next_transition);
  if (time_now > next_transition)
  {
    ac_is_on = false;
    double next_time_off = add_rand_noise(time_off, time_off, 0.15); 
    next_transition = time_now + next_time_off;
    // Ok, if the unit is off, the steady state value is the outside temp
    sim_hot.set_steady_state(outside_temp);
    sim_hot.set_tau(18);
    sim_cold.set_steady_state(outside_temp);
    sim_cold.set_tau(18);
  }
}

/***********************************************************************/

void ac_unit_sim_t::update_off(double time_now)
{
  printf("OFF: next: %0.1lf\n", next_transition);
  if (time_now > next_transition)
  {
    ac_is_on = true;
    double next_time_on = add_rand_noise(time_on, time_on, 0.12); 
    next_transition = time_now + next_time_on;

    sim_hot.set_steady_state(steady_state_hot);
    sim_hot.set_tau(10);
    sim_cold.set_steady_state(steady_state_cold);
    sim_cold.set_tau(10);
  }
}

/***********************************************************************/

int main(int argc, char *argv[])
{
  bool on;
  double hot;
  double cold;
  ac_unit_sim_t unit(60.0, 0.35, 
                10.0, 0.0);

  for (int i=0; true; i++)
  {
    on = unit.update(double(i), &hot, &cold);
    printf("%s: %0.1lf, %0.1lf\n", on ? "on" : "off", cold, hot);
    sleep(1);
  }
}

/***********************************************************************/
