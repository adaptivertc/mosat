
#define TP_MAX_SECTIONS (50)
#define TP_MAX_SENSORS (10)

struct tsecdata_t
{
  int section_time;
  int n_sensors;
  int sensor_location[TP_MAX_SENSORS];
  //int departure_sensor_loc;
  //int arival_sensor_loc;
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
  int get_n_sensors(int section_number);
  void read_section_file(void);
  int get_section_time(int section_number);
  int get_departure_sensor_loc(int section_number);
  int get_arival_sensor_loc(int section_number);
  int get_time_to_start(int section_number);
  int get_sensor_loc(int section_number, int sensor_number);
  const char *get_station_name(int section_number);
  const tsecdata_t *get_section_data(int section_number);
};

extern section_reader_t sections;

