#include "inc/output.h"
#include "inc/globals.h"

void match_dequeuer_join()
{
	pthread_join(thread_match_dequeuer_id, NULL);
}

void match_dequeuer_start()
{
	output.length = 0;
	output.enqueue_index = 0;
	output.dequeue_index = 0;

	pthread_mutex_init(&output.mutex, NULL);

	pthread_cond_init(&output.empty, NULL);
	pthread_cond_init(&output.full, NULL);

	output.closed = false;

	pthread_create(&thread_match_dequeuer_id, NULL, thread_match_dequeuer, NULL);
}

void match_output(match_t *match)
{
	int i;

	for (i = 0; i < TRANSACTIONS_LEN; i++)
	{
		 printf("%d,", transactions[match->transaction_id][i]);

		#ifdef DEBUG_PRINT
			fprintf(stderr, "%d,", transactions[match->transaction_id][i]);
		#endif
	}

	printf("%d\n", match->classification);

	#ifdef DEBUG_PRINT
		fprintf(stderr, "%d\n", match->classification);
	#endif
}

void match_enqueue(int thread_id, int transaction_id, int classification)
{
	pthread_mutex_lock(&output.mutex);
		while (output.length == BUFFER_SIZE)
			pthread_cond_wait(&output.full, &output.mutex);

		output.buffer[output.enqueue_index].transaction_id = transaction_id;
		output.buffer[output.enqueue_index].classification = classification;
		output.enqueue_index = (output.enqueue_index + 1) % BUFFER_SIZE;
		output.length++;

		#ifdef DEBUG_PRINT
			printf("[Thread%02d] added: [%06d,%06d]\n", thread_id, output.buffer[output.length - 1].transaction_id, output.buffer[output.length - 1].classification);
		#endif

		pthread_cond_signal(&output.empty);
	pthread_mutex_unlock(&output.mutex);
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

void *thread_match_dequeuer()
{
	match_t match;

	while (true)
	{
		pthread_mutex_lock(&output.mutex);
			if (output.closed && output.length == 0)
				pthread_exit(0);

			while (output.length == 0)
				pthread_cond_wait(&output.empty, &output.mutex);

			match = output.buffer[output.dequeue_index];
			output.dequeue_index = (output.dequeue_index + 1) % BUFFER_SIZE;
			output.length--;

			pthread_cond_signal(&output.full);
		pthread_mutex_unlock(&output.mutex);

		match_output(&match);

		#ifdef DEBUG_PRINT
			fprintf(stderr, "Just got: [%06d,%06d], still got %d unread\n", match.transaction_id, match.classification, msg_no);
		#endif
	}

	pthread_exit(0);
}
