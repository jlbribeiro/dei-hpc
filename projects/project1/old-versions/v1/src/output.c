#include <stdio.h>

#include "inc/output.h"
#include "inc/globals.h"

void output_match(int transaction_id, int rule_id)
{
	int i;

	for (i = 0; i < TRANSACTIONS_LEN; i++)
	{
		printf("%d,", transactions[transaction_id][i]);

		#ifdef DEBUG_PRINT
			fprintf(stderr, "%d,", transactions[transaction_id][i]);
		#endif
	}

	printf("%d\n", rules[rule_id][RULES_LEN - 1]);

	#ifdef DEBUG_PRINT
		fprintf(stderr, "%d\n", rules[rule_id][RULES_LEN - 1]);
	#endif
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
