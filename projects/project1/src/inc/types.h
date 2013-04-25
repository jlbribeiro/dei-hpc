#ifndef TYPES_H
#define TYPES_H

#include <pthread.h>

typedef struct match_t
{
	int classification;
	int transaction_id;

} match_t;

typedef struct thread_output_buffer_t
{
	char buffer[THREAD_OUTPUT_BUF_SIZE];
	int length;

} thread_output_buffer_t;

typedef struct work_queue_t
{
	int index;
	pthread_mutex_t mutex;

} work_queue_t;

#endif
