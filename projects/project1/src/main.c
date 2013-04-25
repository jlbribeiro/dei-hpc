/*
 * High Performance Computing
 * April 2013
 * Project 1
 *
 * João Paulo Batista Ferreira	2009113274	jpbat@student.dei.uc.pt	
 * José Luís Baia Ribeiro		2008112181	jbaia@student.dei.uc.pt
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include <omp.h>

#include "inc/utils.h"

#include "inc/constants.h"
#include "inc/input.h"
#include "inc/output.h"
#include "inc/naive.h"
#include "inc/bounded_search.h"

#define TIME_MEASURES

int n_rules;
int n_transactions;

int rules[MAX_SIZE][RULES_LEN];
int transactions[MAX_SIZE][TRANSACTIONS_LEN];

match_output_buffer_t output;
pthread_t thread_match_dequeuer_id;

work_queue_t work_queue;

int main(int argc, char** argv)
{
	struct timeval start, end;

	if (argc < 3)
	{
		fprintf(stderr, "Wrong usage!\n");
		fprintf(stderr, "%s <transactions_filename> <rules_filename>\n", argv[0]);
		exit(0);
	}

	START_CHRONO;
	#pragma omp parallel sections
	{
		#pragma omp section
		{
			n_transactions = read_transanctions(argv[1]);
		}

		#pragma omp section
		{
			n_rules = read_rules(argv[2]);
		}
	}

	sort_rules();

	#ifdef DEBUG_PRINT
		fprintf(stderr, "\n");
	#endif
	/*/
	show_transactions(n_transactions);
	show_rules(n_rules);
	/*/

	/* match_naive(); */
	/* match_bounded_search(); */

	match_bounded_search();
	STOP_CHRONO;

	#ifdef TIME_MEASURES
		fprintf(stderr, "\n[Bounded] Took %ld ms to match %d transactions (%.2lf transactions per second)!\n", GET_CHRONO, n_transactions, n_transactions * 1000 / (GET_CHRONO + 0.001));
	#endif

	return 0;
}
