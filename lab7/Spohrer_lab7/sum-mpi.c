//----------------------------------------------------------------------------- 
// Program code for CS 415/515 Parallel Programming, Portland State University.
//----------------------------------------------------------------------------- 

// The sum program (MPI version).
//
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mpi.h>

#define N 64 	/* problem parameter */

//int compute(int i) { return i*i; }

int main(int argc, char **argv) {  
  int rank, size;
  MPI_File fh; 
  MPI_Status st;
  MPI_Init(&argc, &argv);  
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);  
  MPI_Comm_size(MPI_COMM_WORLD, &size);  
  int mycnt = N/size;
  int * a = malloc((mycnt)*sizeof(int));
  memset(a,0,sizeof(int)*mycnt);
  int offset;
  int low = (N/size) * rank;    /* a simplistic partition scheme */ 
  int high = low + (N/size);	
  int psum = 0;
  //for (int i = low; i < high; i++)
    //psum += compute(i);

  if(argc < 2) {
    if(rank==0)
      printf("Usage: mpirun -n <#procs> sum-mpi <filename>\n");
    MPI_Finalize();
    free(a);
    return 0;
  }

  MPI_File_open(MPI_COMM_WORLD, argv[1], MPI_MODE_RDONLY, MPI_INFO_NULL, &fh);
  offset = rank * mycnt * sizeof(int);  
  MPI_File_set_view(fh, offset, MPI_INT, MPI_INT, "native", MPI_INFO_NULL);
  MPI_File_read(fh, a, mycnt, MPI_INT, &st);
  for(int i = 0; i < mycnt; ++i) 
    psum += a[i];

  int sum=0, dest=0;
  MPI_Reduce(&psum, &sum, 1, MPI_INT, MPI_SUM, dest, MPI_COMM_WORLD);
  if (rank == dest)
    printf("The sum is %d (should be 332833500) (the total here is different "
        "from what's in my data.txt, but it all adds up.\n", sum);
  MPI_File_close(&fh);

  free(a);
  MPI_Finalize();
}  
