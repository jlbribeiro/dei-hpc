#include <stdlib.h>
#include <string.h>
#include <mpi.h>

#include "inc/mpi_nodes.h"

#include "inc/globals.h"
#include "inc/input.h"
#include "inc/output.h"
#include "inc/utils.h"

#include "inc/bounded_search.h"

void mpi_nodes_master(char** argv)
{
	int i;

	thread_output_matches_t matches;

	MPI_Status status;

	pthread_mutex_init(&output_mutex, NULL);
	output_fd = fopen(OUTPUT_FILENAME, "w");

	n_transactions = read_transactions(TRANSACTIONS_FILENAME);

	for (i = 1; i < mpi_n_processes; )
	{
		MPI_Recv(&matches.matches, THREAD_OUTPUT_MATCHES_SIZE, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);

		matches.length = matches.matches[0];

		if (!matches.length)
		{
			i++;
			continue;
		}

		output_thread_matches_to_file(&matches);
	}

	fclose(output_fd);
}

void mpi_nodes_slave(char** argv)
{
	int batch_size;
	int transaction_begin_offset, transaction_end_offset;
	int end_value;

	#pragma omp parallel sections
	{
		#pragma omp section
		{
			n_transactions = read_transactions(TRANSACTIONS_FILENAME);
		}

		#pragma omp section
		{
			n_rules = read_rules(RULES_FILENAME);
		}
	}

	sort_rules();

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
	pthread_mutex_init(&output_mutex, NULL);

	bounded_search_match();

	end_value = 0;
	MPI_Send(&end_value, 1, MPI_INT, MPI_RANK_MASTER, mpi_self_rank, MPI_COMM_WORLD);
}