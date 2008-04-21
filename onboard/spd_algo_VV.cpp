#define ARM

#include <stdio.h>
#include <time.h>

#ifndef ARM
#include <ncurses.h>
#endif


#include "spd_algo.h"
#include "spd_algo_VV.h"


/*****************************************************************************/

speed_algorithm_VV_t::speed_algorithm_VV_t(void)
{
}


/**************************************************************************************************/

void speed_algorithm_VV_t::evaluate(time_t now, double actual_speed, double actual_distance,
		                current_speed_limits_t limits, speed_results_t *results)
{
}

/*****************************************************************************/
