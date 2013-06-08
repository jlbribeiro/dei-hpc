#include "inc/mpi_nodes.h"

#include "inc/globals.h"
#include "inc/input.h"
#include "inc/utils.h"

#include "inc/bounded_search.h"

void mpi_nodes_master(char** argv)
{
	n_rules = read_rules(RULES_FILENAME);
	sort_rules();

	/*
		output_fd = fopen(OUTPUT_FILENAME, "w");
		fclose(output_fd);
	*/
}

void mpi_nodes_slave(char** argv)
{
	int batch_size;
	int transaction_begin_offset, transaction_end_offset;

	n_rules = read_rules(RULES_FILENAME);
	sort_rules();

	n_transactions = read_transactions(TRANSACTIONS_FILENAME);

	if (n_transactions > mpi_n_processes)
	{
		batch_size = (n_transactions / (mpi_n_processes - 1));

		transaction_begin_offset = batch_size * (mpi_self_rank - 1);
		transaction_end_offset = batch_size * mpi_self_rank;

		if (transaction_end_offset > n_transactions || mpi_self_rank + 1 == mpi_n_processes)
			transaction_end_offset = n_transactions;

	} else
	{
		/* Only the first working node will perform the calculations */
		if (mpi_self_rank != 1)
			return;

		transaction_begin_offset = 0;
		transaction_end_offset = n_transactions;
	}

	work_queue.index = transaction_begin_offset;
	work_queue.last_index = transaction_end_offset;
	pthread_mutex_init(&(work_queue.mutex), NULL);

	bounded_search_match();

	/* TODO: send results */
}
