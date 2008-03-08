
#ifndef __TRAINPOS_INCLUDE__
#define __TRAINPOS_INCLUDE__

struct train_data_t
{
  int num; // sequential number assigned to trains entering service.
  char train_id[30]; // train id as given in the timetable.
  time_t service_entry_time; // exactly when the train entered service.
  time_t scheduled_service_entry_time; // when the train was scheduled to enter service.  Must implement.
  int line_location; // actual line location in seconds
  int section; // The section which this train in.
               // Actually, "section" could be calculated, but, we avoid a search, and verify the section is correct.
               // Question: When should we say that a train has advanced to the start of the next section?
               //    Should it be:
               // 	1) The estimated time that the train should stop at the station? 
               // 	2) The estimated time when the train should start moving to leave the station?
               // 	3) When the last sensor is crossed in the previous sensor?
               // 	    As of now it is 3)!!
  int next_sensor_to_cross; // The sensor which will be crossed next.
		// The sensorso are numbered 0 to n-1. (n = number of sensors in this section)
		// If the next sensor to cross = n, then the next sensor to cross is in the next section.
};

#endif

