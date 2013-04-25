#ifndef UTILS_H
#define UTILS_H

#include <sys/time.h>

#define START_CHRONO gettimeofday(&start, NULL)
#define STOP_CHRONO gettimeofday(&end, NULL)
#define GET_CHRONO calc_time(start, end)

long calc_time(struct timeval start, struct timeval end);

#endif
