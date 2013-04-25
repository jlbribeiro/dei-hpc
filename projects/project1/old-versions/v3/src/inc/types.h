#ifndef TYPES_H
#define TYPES_H

#include <pthread.h>

typedef struct match_t
{
	int classification;
	int transaction_id;

} match_t;

typedef struct match_buffer_t
{
	match_t buffer[BUFFER_SIZE];
	int length;
	int enqueue_index;
	int dequeue_index;
	pthread_mutex_t mutex;
	pthread_cond_t empty;
	pthread_cond_t full;
	bool closed;

} match_output_buffer_t;

typedef struct work_queue_t
{
	int index;
	pthread_mutex_t mutex;

} work_queue_t;

#endif
