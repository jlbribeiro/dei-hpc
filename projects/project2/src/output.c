#include "inc/output.h"
#include "inc/globals.h"

void output_match_to_thread_buffer(thread_output_buffer_t *thread_buffer, int trans_id, int classification)
{
	if (thread_buffer->length == THREAD_OUTPUT_BUF_SIZE)
		output_thread_buffer_to_master(thread_buffer);

	thread_buffer->buffer[thread_buffer->length++] = trans_id;
	thread_buffer->buffer[thread_buffer->length++] = classification;
}

void output_thread_buffer_to_file(thread_output_buffer_t *thread_buffer)
{
	pthread_mutex_lock(&output_mutex);
		fwrite(thread_buffer->buffer, sizeof(int), thread_buffer->length, output_fd);
		thread_buffer->length = 0;
	pthread_mutex_unlock(&output_mutex);
}

void output_thread_buffer_to_master(thread_output_buffer_t *thread_buffer)
{
	/* TODO: send results to the master */
	thread_buffer->length = 0;
}

void show_rules(int n)
{
	int i, j;

	fprintf(stderr, "Rules:\n");
	for (i = 0; i < n; i++)
	{
		for (j = 0; j < RULES_LEN; j++)
			fprintf(stderr, "%d ", rules[i][j]);

		fprintf(stderr, "\n");
	}

	fprintf(stderr, "\n");
}

void show_transactions(int n)
{
	int i, j;

	fprintf(stderr, "Transactions:\n");
	for (i = 0; i < n; i++)
	{
		for (j = 0; j < TRANSACTIONS_LEN; j++)
			fprintf(stderr, "%d ", transactions[i][j]);

		fprintf(stderr, "\n");
	}

	fprintf(stderr, "\n");
}
