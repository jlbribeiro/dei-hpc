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

char* file_to_string(char *filepath)
{
	size_t size;
	FILE* file;
	char* string;

	file = fopen(filepath, "r");
	if (!file)
	{
		fprintf(stderr, "Error: File \"%s\" couldn't be open. Aborting.", filepath);
		exit(-1);
	}

	fseek(file, 0, SEEK_END);
	size = ftell(file);
	fseek(file, 0, SEEK_SET);

	string = (char *) malloc((size + 1) * sizeof(char));
	fread(string, size, 1, file);
	fclose(file);

	string[size] = '\0';

	return string;
}

void swapf(float *a, float *b)
{
	float tmp = *a;
	*a = *b;
	*b = tmp;
}
