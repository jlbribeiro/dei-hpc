#define START_CHRONO gettimeofday(&start, NULL)
#define STOP_CHRONO gettimeofday(&end, NULL)
#define GET_CHRONO calc_time(start, end)

#define clErrorHandling(method) if (err != CL_SUCCESS) { fprintf(stderr, "Error @ %s - code: %d", method, err); exit(1); }

long calc_time(struct timeval start, struct timeval end)
{
	long mtime, seconds, useconds;

	seconds  = end.tv_sec  - start.tv_sec;
	useconds = end.tv_usec - start.tv_usec;
	return ((seconds) * 1000 + useconds/1000.0) + 0.5;
}