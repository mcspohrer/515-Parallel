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
#include <string.h>

int *a;           // array to be sorted
int N = 100;        // array size
int sorted = 0;
int r;

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
  srand(time(NULL));
  N = rand() % N;
  a = (int *) malloc(N * sizeof(int));
  for (int i = 0; i < N; i++) 
    a[i] = rand() % N;
}

// Populates array with from file entered in at commandline
//
void getArray(char * infile) {
  FILE * fin = fopen(infile, "r");

  if(!fin) {
    initArray();
    printf("I initialized a random array of the initial size less"
        " than or equal to %d\n", N);
    return;
  }

  if((r = fseek(fin, 0L, SEEK_END))!= 0){
    fprintf(stderr, "Error with fseek() in getArray()\n");
    exit(EXIT_FAILURE);
  }
  if((N = (ftell(fin))/sizeof(int)) == -1){
    fprintf(stderr, "Error with ftell() in getArray()\n");
    exit(EXIT_FAILURE);
  }
  rewind(fin);

  printf("N=%d\n",N);
  a = (int *) malloc(N * sizeof(int));
  memset(a,0,(N*sizeof(int)));

  fread(a, sizeof(int), N, fin);

  if((r = fclose(fin)) != 0){
    fprintf(stderr, "Error with fclose() in getArray()\n");
    exit(EXIT_FAILURE); 
  }
}

void writeArray(char * ofile) {
  FILE * fd = fopen(ofile, "w");
  if(!fd) {
    fprintf(stderr, "Error with fopen() in writeArray()\n");
    exit(EXIT_FAILURE); 
  }

  r = fwrite(a, sizeof(int), N, fd);

  if((r = fclose(fd)) != 0){
    fprintf(stderr, "Error with fclose() in getArray()\n");
    exit(EXIT_FAILURE); 
  }
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
#ifdef DEBUG
  int t = 0;
#endif
  while(!sorted) {
    sorted = 1;
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
    t+=2;
#endif
  }
}

// Main routine for testing oddevenSort
// 
int main(int argc, char **argv) {
  if (argc < 3) {
    printf("Usage: ./oddevenSort <infile> <outfile>\n");
    exit(0);
  }

  getArray(argv[1]);
  printf("Initial data: ");
  printArray();

  oddevenSort();
  printf("Result: ");
  printArray();

  verifyArray();
  writeArray(argv[2]);
}

