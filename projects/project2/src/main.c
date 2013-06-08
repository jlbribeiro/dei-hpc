/*
 * High Performance Computing
 * June 2013
 * Project 2
 *
 * João Paulo Batista Ferreira	2009113274	jpbat@student.dei.uc.pt	
 * José Luís Baia Ribeiro		2008112181	jbaia@student.dei.uc.pt
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include <omp.h>
#include <mpi.h>

#include "inc/utils.h"

#include "inc/constants.h"
#include "inc/mpi_nodes.h"
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

int mpi_self_rank;
int mpi_n_processes;

int main(int argc, char** argv)
{
	if (argc < 4)
	{
		fprintf(stderr, "Wrong usage!\n");
		fprintf(stderr, "%s <transactions_filename> <rules_filename> <output_filename>\n", argv[0]);
		exit(0);
	}

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &mpi_self_rank);
	MPI_Comm_size(MPI_COMM_WORLD, &mpi_n_processes);

	if (mpi_self_rank == MPI_RANK_MASTER)
		mpi_nodes_master(argv);
	else
		mpi_nodes_slave(argv);

	MPI_Finalize();

	return 0;
}
