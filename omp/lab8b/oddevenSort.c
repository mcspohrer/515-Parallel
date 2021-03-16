//----------------------------------------------------------------------------- 
// Program code for CS 415P/515 Parallel Programming, Portland State University
//----------------------------------------------------------------------------- 

// An odd-even sort program (C sequential version)
//
// Usage: 
//   linux> ./oddevenSort N
// 
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

int *a;           // array to be sorted
int N = 1;        // array size

// Swap two array elements 
//
void swap(int i, int j) {
  if (i == j) return;
  int tmp = a[i];
  a[i] = a[j];
  a[j] = tmp;
}

// Initialize array with random values from [0..N]
//
void initArray() {
  a = (int *) malloc(N * sizeof(int));
  srand(time(NULL));
  for (int i = 0; i < N; i++) 
    a[i] = rand() % N;
}

// Print array 
//
void printArray() {
  for (int i = 0; i < N; i++)
    printf("%d,", a[i]);
  printf("\n");
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
  for (int t = 0; t < N; t += 2) {
    // even phase
    for (int i = 0; i < N-1; i += 2) 
      if (a[i] > a[i+1]) 
	swap(i, i+1);
    // odd phase
    for (int i = 1; i < N-1; i += 2) 
      if (a[i] > a[i+1])
	swap(i, i+1);
#ifdef DEBUG
    printf("t=%2d: ", t);
    printArray();
#endif
  }
}

// Main routine for testing oddevenSort
// 
int main(int argc, char **argv) {
  if (argc < 2) {
    printf("Usage: ./oddevenSort N\n");
    exit(0);
  }
  if ((N = atoi(argv[1])) < 1) {
    printf("N must be positive\n");
    exit(0);
  }

  initArray();
  printf("Initial data: ");
  printArray();

  oddevenSort();
  printf("Result: ");
  printArray();

  verifyArray();
}

