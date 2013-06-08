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

void bounded_search_transaction_match(thread_output_buffer_t *thread_buffer, int trans_id, int part_index, int begin, int end)
{
	int value, zero_first, zero_last, value_first, value_last;

	if (part_index == TRANSACTIONS_LEN) 
	{
		output_match_to_thread_buffer(thread_buffer, trans_id, rules[begin][RULES_LEN - 1]);
		return;
	}

	value = transactions[trans_id][part_index];

	zero_first = rules[begin][part_index] == WILDCARD ? begin : NOT_FOUND;
	zero_last = zero_first != NOT_FOUND ? bounded_search_last_rule_match(WILDCARD, part_index, zero_first != NOT_FOUND ? zero_first : begin, end) : NOT_FOUND;

	value_first = bounded_search_first_rule_match(value, part_index, zero_last != NOT_FOUND ? zero_last : begin, end);
	value_last = value_first != NOT_FOUND ? bounded_search_last_rule_match(value, part_index, value_first, end) : NOT_FOUND;

	if (zero_last != NOT_FOUND)
		bounded_search_transaction_match(thread_buffer, trans_id, part_index + 1, zero_first, zero_last);

	if (value_last != NOT_FOUND)
		bounded_search_transaction_match(thread_buffer, trans_id, part_index + 1, value_first, value_last);

}

void bounded_search_match()
{
	int i;
	int n_cores = MAX_CORES;

	pthread_t thread_worker_ids[MAX_CORES];

	for (i = 0; i < n_cores; i++)
		pthread_create(&thread_worker_ids[i], NULL, thread_match_bounded_search_worker, NULL);

	for (i = 0; i < n_cores; i++)
		pthread_join(thread_worker_ids[i], NULL);

}

void *thread_match_bounded_search_worker(void *arg)
{
	int i;
	int begin, end;

	thread_output_buffer_t self_buffer;
	self_buffer.length = 0;

	while (true)
	{
		pthread_mutex_lock(&work_queue.mutex);
			if (work_queue.index > work_queue.last_index)
			{
				pthread_mutex_unlock(&work_queue.mutex);
				break;
			}

			begin = work_queue.index;
			work_queue.index += WORK_BATCH_SIZE;
		pthread_mutex_unlock(&work_queue.mutex);

		end = begin + WORK_BATCH_SIZE;
		if (end > work_queue.last_index)
			end = work_queue.last_index;

		for (i = begin; i < end; i++)
			bounded_search_transaction_match(&self_buffer, i, 0, 0, n_rules);

	}

	if (self_buffer.length)
		output_thread_buffer_to_master(&self_buffer);

	pthread_exit(0);
}
