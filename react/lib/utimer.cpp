/***************************************************************************
Copyright (c) 2002,2003,2004 Donald Wayne Carr 

Permission is hereby granted, free of charge, to any person obtaining a 
copy of this software and associated documentation files (the "Software"), 
to deal in the Software without restriction, including without limitation 
the rights to use, copy, modify, merge, publish, distribute, sublicense, 
and/or sell copies of the Software, and to permit persons to whom the 
Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included 
in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS 
OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL 
THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER 
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING 
FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER 
DEALINGS IN THE SOFTWARE.
*************************************************************************/


#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <unistd.h>


#include "utimer.h"
/*******************************************************************

  This is a timer for a process that needs to execute actions at
  regular intervals. All intervals are given in micro-seconds, but of course
  the timing would greatly depend on the underlying OS and the
  loading on the system. That said, the average time between
  executions will be very very close since I read the current time and
  re-calculate the wait time every time. So, if you were late on the
  previous interval because of other processes getting the CPU, or
  inacuracies in the way the OS does things, I will wait less
  the next time.

*******************************************************************/

void utimer_t::normalize_timeval(struct timeval *atv)
{
  /*
   * Here we normalize the timeval structure after adding or
   * subtracting microseconds.
   */

  while (atv->tv_usec >= 1000000)
  {
    atv->tv_sec += 1;
    atv->tv_usec -= 1000000;
  }
  while (atv->tv_usec < 0)
  {
    atv->tv_sec -= 1;
    atv->tv_usec += 1000000;
  }
}

/*******************************************************************/

void utimer_print(utimer_status_t s)
{
  printf("start time:   %ld, %6ld\n",
		  s.start_time.tv_sec, s.start_time.tv_usec);
  printf("current time: %ld, %6ld\n",
		  s.current_time.tv_sec, s.current_time.tv_usec);
  printf("elapsed time: %ld, %6ld\n",
		  s.elapsed_time.tv_sec, s.elapsed_time.tv_usec);
  printf("next time:    %ld, %6ld\n",
		  s.next_time.tv_sec, s.next_time.tv_usec);
  printf("time left   :      %6ld\n", s.time_left);
  printf("interval    :      %6ld\n", s.interval);
}

/*******************************************************************/

void utimer_t::print_status(void)
{
  utimer_status_t s;
  s = this->get_status();
  utimer_print(s);
}

/*******************************************************************/

utimer_status_t utimer_t::get_status(void)
{
  utimer_status_t s;
  struct timeval tv2;
  struct timeval tv3;
  gettimeofday(&tv2, NULL);
  long dif = usec_timer_dif(tv2);
  tv3.tv_sec = tv2.tv_sec - tv0.tv_sec;
  tv3.tv_usec = tv2.tv_usec - tv0.tv_usec;
  normalize_timeval(&tv3);

  s.start_time = tv0;
  s.current_time = tv2;
  s.next_time = tv1;
  s.elapsed_time = tv3;
  s.time_left = dif;
  s.interval = interval;
  return s;
}

/*******************************************************************/

void utimer_t::inc_time(long usecs)
{
  /*
   * This increments the time for the next event by the
   * given number of microsecents.
   */

  tv1.tv_usec += usecs;
  normalize_timeval(&tv1);
}

/*******************************************************************/

struct timeval utimer_t::elapsed_time(void)
{
  /*
   * This returns the total elapsed time since the timer was started.
   */

  struct timeval tv2;
  struct timeval tv3;
  gettimeofday(&tv2, NULL);
  tv3.tv_sec = tv2.tv_sec - tv0.tv_sec;
  tv3.tv_usec = tv2.tv_usec - tv0.tv_usec;
  normalize_timeval(&tv3);
  return tv3;
}

/*******************************************************************/
long utimer_t::usec_timer_dif(struct timeval atv)
{
  /*
   * This returns the difference in microseconds between two
   * timeval structures.
   */

  long secdif = tv1.tv_sec - atv.tv_sec;
  long usecdif = tv1.tv_usec - atv.tv_usec;
  //long secdif = atv.tv_sec - tv1.tv_sec;
  //long usecdif = atv.tv_usec - tv1.tv_usec;
  return usecdif + (secdif * 1000000);
}

/*******************************************************************/

void utimer_t::my_busy_wait()
{
  struct timeval tv2;
  bool done = false;
  while (!done)
  {
    gettimeofday(&tv2, NULL);
    long dif = usec_timer_dif(tv2);
    if (dif <= 0)
    {
      done = true;
    }
  }
}

/*******************************************************************/

double utimer_t::late_time(void)
{
  struct timeval tv2;
  gettimeofday(&tv2, NULL);
  long dif = usec_timer_dif(tv2);
  return double(dif) * -1.0e-6;
}

/*******************************************************************/

void utimer_t::wait_next(void)
{
  /*
   * This procedure waits until the next time interval. Notice that
   * we check the current time and only wait for the REMAINING time.
   * This takes into account processing time or the fact that this
   * process might not have gottong the CPU right away, or just
   * inacuracies in the way the OS handles sleep. For instance in
   * DOS, the wait would be to the nearest 55 miliseconds.
   */

  struct timeval tv2;
  gettimeofday(&tv2, NULL);
  long dif = usec_timer_dif(tv2);

  //printf("now time: %ld, %ld\n", tv2.tv_sec, tv2.tv_usec);
  //printf("next time: %ld, %ld\n", tv1.tv_sec, tv1.tv_usec);
  //printf("waiting: %ld\n", dif);
  if (dif <= 0)
  {
    //printf("More than 1 sec late!: %ld usecs\n", dif);
    inc_time(interval); // We are behind, hope we can catch up!
    return;
  }
  //printf("Sleeping for %ld usecs\n", dif);
  if (busy_wait)
  {
    my_busy_wait();
  }
  else
  {
    usleep(dif);
  }
  //struct timespec ts;
  //ts.tv_sec = 0;
  //ts.tv_nsec = dif * 1000L;
  //nanosleep(&ts, NULL);
  //printf("done sleeping\n");
  inc_time(interval);
}

/*******************************************************************/

void utimer_t::set_start_time(void)
{
  /*
   * This sets the start time for the timer to NOW.
   */

  gettimeofday(&tv1, NULL);
  tv0 = tv1;
  inc_time(interval);
}

/*******************************************************************/

utimer_t::utimer_t()
{
  /*
   * Constructor for the timer, default to a start time of now
   * and a 1 second interval.
   */

  busy_wait = false; // Default, do not use busy wait, use usleep(). 
  set_interval(1000000); // Default interval is 1 second.
  set_start_time(); // Default start time is now
}

/*******************************************************************

int main(int argc, char *argv[])
{
  // a main for testing
  utimer_t utimer;
  utimer.set_interval(1000000);
  utimer.set_start_time();
  struct timeval mytv;
  for (int i=0; true; i++)
  {
    printf("%d\n", i);
    utimer.wait_next();
    mytv = utimer.elapsed_time();
    printf("elapsed time: %ld, %ld\n", mytv.tv_sec, mytv.tv_usec);
    usleep(400000L);
  }
}

*******************************************************************/


