#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>
#define  MASTER		0

int main (int argc, char *argv[])
{
	int  numtasks, rank, len, partner, message;
	char hostname[MPI_MAX_PROCESSOR_NAME];
	MPI_Status status;

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &numtasks);

	/* need an even number of tasks  */
	if (numtasks % 2 != 0) {
	   if (rank == MASTER) 
	      printf("Quitting. Need an even number of tasks: numtasks=%d\n", numtasks);
	   } 

	else {
	   if (rank == MASTER) 
	      printf("MASTER: Number of MPI tasks is: %d\n",numtasks);

	   MPI_Get_processor_name(hostname, &len);
	   printf ("Hello from task %d on %s!\n", rank, hostname);

	   /* determine partner and then send/receive with partner */
	   if (rank < numtasks/2) {
	     partner = numtasks/2 + rank;
	     MPI_Send(&rank, 1, MPI_INT, partner, 1, MPI_COMM_WORLD);
	     MPI_Recv(&message, 1, MPI_INT, partner, 1, MPI_COMM_WORLD, &status);
	     }
	   else if (rank >= numtasks/2) {
	     partner = rank - numtasks/2;
	     MPI_Recv(&message, 1, MPI_INT, partner, 1, MPI_COMM_WORLD, &status);
	     MPI_Send(&rank, 1, MPI_INT, partner, 1, MPI_COMM_WORLD);
	     }

	   /* print partner info and exit*/
	   printf("Task %d is partner with %d\n",rank,message);
	   }

	MPI_Finalize();

}

