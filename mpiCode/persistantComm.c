#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>

#define MASTER 0
#define PARTNER 1
#define TRIALS 10
#define MAX_MSGSIZE 1

int main (int argc, char *argv[])
{
char hostname[MPI_MAX_PROCESSOR_NAME];
int rank, numtasks, count, sbuff, rbuff, len, i;
MPI_Status status;
MPI_Request recv_obj;
MPI_Request send_obj;

MPI_Init(&argc, &argv);
MPI_Comm_size(MPI_COMM_WORLD, &numtasks);
MPI_Comm_rank(MPI_COMM_WORLD, &rank);
MPI_Get_processor_name(hostname, &len);

if (numtasks != 2)
{
        if (rank == MASTER)
                printf("Quitting. Programs needs 2 tasks: numtasks=%d\n", numtasks);
}
else
{
	if (rank == MASTER){
	//Initializing Request
		MPI_Recv_init (&rbuff, MAX_MSGSIZE, MPI_INT, PARTNER, 100, MPI_COMM_WORLD, &recv_obj);
		MPI_Send_init (&sbuff, MAX_MSGSIZE, MPI_INT, PARTNER, 101, MPI_COMM_WORLD, &send_obj);
		
		for (i = 0; i < TRIALS; i++){
		//Start recieve operation
			MPI_Start(&recv_obj);
		//Work
			count = i + 1;
			sbuff = i + 1;
		//Start send operation
			MPI_Start(&send_obj);
		//Wait for operations to complete
			MPI_Wait(&send_obj, &status);
			MPI_Wait(&recv_obj, &status);
		//Show resul
			printf("Task %d has recieved %d messages on trial %d from node %s \n", MASTER, rbuff, i, hostname);
		}
	//Clean up request
		MPI_Request_free (&recv_obj);
		MPI_Request_free (&send_obj);
	}
	else {
	//Initializing Request
		MPI_Recv_init (&rbuff, MAX_MSGSIZE, MPI_INT, MASTER, 101, MPI_COMM_WORLD, &recv_obj);
                MPI_Send_init (&sbuff, MAX_MSGSIZE, MPI_INT, MASTER, 100, MPI_COMM_WORLD, &send_obj);
		
		for (i = 0; i < TRIALS; i++){
			//Start recieve operation
			MPI_Start(&recv_obj);
			//Work
			count = i + 1;
                        sbuff = i + 1;
			//Start send operation
			MPI_Start(&send_obj);
			//Wait for operations to complete
			MPI_Wait(&send_obj, &status);
                        MPI_Wait(&recv_obj, &status);
			//Show result
			printf("Task %d has recieved %d messages on trial %d from node %s \n", PARTNER, rbuff, i, hostname);
		}	
	//Clean up request
		MPI_Request_free (&recv_obj);
                MPI_Request_free (&send_obj);
	}
}
MPI_Finalize();
return 0;
}
