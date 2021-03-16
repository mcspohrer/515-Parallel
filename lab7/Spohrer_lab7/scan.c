#include <stdlib.h>
#include <stdio.h>
#include <mpi.h>

int main(int argc, char **argv) {
  int rank, size; 
  int *rec;

  MPI_Init(&argc, &argv);  
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);  
  MPI_Comm_size(MPI_COMM_WORLD, &size);  
  rec = malloc(size * sizeof(int));

  // sum-scan the rank numbers
  int psum;
  MPI_Scan(&rank, &psum, 1, MPI_INT, MPI_SUM, MPI_COMM_WORLD);
  printf("P[%d] psum = %d\n", rank, psum);

  MPI_Gather(&psum, 1, MPI_INT, rec, 1, MPI_INT, 0, MPI_COMM_WORLD);
  if(rank==0) {
    for(int i = 0; i < size; ++i) {
      printf("%d,", rec[i]);
    }
    printf("\n");
  }

  MPI_Finalize();
}  
