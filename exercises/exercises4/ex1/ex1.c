#include <mpi.h>
#include <stdio.h>
#include <string.h>

#define MSG_BUFFER_SIZE 32

int main (int argc, char **argv)
{
	char message[MSG_BUFFER_SIZE];
	int my_rank;

	MPI_Status status;
	MPI_Init(&argc, &argv);

	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

	if (my_rank == 0)
	{
		/* process 0 */
		strcpy(message, "Hello World!");
		MPI_Send(message, strlen(message) + 1, MPI_CHAR, 1, 99, MPI_COMM_WORLD);
		printf("[%d] Sent: %s\n", my_rank, message);
	}
	else
	{
		/* process 1 */
		MPI_Recv(message, MSG_BUFFER_SIZE, MPI_CHAR, 0, 99, MPI_COMM_WORLD, &status);
		printf("[%d] Received: %s\n", my_rank, message);
	}

	MPI_Finalize();

	return 0;
}
