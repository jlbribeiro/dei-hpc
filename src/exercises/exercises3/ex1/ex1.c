/*
 * High Performance Computing
 * April 2013
 */

#include <omp.h>
#include <stdio.h>
#include <pthread.h>

#define N_THREADS 8

void openmp()
{
	int i;

	omp_set_num_threads(N_THREADS);

	#pragma omp parallel for
	for (i = 0; i < N_THREADS; i++)
		printf("Hello world, I am thread %d\n", omp_get_thread_num());

}

void* worker(void* id)
{
	int my_id = *((int *) id);

	printf("Hello world, I am thread %d\n", my_id);

	pthread_exit(0);
}

void pthreads()
{
	int i, t_ids[N_THREADS];
	pthread_t threads[N_THREADS];

	for (i = 0; i < N_THREADS; i++)
	{
		t_ids[i] = i+1;
		pthread_create(&threads[i], NULL, worker, &t_ids[i]);
	}

	for (i = 0; i < N_THREADS; i++)
		pthread_join(threads[i], NULL);
}

int main(int argc, char** argv)
{
	printf("---------------------------\n");
	printf("----------OPENMP-----------\n");
	printf("---------------------------\n");
	openmp();
	getchar();

	printf("---------------------------\n");
	printf("---------PTHREADS----------\n");
	printf("---------------------------\n");
	pthreads();

	return 0;
}
