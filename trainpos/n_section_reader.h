
#define TP_MAX_SECTIONS (50)
#define TP_MAX_SENSORS (10)
#define TP_MAX_GLOBAL_SENSORS (200)

// Proposed structure for sensor information
struct sensor_info_t
{
  int section;
  int num_in_section;
  int global_loc;
};

struct tsecdata_t
{
  int section_time;
  int n_sensors;
  int sensor_location[TP_MAX_SENSORS];
  int time_to_start;
  int dwell_time;
  char station[20];
};


class section_reader_t
{
private: 
  int n_sections;
  int n_global_sensors;
  tsecdata_t sections[TP_MAX_SECTIONS];
  sensor_info_t sensors[TP_MAX_GLOBAL_SENSORS];  
public:
  int get_n_sections(void);
  int get_n_sensors(int section_number);
  int get_section_time(int section_number);

  void read_section_file(void);

  // The following two functions must go away!!
  // The model must allow a variable numer of sensors in
  // each section, including one or zero sensors.
  int get_departure_sensor_loc(int section_number);
  int get_arival_sensor_loc(int section_number);

  int get_time_to_start(int section_number);
  int get_sensor_loc(int section_number, int sensor_number);
  const char *get_station_name(int section_number);
  const tsecdata_t *get_section_data(int section_number);
  // New functions
  int get_sensor_loc(int global_sensor_number);
  int get_dwell_time(int section_number);
  const sensor_info_t *get_sensor_info(int global_sensor_number);
  int get_n_global_sensors(void);
};

extern section_reader_t sections;

