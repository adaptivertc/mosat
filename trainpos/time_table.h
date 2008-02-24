

class time_table_t
{
private:
  char train_id[500][30];
  time_t times[500];
  bool matched[500];
  int last_match;
  int n_departures;
public:
  void read_day(const char *fname);
  const char *match_departure(time_t actual_departure_time);
  bool is_a_match(time_t scheduled_time, time_t actual_time);
};


