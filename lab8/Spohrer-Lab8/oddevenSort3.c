//----------------------------------------------------------------------------- 
// Program code for CS 415P/515 Parallel Programming, Portland State University
//----------------------------------------------------------------------------- 

// An odd-even sort program (C sequential version)
//
// Usage: 
//   linux> ./oddevenSort N
// 
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/time.h>
#include <mpi.h>

int *a;           // array to be sorted
int N = 1;        // array size
int sorted = 0;

// Swap two array elements 
//
void swap(int i, int j) {
  if (i == j) return;
  int tmp = a[i];
  a[i] = a[j];
  a[j] = tmp;
  sorted = 0;
}

// Initialize array with random values from [0..N]
//
void initArray() {
  struct timeval tv;
  gettimeofday(&tv,NULL);
  a = (int *) malloc((N) * sizeof(int));
  srand(tv.tv_usec);
  for (int i = 0; i < N; i++) 
    a[i] = rand() % N;
}

// Print array 
//
void printArray() {
  for (int i = 0; i < N; i++)
    printf("%d,", a[i]);
}

// Verify that array is sorted
//
void verifyArray() {
  for (int i = 0; i < N-1; i++)
    if (a[i] > a[i+1]) {
      printf("FAILED: a[%d]=%d, a[%d]=%d\n", 
          i, a[i], i+1, a[i+1]);
      return;
    }
  printf("Result verified!\n");
}

// Odd-even sort the array a
//
void oddevenSort() {
    // odd or even could be done in either order
  for (int t = 0; t < N; t += 2) {
    for (int i = 0; i < N; i += 2) 
      if (a[i] > a[i+1]) 
        swap(i, i+1);
    for (int i = 1; i < N; i += 2) 
      if (a[i] > a[i+1])
        swap(i, i+1);
#ifdef DEBUG
    printf("t=%2d: ", t);
    printArray();
#endif
  }
}

void exchange(int rank){
  int val=0;
  if(rank ==0) {
    for(int i = N - 1; i >= 0; --i) {
      MPI_Send(&a[i], 1, MPI_INT, 1, 0, MPI_COMM_WORLD);  
      MPI_Recv(&val, 1, MPI_INT, 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
      if(val < a[i]) {
        a[i] = val;
        sorted = 0;
      } 
    }
  } else {
    int tosend;
    for(int i = 0; i < N; ++i) {
      MPI_Recv(&val, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
      if(a[i] < val) {
        tosend = a[i];
        a[i] = val;
        sorted = 0;
        MPI_Send(&tosend, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);  
      } else {
        MPI_Send(&N, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);  
      }
    }
  }
}
// Main routine for testing oddevenSort
// 
int main(int argc, char **argv) {
  int rank, size;

  MPI_Status st;
  MPI_Init(&argc, &argv);  
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);  
  MPI_Comm_size(MPI_COMM_WORLD, &size);  

  if (argc < 2) {
    printf("Usage: ./oddevenSort N\n");
    exit(0);
  }
  if ((N = atoi(argv[1])) < 1) {
    printf("N must be positive\n");
    exit(0);
  }
  N = N / 2; 

  initArray();

  printf("Initial data for proc %d: ", rank);
  printArray();
  printf("\n");

  while(!sorted) {
    sorted = 1;
    oddevenSort();
    exchange(rank);
  }

  if(rank == 0) {
    printf("Result: [");
    printArray();
  }
  fflush(stdout);
  MPI_Barrier(MPI_COMM_WORLD);
  if(rank == 1) {
    printArray();
    printf("]\n");
  }
  MPI_Barrier(MPI_COMM_WORLD);

  if(rank == 1) verifyArray();
  free(a);
  MPI_Finalize();
}

