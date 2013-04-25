#include "inc/utils.h"

long calc_time(struct timeval start, struct timeval end)
{
	long seconds, useconds;

	seconds  = end.tv_sec  - start.tv_sec;
	useconds = end.tv_usec - start.tv_usec;
	return ((seconds) * 1000 + useconds / 1000.0) + 0.5;
}
