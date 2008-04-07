

struct spd_sim_data_t
{
  int n;
  double speed[500];
  double dist[500];
};


class sim_reader_t
{
private:
  int n_sections;
  int the_line;
  spd_sim_data_t sim_data[RT_MAX_SECTIONS];
public:
  sim_reader_t(void);
  int get_n_sections(void) {return n_sections;};
  double get_total_dist(int section);
  void set_line(int aLine) {the_line = aLine;};
  void get_sim_speed_dist(int section, int t, double *dist, double *speed);
  void read_sim_data(void);
};



