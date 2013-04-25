#include <stdio.h>
#include <stdlib.h>

#include "inc/input.h"
#include "inc/utils.h"

int cmp_rules(const void *a, const void *b)
{
	int i;
	int diff;

	int *at = (int *)a;
	int *bt = (int *)b;

	for (i = 0; i < RULES_LEN; i++)
	{
		diff = at[i] - bt[i];

		if (diff != 0)
			return diff;

	}

	return 0;
}

int read_rules(char *filename)
{
	struct timeval start, end;

	int i = 0;

	int j, k;
	int begin;
	int rule_part;

	int tenFactor;

	char rule_buffer[RULE_BUF_SIZE];

	FILE *f = fopen(filename, "r");

	START_CHRONO;
	while (fgets(rule_buffer, RULE_BUF_SIZE, f) != NULL)
	{
		j = 0;

		for (rule_part = 0; rule_part < RULES_LEN; rule_part++)
		{
			begin = j - 1;

			rules[i][rule_part] = 0;
			tenFactor = 1;

			/* scan the next comma, or stop in case of a \n or \0 */
			for (; rule_buffer[j] != ',' && rule_buffer[j] != '\n' && rule_buffer[j] != '\0'; j++);

			k = j - 1;

			for (k = j - 1; k > begin; k--)
			{
				rules[i][rule_part] += (rule_buffer[k] - '0') * tenFactor;
				tenFactor *= 10;
			}

			j++;
		}

		i++;
	}
	STOP_CHRONO;

	fclose(f);

	#ifdef DEBUG_PRINT
		fprintf(stderr, "Took %ld ms to read %d rules!\n", GET_CHRONO, i);
	#endif

	return i;
}

int read_transanctions(char *filename)
{
	struct timeval start, end;

	int i = 0;

	int j, k;
	int begin;
	int transaction_part;

	int tenFactor;

	char transaction_buffer[TRANSACTION_BUF_SIZE];

	FILE *f = fopen(filename, "r");

	START_CHRONO;
	while (fgets(transaction_buffer, TRANSACTION_BUF_SIZE, f) != NULL)
	{
		j = 0;

		for (transaction_part = 0; transaction_part < TRANSACTIONS_LEN; transaction_part++)
		{
			begin = j - 1;

			transactions[i][transaction_part] = 0;
			tenFactor = 1;

			/* scan the next comma, or stop in case of a \n or \0 */
			for (; transaction_buffer[j] != ',' && transaction_buffer[j] != '\n' && transaction_buffer[j] != '\0'; j++);

			k = j - 1;

			for (k = j - 1; k > begin; k--)
			{
				transactions[i][transaction_part] += (transaction_buffer[k] - '0') * tenFactor;
				tenFactor *= 10;
			}

			j++;
		}

		i++;
	}
	STOP_CHRONO;

	fclose(f);

	#ifdef DEBUG_PRINT
		fprintf(stderr, "Took %ld ms to read %d transactions!\n", GET_CHRONO, i);
	#endif

	return i;
}

void sort_rules()
{
	struct timeval start, end;

	START_CHRONO;
	qsort(rules, n_rules, sizeof(int) * RULES_LEN, cmp_rules);
	STOP_CHRONO;

	#ifdef DEBUG_PRINT
		fprintf(stderr, "Took %ld ms to sort %d rules!\n", GET_CHRONO, n_rules);
	#endif
}
