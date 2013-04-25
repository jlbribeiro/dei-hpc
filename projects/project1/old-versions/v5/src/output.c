#include "inc/output.h"
#include "inc/globals.h"

void output_match_to_thread_buffer(thread_output_buffer_t *thread_buffer, int trans_id, int classification)
{
	int i, j;
	int val;
	int part_index;

	char str_int[5];

	if (thread_buffer->length >= THREAD_OUTPUT_BUF_MAX_LEN)
		output_thread_buffer_to_file(thread_buffer);

	for (part_index = 0; part_index < TRANSACTIONS_LEN; part_index++)
	{
		val = transactions[trans_id][part_index];
		for (i = 0; val; i++)
		{
			str_int[i] = '0' + val % 10;
			val /= 10;
		}

		for (j = 1; j <= i; j++)
			thread_buffer->buffer[thread_buffer->length++] = str_int[i - j];

		thread_buffer->buffer[thread_buffer->length++] = ',';
	}

	if (classification)
	{
		for (i = 0; classification; i++)
		{
			str_int[i] = '0' + classification % 10;
			classification /= 10;
		}

		for (j = 1; j <= i; j++)
			thread_buffer->buffer[thread_buffer->length++] = str_int[i - j];

	} else
		thread_buffer->buffer[thread_buffer->length++] = '0';

	thread_buffer->buffer[thread_buffer->length++] = '\n';
}

void output_thread_buffer_to_file(thread_output_buffer_t *thread_buffer)
{
	pthread_mutex_lock(&output_mutex);
		fwrite(thread_buffer->buffer, sizeof(char), thread_buffer->length, output_fd);
		thread_buffer->length = 0;
	pthread_mutex_unlock(&output_mutex);
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
