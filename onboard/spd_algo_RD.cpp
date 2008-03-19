

#include <stdio.h>
#include <time.h>
#include <curses.h>


#include "spd_algo.h"
#include "spd_algo_RD.h"


/*****************************************************************************/

speed_algorithm_RD_t::speed_algorithm_RD_t(void)
{
	printf ("Starting speed algorithm\n");
	warning_beginning = 0;
	previous_state = SPD_STATE_NORMAL;
}

/**************************************************************************************************/

char *state_string(int state)
{
	switch(state)
	{
		case SPD_STATE_SHUTDOWN : return "SHUTDONW";
		case SPD_STATE_WARN_HI : return "HIGH SPEED";
		case SPD_STATE_NORMAL : return "NORMAL";
		case SPD_STATE_WARN_LO : return "LOW SPEED";
	}
	return "UNDEFINED";
}

/**************************************************************************************************/

speed_state_t speed_algorithm_RD_t::current_state(time_t now, double actual_speed, double low_limit, double high_limit, double shutdown_limit)
{
	double actual_low_limit = low_limit;
	double actual_high_limit = high_limit;
	
	if(previous_state == SPD_STATE_WARN_LO)
		actual_low_limit += SPD_STATE_DEADBAND;
	
	if(previous_state == SPD_STATE_WARN_HI)
		actual_high_limit -= SPD_STATE_DEADBAND;
		
	if(actual_speed >= shutdown_limit)
	{
		previous_state = SPD_STATE_SHUTDOWN;
		return SPD_STATE_SHUTDOWN;
	}
	else
		if(actual_speed >= actual_high_limit)
		{
			
		}
	
	return SPD_STATE_WARN_LO;

}

/**************************************************************************************************/

void speed_algorithm_RD_t::evaluate(time_t now, double actual_speed, double actual_distance,
		                current_speed_limits_t limits, speed_results_t *results)
{
	results->state = current_state(now,actual_speed,limits.low,limits.high,limits.very_high);
	
	mvprintw(27,40,"  Actual: %5.1lf", actual_speed);
	mvprintw(28,40,"     Low: %5.1lf", limits.low);
	mvprintw(29,40,"    High: %5.1lf", limits.high);
	mvprintw(30,40,"Shutdown: %5.1lf", limits.very_high);
	mvprintw(31,40,"   State: %-10s, time: %8d", state_string(results->state), now);
}

/*****************************************************************************/
