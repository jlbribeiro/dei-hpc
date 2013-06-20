#include <stdio.h>
#include <mpi.h>

#define N 5000000
#define TOTAL_N N * 2 /* N - (-N) */
#define INPUT_FILE argv[1]

#define MPI_RANK_MASTER 0
#define N_PROCESSES 4

int mpi_self_rank;
int mpi_n_processes;

int numbers[TOTAL_N];
int recv_numbers[TOTAL_N / N_PROCESSES];
int n_numbers_per_process;

int maxs[N_PROCESSES];
int max;

int main(int argc, char **argv)
{
	int i;
	FILE *input_fd;

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &mpi_self_rank);
	MPI_Comm_size(MPI_COMM_WORLD, &mpi_n_processes);

	/* MASTER ONLY */
	if (mpi_self_rank == MPI_RANK_MASTER)
	{
		input_fd = fopen(INPUT_FILE, "r");

		for (i = 0; i < TOTAL_N; i++)
			fscanf(input_fd, "%d", &numbers[i]);

		fclose(input_fd);
	}

	n_numbers_per_process = TOTAL_N / mpi_n_processes;

	MPI_Scatter(numbers, n_numbers_per_process, MPI_INT, &recv_numbers, n_numbers_per_process, MPI_INT, MPI_RANK_MASTER, MPI_COMM_WORLD);

	max = recv_numbers[0];
	for (i = 1; i < n_numbers_per_process; i++)
		if (recv_numbers[i] > max)
			max = recv_numbers[i];

	MPI_Gather(&max, 1, MPI_INT, maxs, 1, MPI_INT, 0, MPI_COMM_WORLD);

	/* MASTER ONLY */
	if (mpi_self_rank == MPI_RANK_MASTER)
	{
		max = maxs[0];
		for (i = 1; i < mpi_n_processes; i++)
			if (maxs[i] > max)
				max = maxs[i];

		printf("Expected max = %d\n", N - 1);
		printf("Max found    = %d\n", max);
	}

	MPI_Finalize();

	return 0;
}