
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
  int section_time; // Time required to traverse this section.
  int n_sensors; // Number of sensors in this section.
  int sensor_location[TP_MAX_SENSORS]; // Location of sensor in second after leaving the station.
  int time_to_start; // Time to reach the start of this section.
  int dwell_time; // Average time waiting in this station.
  char station[20]; // The name of this station.
};


class section_reader_t
{
private: 
  int n_sections;
  tsecdata_t sections[TP_MAX_SECTIONS];
  sensor_info_t sensors[TP_MAX_GLOBAL_SENSORS];  
public:
  int get_n_sections(void); // Return the number of sections on this line.
  int get_n_sensors(int section_number); // Return the number of sensors for a section.
  void read_section_file(void); // Read the file.
  int get_section_time(int section_number); // Get the time (seconds) to traverse a section.
  // The following two functions must go away!!
  // The model must allow a variable numer of sensors in
  // each section, including one or zero sensors.
  int get_departure_sensor_loc(int section_number);
  int get_arival_sensor_loc(int section_number);

  int get_time_to_start(int section_number); // Get the time (seconds) to the start of a section.
  int get_sensor_loc(int section_number, int sensor_number);
  int get_dwell_time(int section_number);
  const char *get_station_name(int section_number);
  const tsecdata_t *get_section_data(int section_number);
  // proposed new functions
  // sensor_info_t get_sensor_info(int global_sensor_number);
  // int get_n_global_sensors(void);
};

extern section_reader_t sections;

