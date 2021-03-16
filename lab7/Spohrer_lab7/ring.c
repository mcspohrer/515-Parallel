#define _BSD_SOURCE
#include <unistd.h>	// for gethostname()
#include <stdlib.h>
#include <stdio.h>
#include <mpi.h>

#define TAG 1001

int main(int argc, char *argv[])
{
  int nprocs, rank;
  char host[20];
  MPI_Status status;
  int N = 10;		// default integer value
  
  if (argc == 2) {
    N = atoi(argv[1]);	// overwirte the value
  }
  gethostname(host, 20);

  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &nprocs);
  if (nprocs < 2) {
    printf("Need at least 2 processes.\n");
    MPI_Finalize();
    return(1);
  }
  printf("P%d of %d started on %s ...\n", rank, nprocs, host);

  if (rank == 0) {
    MPI_Send(&N, 1, MPI_INT, rank+1, TAG, MPI_COMM_WORLD);
    printf("P0 sent %d to P1\n", N);
  } 
  else if (rank > 0 && rank < nprocs-1) {
    MPI_Recv(&N, 1, MPI_INT, rank-1, TAG, MPI_COMM_WORLD, &status);
    printf("P%d recieved %d from P%d\n",rank, N, rank-1);
    N--;
    MPI_Send(&N, 1, MPI_INT, rank+1, TAG, MPI_COMM_WORLD);
    printf("P%d sent %d to P%d\n",rank, N, rank+1);
  } else {
    MPI_Recv(&N, 1, MPI_INT, rank-1, TAG, MPI_COMM_WORLD, &status);
    printf("P%d recieved %d from P%d\n",rank, N, rank-1);
  }

  MPI_Finalize();
}
