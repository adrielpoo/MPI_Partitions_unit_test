#include "mpi.h"
#include "stdio.h"
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#define MASTER 0
#define DATA_LENGTH 10

int main(int argc, char *argv[])
{
char hostname[MPI_MAX_PROCESSOR_NAME];
void *buffer;
int data[DATA_LENGTH];
int rank, numtasks, size, len, i;
MPI_Status status;

MPI_Init(&argc, &argv);
MPI_Comm_rank(MPI_COMM_WORLD, &rank);
MPI_Comm_size(MPI_COMM_WORLD, &numtasks);
MPI_Get_processor_name(hostname, &len);

//Checking that there is only two task 
if (numtasks != 2) 
{
	if (rank == MASTER) 
		printf("Quitting. Programs needs 2 tasks: numtasks=%d\n", numtasks);
}
else 
{
	printf("Task %d on node %s starting...\n", rank, hostname);
//Master Process Sends Two messages	
	if (rank == MASTER){
        	//Determining size of buffer needed & Attaching Buffer
		MPI_Pack_size(DATA_LENGTH, MPI_INT, MPI_COMM_WORLD, &size);
		size = size + MPI_BSEND_OVERHEAD;
		printf("Buffer size: %d Overhead: %d \n", size, MPI_BSEND_OVERHEAD);
		buffer = malloc(size);
		MPI_Buffer_attach(buffer, size);

		//Setting Data for first Send call
		for (i = 0; i < DATA_LENGTH; i++)
			data[i] = i;
        	MPI_Bsend(data, DATA_LENGTH, MPI_INT, 1, 100, MPI_COMM_WORLD);
		//Setting Data for second Send call
		for (i = 0; i < DATA_LENGTH; i++)
			data[i] = 5;
		MPI_Bsend(data, DATA_LENGTH, MPI_INT, 1, 101, MPI_COMM_WORLD);
		
		//Detach Buffer
		MPI_Buffer_detach(&buffer, &size);
		free(buffer);
		}
//Worker recieve two messages
	if (rank != MASTER) {
		for (i = 0; i < DATA_LENGTH ; i++)
       			data[i] = 0;
		//First Recv call and output
		MPI_Recv(data, DATA_LENGTH, MPI_INT, MASTER, 100, MPI_COMM_WORLD, &status);
		printf("\nTask %d Tag %d recieved:", rank, status.MPI_TAG);
		for (i = 0; i < DATA_LENGTH; i++){
			assert(data[i] == i);
			printf(" %d,", data[i]);
		}				
		MPI_Recv(data, DATA_LENGTH, MPI_INT, MASTER, 101, MPI_COMM_WORLD, &status);
		printf("\nTask %d Tag %d recieved:", rank, status.MPI_TAG);
		for (i = 0; i < DATA_LENGTH; i++){
			assert(data[i] == 5);
			printf(" %d,", data[i]);
		}
		printf("\n");
		}
}
MPI_Finalize();
return 0;
}

