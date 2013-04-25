#include <omp.h>

#include "inc/naive.h"
#include "inc/utils.h"
#include "inc/output.h"

void match_naive()
{
	int i, j, k;
	struct timeval start, end;

	#ifdef DEBUG_PRINT
		fprintf(stderr, "Naïve matches:\n");
	#endif

	START_CHRONO;

	#pragma omp for
	for (i = 0; i < n_transactions; i++)
	{
		for (j = 0; j < n_rules; j++)
		{
			for (k = 0; k < TRANSACTIONS_LEN; k++)
			{
				if (transactions[i][k] != rules[j][k] && rules[j][k] != WILDCARD)
					break;

			}

			if (k == TRANSACTIONS_LEN)
				output_match(i, j);

		}
	}

	STOP_CHRONO;

	#ifdef DEBUG_PRINT
		fprintf(stderr, "\n[Naïve] Took %ld ms to match %d transactions (%.2lf transactions per second)!\n", GET_CHRONO, n_transactions, n_transactions * 1000 / (GET_CHRONO + 0.001));
	#endif
}
