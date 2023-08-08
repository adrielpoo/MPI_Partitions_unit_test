#include "mpi.h"
#include "stdio.h"
#include <string.h>
#define MASTER 0

int main(int argc, char *argv[])
{
char message[20];
char hostname[MPI_MAX_PROCESSOR_NAME];
int rank, numtasks, len;
MPI_Status status;

MPI_Init(&argc, &argv);
MPI_Comm_rank(MPI_COMM_WORLD, &rank);
MPI_Comm_size(MPI_COMM_WORLD, &numtasks);
MPI_Get_processor_name(hostname, &len);

printf("Task %d on node %s starting...\n", rank, hostname);

if (rank == MASTER){
	strcpy(message, "Hello World");
	MPI_Send(message, strlen(message)+1, MPI_CHAR, 1, 7, MPI_COMM_WORLD);		
	}
if (rank != MASTER) {
	MPI_Recv(message, 20, MPI_CHAR, MASTER, 7, MPI_COMM_WORLD, &status);
	printf("recieved: %s \n", message);
	}

MPI_Finalize();
return 0;		
}
