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

work_queue_t work_queue;
pthread_mutex_t output_mutex;

FILE *output_fd;

int main(int argc, char** argv)
{
	struct timeval start, end;

	long input, sort, match, total;

	if (argc < 4)
	{
		fprintf(stderr, "Wrong usage!\n");
		fprintf(stderr, "%s <transactions_filename> <rules_filename> <output_filename>\n", argv[0]);
		exit(0);
	}

	START_CHRONO;
		work_queue.index = 0;
		pthread_mutex_init(&(work_queue.mutex), NULL);
		pthread_mutex_init(&output_mutex, NULL);

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
	STOP_CHRONO;
	input = GET_CHRONO;
	fprintf(stderr, "Reading files:\t%ld (ms)\n", input);

		output_fd = fopen(argv[3], "w");
	START_CHRONO;
		sort_rules();
	STOP_CHRONO;
	sort = GET_CHRONO;
	fprintf(stderr, "Sorting:\t%ld (ms)\n", sort);

	START_CHRONO;
		bounded_search_match();
	STOP_CHRONO;
	match = GET_CHRONO;
	fprintf(stderr, "Matching:\t%ld (ms)\n", match);

	fclose(output_fd);

	total = input + sort + match;

	fprintf(stderr, "Total:\t\t%ld ms to match %d transactions (%.2lf transactions per second)!\n", total, n_transactions, n_transactions * 1000 / total * 1.0);

	return 0;
}
