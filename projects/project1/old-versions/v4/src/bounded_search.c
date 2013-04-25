#include <stdio.h>
#include <omp.h>

#include "inc/bounded_search.h"
#include "inc/utils.h"
#include "inc/output.h"

int bounded_search_first_rule_match(int value, int part_index, int begin, int end)
{
	int it, step, count;

	count = end - begin;
	while (count > 0)
	{
		step = count >> 1;
		it = begin + step;

		if (rules[it][part_index] < value)
		{
			begin = it + 1;
			count -= step + 1;

		} else
			count = step;
	}

	return rules[begin][part_index] == value ? begin : NOT_FOUND;
}

int bounded_search_last_rule_match(int value, int part_index, int begin, int end)
{
	int it, step, count;

	count = end - begin;
	while (count > 0)
	{
		step = count >> 1;
		it = begin + step;

		if (rules[it][part_index] <= value)
		{
			begin = it + 1;
			count -= step + 1;

		} else
			count = step;
	}

	return rules[begin - 1][part_index] == value ? begin : NOT_FOUND;
}

void match_bounded_search_transaction(int trans_id, int part_index, int begin, int end)
{
	int value, zero_first, zero_last, value_first, value_last;

	if (part_index == TRANSACTIONS_LEN) 
	{
		match_enqueue(omp_get_thread_num(), trans_id, rules[begin][RULES_LEN - 1]);
		return;
	}

	value = transactions[trans_id][part_index];

	zero_first = rules[begin][part_index] == WILDCARD ? begin : NOT_FOUND;
	zero_last = zero_first != NOT_FOUND ? bounded_search_last_rule_match(WILDCARD, part_index, zero_first != NOT_FOUND ? zero_first : begin, end) : NOT_FOUND;

	value_first = bounded_search_first_rule_match(value, part_index, zero_last != NOT_FOUND ? zero_last : begin, end);
	value_last = value_first != NOT_FOUND ? bounded_search_last_rule_match(value, part_index, value_first, end) : NOT_FOUND;

	if (zero_last != NOT_FOUND)
		match_bounded_search_transaction(trans_id, part_index + 1, zero_first, zero_last);

	if (value_last != NOT_FOUND)
		match_bounded_search_transaction(trans_id, part_index + 1, value_first, value_last);

}

void match_bounded_search()
{
	int i;
	int n_cores = omp_get_num_procs();
	pthread_t thread_worker_ids[9]; /* TODO: move */

	work_queue.index = 0;
	pthread_mutex_init(&work_queue.mutex, NULL); /* TODO: move */

	match_dequeuer_start();

	for (i = 0; i < n_cores; i++)
		pthread_create(&thread_worker_ids[i], NULL, thread_match_bounded_search_worker, NULL);

	match_dequeuer_join();
	for (i = 0; i < n_cores; i++)
		pthread_join(thread_worker_ids[i], NULL);

}

void *thread_match_bounded_search_worker()
{
	int trans_id;

	while (true)
	{
		pthread_mutex_lock(&work_queue.mutex);
			if (work_queue.index > n_transactions)
			{
				pthread_mutex_unlock(&work_queue.mutex);
				break;
			}

			trans_id = work_queue.index++;
		pthread_mutex_unlock(&work_queue.mutex);

		match_bounded_search_transaction(trans_id, 0, 0, n_rules);
	}

	pthread_mutex_lock(&output.mutex);
		output.closed = true;
		pthread_cond_broadcast(&output.empty);
	pthread_mutex_unlock(&output.mutex);

	pthread_exit(0);
}
