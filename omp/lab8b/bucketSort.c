//----------------------------------------------------------------------------- 
// Program code for CS 415P/515 Parallel Programming, Portland State University
//----------------------------------------------------------------------------- 

// A bucket sort program (C version)
//
// Usage: 
//   linux> ./bucketSort N [B]    // N: array size, B: num of buckets
// 
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>

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

// Main routine 
// 
int main(int argc, char **argv) {
  int N = 1;          // data array size
  int B = 1;          // number of buckets

  if (argc < 2) {
    printf("Usage: ./bucketSort N [B]\n");
    exit(0);
  }
  if ((N = atoi(argv[1])) < 1) {
    printf("N must be positive\n");
    exit(0);
  }
  // Get param B (num of buckets), verify it's a power of 2
  if (argc == 3)
    B = atoi(argv[2]);
  if (!IsPowerOf2(B)) {
    printf("B must be a power of 2\n");
    exit(0);
  }

  // initialize data array
  int *a = createArray(N);    // array to be sorted
  printf("Initial data: ");
  printArray(N, a);

  // allocate buckets with a safe bucket size
  int bucket[B][Max(64,2*N/B)];
  int bcnt[B];                // individual bucket count
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

  // bubble sort each bucket
  for (int k = 0; k < B; k++)
    bubbleSort(bcnt[k], bucket[k]);
#ifdef DEBUG
  for (int k = 0; k < B; k++) {
    printf("sorted bucket[%d]: ", k);
    printArray(bcnt[k], bucket[k]);
  }
#endif
   
  // copy sorted data back to array a
  int i = 0;
  for (int k = 0; k < B; k++) 
    for (int j = 0; j < bcnt[k]; j++)
      a[i++] = bucket[k][j];

  printf("Result: ");
  printArray(N, a);
  verifyArray(N, a);
}
