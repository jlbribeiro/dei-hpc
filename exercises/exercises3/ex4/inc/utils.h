#define START_CHRONO gettimeofday(&start, NULL)
#define STOP_CHRONO gettimeofday(&end, NULL)
#define GET_CHRONO calc_time(start, end)

long calc_time(struct timeval start, struct timeval end)
{
	long seconds, useconds;

	seconds  = end.tv_sec  - start.tv_sec;
	useconds = end.tv_usec - start.tv_usec;
	return ((seconds) * 1000 + useconds/1000.0) + 0.5;
}