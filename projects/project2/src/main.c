#include <mpi.h>
#include <stdio.h>
#include <string.h>

#define MSG_BUFFER_SIZE 32
#define N_PROCESSES 9

int main (int argc, char **argv)
{
	char message[MSG_BUFFER_SIZE];
	int my_rank;

	int dest_rank, master_rank = 0;

	int message_tag = 1;

	MPI_Status status;
	MPI_Init(&argc, &argv);
	
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

	if (my_rank == master_rank)
	{
		/* process 0 */
		for (dest_rank = 1; dest_rank < N_PROCESSES; dest_rank++)
		{
			sprintf(message, "%d says: \"Hello, process %d!\"", master_rank, dest_rank);
			MPI_Send(message, strlen(message) + 1, MPI_CHAR, dest_rank, message_tag, MPI_COMM_WORLD);

			printf("[%d] Sent to %d: %s\n", my_rank, dest_rank, message);
		}
	}
	else
	{
		/* process 1-8 */
		MPI_Recv(message, MSG_BUFFER_SIZE, MPI_CHAR, master_rank, message_tag, MPI_COMM_WORLD, &status);
		printf("[%d] Received: %s\n", my_rank, message);
	}

	MPI_Finalize();

	return 0;
}
