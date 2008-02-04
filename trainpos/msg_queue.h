
#ifndef __MSG_QUEUE_INC__
#define __MSG_QUEUE_INC__

struct crossing_event_t
{
  time_t time_stamp;
  int section;
  bool departure;
  bool update;
  bool skip;
};

struct crossing_queue_event_t
{
  long mtype;
  crossing_event_t data;
};

int create_message_queue(void);
int connect_message_queue(void);
void msgq_send_departure(time_t now, int section);
void msgq_send_arrival(time_t now, int section);
void msgq_send_update(time_t now);


#endif
