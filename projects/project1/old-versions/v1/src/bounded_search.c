#include <stdio.h>

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
		output_match(trans_id, begin);
		return;
	}

	value = transactions[trans_id][part_index];

	zero_first = bounded_search_first_rule_match(WILDCARD, part_index, begin, end);
	zero_last = zero_first != NOT_FOUND ? bounded_search_last_rule_match(WILDCARD, part_index, zero_first != NOT_FOUND ? zero_first : begin, end) : NOT_FOUND;

	value_first = bounded_search_first_rule_match(value, part_index, zero_last != NOT_FOUND ? zero_last : begin, end);
	value_last = value_first != NOT_FOUND ? bounded_search_last_rule_match(value, part_index, value_first, end) : NOT_FOUND;

	if (zero_last != NOT_FOUND)
		match_bounded_search_transaction(trans_id, part_index + 1, zero_first, zero_last);

	if (value_last != NOT_FOUND)
		match_bounded_search_transaction(trans_id, part_index + 1, value_first, value_last);

	/* fprintf(stderr, "zero_first: %d, zero_last: %d, value_first: %d, value_last: %d\n", zero_first, zero_last, value_first, value_last); */
}

void match_bounded_search()
{
	int i;
	struct timeval start, end;

	START_CHRONO;

	for (i = 0; i < n_transactions; i++)
		match_bounded_search_transaction(i, 0, 0, n_rules);

	STOP_CHRONO;

	#ifdef DEBUG_PRINT
		fprintf(stderr, "\n[Bounded] Took %ld ms to match %d transactions (%.2lf transactions per second)!\n", GET_CHRONO, n_transactions, n_transactions * 1000 / (GET_CHRONO + 0.001));
	#endif
}