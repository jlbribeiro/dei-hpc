#ifndef UTILS_H
#define UTILS_H

#include <sys/time.h>

#define MARK_AS_USED(x) (void)(x)

#define START_CHRONO gettimeofday(&start, NULL)
#define STOP_CHRONO gettimeofday(&end, NULL)
#define GET_CHRONO calc_time(start, end)

#define CHECKPOINT fprintf(stderr, "[Node %d] [%s @ %s : %d] CHECKPOINT\n", mpi_self_rank, __func__, __FILE__, __LINE__)

long calc_time(struct timeval start, struct timeval end);

#endif
