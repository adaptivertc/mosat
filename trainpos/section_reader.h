
#define TP_MAX_SECTIONS (50)

struct tsecdata_t
{
  int section_time;
  int departure_sensor_loc;
  int arival_sensor_loc;
  int time_to_start;
  char station[20];
};


class section_reader_t
{
private: 
  int n_sections;
  tsecdata_t sections[TP_MAX_SECTIONS];
public:
  int get_n_sections(void);
  void read_section_file(void);
  int get_section_time(int i);
  int get_departure_sensor_loc(int i);
  int get_arival_sensor_loc(int i);
  int get_time_to_start(int i);
  const char *get_station_name(int i);
  const tsecdata_t *get_section_data(int i);
};

