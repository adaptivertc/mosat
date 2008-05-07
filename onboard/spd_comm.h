
#ifndef __SPC_COMM_INC__
#define __SPC_COMM_INC__

// Note, for the following structure, we have interntionally left out any details
// of deterining which end of the train is coupled, which direction the train is going.
// It is assumed that this should be determined by the module which doors are "left", 
// and which doors are "right". This changets depending on the direction of the train!.
struct spd_discrete_t
{
  bool doors_open; // Any doors are open (left, right, or both)
  bool left_open;  // Left doors are open
  bool right_open; // Right doors are open
  bool master;     // This computer is the master
};

void reset_distance(int section);
void connect_modbus(void);
void get_actual_speed_dist(int section, int t, double *dist, double *speed, spd_discrete_t *discretes);
void init_io(void);
void spd_set_key(int ch);

#endif
