//----------------------------------------------------------------------------- 
// Program code for CS 415/515 Parallel Programming, Portland State University.
//----------------------------------------------------------------------------- 

// The vector sum program (MPI version).
//
//
#include <stdlib.h>
#include <stdio.h>
#include <mpi.h>

int main(int argc, char **argv) { 
  int N, rank, size, myn, *vector;

  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank); 
  MPI_Comm_size(MPI_COMM_WORLD, &size); 

  /* The root process reads input and initializes data */
  if (rank == 0) {
    printf("Enter the vector length: ");
    fflush(stdout);
    scanf("%d", &N); 
    vector = (int *) malloc(sizeof(int) * N);
    for (int i = 0; i < N; i++)
      vector[i] = i + 1;
    myn = N / size;
  }

  /* Broadcast size parameter and scatter the vector */
  MPI_Bcast(&myn, 1, MPI_INT, 0, MPI_COMM_WORLD);
  int *myvec = (int *) malloc(sizeof(int)*myn);
  MPI_Scatter(vector, myn, MPI_INT, myvec, myn, MPI_INT, 0, MPI_COMM_WORLD);

  /* Find the local sum, then the global sum of the vectors */
  int mysum = 0;
  for (int i = 0; i < myn; i++)
    mysum += myvec[i];

  if (rank == 0)
    printf("After reduce mysum: %d\n", mysum);

  int total = 0;
  MPI_Reduce(&mysum, &total, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

  /* Multiply local part by (rank + 1), then gather the result */
  for (int i = 0; i < myn; i++) 
    myvec[i] *= (rank + 1);
  MPI_Gather(myvec, myn, MPI_INT, vector, myn, MPI_INT, 0, MPI_COMM_WORLD);

  /* The root process prints out the result */
  if (rank == 0) {
    printf("[%d] total = %d, vector: ", rank, total);
    for (int i = 0; i < N; i++)
      printf("%d,", vector[i]);
    printf("\n");
  }

  MPI_Finalize();
} 
