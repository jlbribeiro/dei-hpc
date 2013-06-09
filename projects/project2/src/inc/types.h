#ifndef TYPES_H
#define TYPES_H

#include <pthread.h>
#include <mpi.h>

typedef struct match_t
{
	int classification;
	int transaction_id;

} match_t;

typedef struct thread_output_matches_t
{
	int matches[THREAD_OUTPUT_MATCHES_SIZE];
	int length;

} thread_output_matches_t;

typedef struct work_queue_t
{
	int index;
	int last_index;
	pthread_mutex_t mutex;

} work_queue_t;

#endif
