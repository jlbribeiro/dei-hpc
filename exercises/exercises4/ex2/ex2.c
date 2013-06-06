#include <mpi.h>
#include <stdio.h>
#include <string.h>

#define MSG_BUFFER_SIZE 5

void transform(float *a, int size, char *buffer)
{
	int i;

	for (i = 0; i < size; i++)
	{
		printf("%.3f ", a[i]);
		sprintf(buffer, "%.3f ", a[i]);
	}
	buffer[strlen(buffer)] = '\0';
}

int main (int argc, char **argv)
{
	int my_rank, i;
	char buffer[MSG_BUFFER_SIZE * 10];
	float buf1[MSG_BUFFER_SIZE], buf2[MSG_BUFFER_SIZE];
	MPI_Request req1, req2;

	MPI_Init(&argc, &argv);

	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

	if (my_rank == 0)
	{
		for (i = 1; i <= MSG_BUFFER_SIZE; i++)
		{
			buf1[i-1] = i;
			buf2[i-1] = i * i;
		}
		MPI_Isend(buf1, MSG_BUFFER_SIZE, MPI_FLOAT, 1, 0, MPI_COMM_WORLD, &req1);
		MPI_Isend(buf2, MSG_BUFFER_SIZE, MPI_FLOAT, 1, 0, MPI_COMM_WORLD, &req2);

		transform((float*)buf1, MSG_BUFFER_SIZE, &buffer[0]);
		printf("[%d] Sent: %s\n", my_rank, buffer);

		transform(buf2, MSG_BUFFER_SIZE, &buffer[0]);
		printf("[%d] Sent: %s\n", my_rank, buffer);
	}
	else
	{
		MPI_Irecv(buf1, MSG_BUFFER_SIZE, MPI_FLOAT, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &req1);
		MPI_Irecv(buf2, MSG_BUFFER_SIZE, MPI_FLOAT, 0, 0, MPI_COMM_WORLD, &req2);

		transform(buf1, MSG_BUFFER_SIZE, &buffer[0]);
		printf("[%d] Received: %s\n", my_rank, buffer);

		transform(buf2, MSG_BUFFER_SIZE, &buffer[0]);
		printf("[%d] Received: %s\n", my_rank, buffer);
	}

	MPI_Finalize();

	return 0;
}
