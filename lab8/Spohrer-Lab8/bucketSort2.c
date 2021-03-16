//----------------------------------------------------------------------------- 
// Program code for CS 415P/515 Parallel Programming, Portland State University
//----------------------------------------------------------------------------- 

// A bucket sort program (C version)
//
// Usage: 
//   linux> ./bucketSort N [B]    // N: array size, B: num of buckets
// 
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <mpi.h>

#define DATABITS 13 // assume data are 13-bit integers: [0,8191] 

#define Max(x,y) (((x) > (y)) ? (x) : (y))
#define IsPowerOf2(x) (!((x) & ((x) - 1)))

// Create an array with random values from [0..8191]
//
int *createArray(int n) {
  int *a = (int *) malloc(n * sizeof(int));
  srand(time(NULL));
  for (int i = 0; i < n; i++) 
    a[i] = rand() % 8192;
  return a;
}

// Print array 
//
void printArray(int n, int a[]) {
  for (int i = 0; i < n; i++)
    printf("%d,", a[i]);
  printf("\n");
}

// Verify that array is sorted
//
void verifyArray(int n, int a[]) {
  for (int i = 0; i < n-1; i++)
    if (a[i] > a[i+1]) {
      printf("FAILED: a[%d]=%d, a[%d]=%d\n", 
          i, a[i], i+1, a[i+1]);
      return;
    }
  printf("Result verified!\n");
}

// Find bucket idx for an int value x of b bits
//
int bktidx(int x, int b, int B) {
  return x >> (b - (int)log2(B));
}

// Bubble sort
//
void bubbleSort(int n, int a[]) {
  for (int i = 0; i < n; i++)
    for (int j = i+1; j < n; j++) 
      if (a[i] > a[j]) {
        int tmp = a[i];
        a[i] = a[j];
        a[j] = tmp;
      }
}
int* zerosWork(int N, int B) {
}
// Main routine 
// 
int main(int argc, char **argv) {
  int rank, size, N = 1;          // data array size
  int B = 1;          // number of buckets

  MPI_File fh;
  MPI_Status st;
  MPI_Init(&argc, &argv);  
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);  
  MPI_Comm_size(MPI_COMM_WORLD, &B);  
  MPI_File_open(MPI_COMM_SELF, argv[2],MPI_MODE_CREATE | MPI_MODE_WRONLY, MPI_INFO_NULL, &fh);

  if (argc < 2) {
    printf("Usage: ./bucketSort N <outfile>\n");
    MPI_Finalize();
    exit(0);
  }
  if ((N = atoi(argv[1])) < 1) {
    printf("N must be positive\n");
    MPI_Finalize();
    exit(0);
  }
  // Get param B (num of buckets), verify it's a power of 2
  if (!IsPowerOf2(B)) {
    printf("Number of processes must be a power of 2\n");
    MPI_Finalize();
    exit(0);
  }

  int bucket[B][Max(64,2*N/B)];
  memset(bucket, 0, B);
  int bcnt[B];                // individual bucket count
  memset(bcnt, 0, B);
  int displ[B];                // individual bucket count
  memset(bcnt, 0, B);
  // initialize data array
  if(rank == 0){
    int *a = createArray(N);    // array to be sorted
    printf("Initial data: ");
    printArray(N, a);

    // allocate buckets with a safe bucket size
    for (int k = 0; k < B; k++)
      bcnt[k] = 0;

    // distribute data to buckets
    for (int i = 0; i < N; i++) {
      int k = bktidx(a[i], DATABITS, B);
      bucket[k][bcnt[k]++] = a[i];
    }
#ifdef DEBUG
    for (int k = 0; k < B; k++) {
      printf("bucket[%d]: ", k);
      printArray(bcnt[k], bucket[k]);
    }
#endif

    /*
       printf("\nbucket");
       for (int k = 0; k < B; k++) {
       printf("\n %d: ", k);
       for (int j = 0; j < bcnt[k]; j++) {
       printf(" %d,", bucket[k][j]);
       }
       }
       printf("\nbcnt");
       for (int k = 0; k < B; k++) {
       printf(" %d,", bcnt[k]);
       }
       printf("\n");
    //find displacements
    displ[0] = 0;
    for(int i = 1; i < B; i++) {
    displ[i] += displ[i-1] + bcnt[i-1];
    }
    */
  }
  MPI_Scatter(bcnt, 1, MPI_INT, &bcnt[rank], 1, MPI_INT, 0, MPI_COMM_WORLD);
 /* MPI_Bcast(&bcnt, B, MPI_INT, 0, MPI_COMM_WORLD);
  MPI_Recv(&bcnt, B, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
  MPI_Barrier(MPI_COMM_WORLD);
  printf("rank %d bcnt:", rank);
       for (int k = 0; k < B; k++) {
       printf(" %d,", bcnt[k]);
       }
  printf("\n");
  */


  if(rank == 0) {
    for (int i = 1; i < B; i++) 
      MPI_Send(&bucket[i], bcnt[i], MPI_INT, i,0, MPI_COMM_WORLD); 
  } else {
    MPI_Recv(&bucket[rank], bcnt[rank], MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
  }
  /*MPI_Scatterv(&bucket[0][0], bcnt, displ, MPI_INT, &bucket[rank], bcnt[rank], MPI_INT, 0, MPI_COMM_WORLD);


*/
  bubbleSort(bcnt[rank], bucket[rank]);
#ifdef DEBUG
  for (int k = 0; k < B; k++) {
    printf("sorted bucket[%d]: ", k);
    printArray(bcnt[k], bucket[k]);
  }
#endif

  /* copy sorted data back to array a
     int i = 0;
     for (int k = 0; k < B; k++) 
     for (int j = 0; j < bcnt[k]; j++)
     a[i++] = bucket[k][j];
     printf("Result: ");
     printArray(N, a);
     */
  printf("rank %d cnt %d: ", rank, bcnt[rank]);
  for (int j = 0; j < bcnt[rank]; j++) {
    printf(" %d,", bucket[rank][j]);
  }
  printf("\n");
  for(int i = 0; i < B; i++) {
    if(i == rank) {
      MPI_File_seek(fh, 0, MPI_SEEK_END);
      MPI_File_write(fh, bucket[rank], bcnt[rank], MPI_INT, MPI_STATUS_IGNORE);
    }
    MPI_Barrier(MPI_COMM_WORLD);
  }
  MPI_File_close(&fh);
  MPI_Finalize();
}
