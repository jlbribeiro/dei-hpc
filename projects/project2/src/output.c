#include <mpi.h>

#include "inc/output.h"
#include "inc/globals.h"

void output_match_to_thread_matches(thread_output_matches_t *thread_matches, int trans_id, int classification)
{
	if (thread_matches->length == THREAD_OUTPUT_MATCHES_SIZE)
		output_thread_matches_to_master(thread_matches);

	thread_matches->matches[thread_matches->length++] = trans_id;
	thread_matches->matches[thread_matches->length++] = classification;
}

void output_thread_matches_to_file(thread_output_matches_t *thread_matches)
{
	int i;
	int j;
	int match_id;
	int trans_id, classification;
	int part_index;
	int val;

	char str_int[5];
	int length = 0;
	char buffer[THREAD_OUTPUT_MATCHES_BUFFER_SIZE];

	for (match_id = 1; match_id < thread_matches->length; match_id += 2)
	{
		trans_id = thread_matches->matches[match_id];
		classification = thread_matches->matches[match_id + 1];

		for (part_index = 0; part_index < TRANSACTIONS_LEN; part_index++)
		{
			val = transactions[trans_id][part_index];
			if (val)
			{
				for (i = 0; val; i++)
				{
					str_int[i] = '0' + val % 10;
					val /= 10;
				}

				for (j = 1; j <= i; j++)
					buffer[length++] = str_int[i - j];

			} else
				buffer[length++] = '0';

			buffer[length++] = ',';
		}

		if (classification)
		{
			for (i = 0; classification; i++)
			{
				str_int[i] = '0' + classification % 10;
				classification /= 10;
			}

			for (j = 1; j <= i; j++)
				buffer[length++] = str_int[i - j];

		} else
			buffer[length++] = '0';

		buffer[length++] = '\n';
	}

	pthread_mutex_lock(&output_mutex);
		fwrite(buffer, sizeof(char), length, output_fd);
	pthread_mutex_unlock(&output_mutex);
}

void output_thread_matches_to_master(thread_output_matches_t *thread_matches)
{
	thread_matches->matches[0] = thread_matches->length - 1;

	pthread_mutex_lock(&output_mutex);
		MPI_Send(thread_matches->matches, thread_matches->length, MPI_INT, MPI_RANK_MASTER, mpi_self_rank, MPI_COMM_WORLD);
	pthread_mutex_unlock(&output_mutex);

	thread_matches->length = 1; /* reserve 0 index for the length */
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
