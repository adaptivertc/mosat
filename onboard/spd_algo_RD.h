
class speed_algorithm_RD_t : public speed_algorithm_base_t 
{
private:
  speed_command_t check_speed(time_t now, double actual, 
    double the_lo_limit, double the_hi_limit, double shutdown_limit);

public:
  speed_algorithm_RD_t(void);
  void evaluate(time_t now, double actual_speed, double actual_distance, 
		current_speed_limits_t limits, speed_results_t *results);
};

