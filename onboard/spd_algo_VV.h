
class speed_algorithm_VV_t : public speed_algorithm_base_t 
{
private:

public:
  speed_algorithm_VV_t(void);
  void evaluate(time_t now, double actual_speed, double actual_distance, 
		current_speed_limits_t limits, speed_results_t *results);
};

