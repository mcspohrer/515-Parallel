//----------------------------------------------------------------------------- 
// Program code for CS 415/515 Parallel Programming, Portland State University.
//----------------------------------------------------------------------------- 

// The sum program (OpenMP version).
//
//
#include <stdio.h>
#include <omp.h>

#define N 1000 	/* problem parameter */

int compute(int i) { return i*i; }

int main(int argc, char **argv) {
  int sum = 0;
#pragma omp parallel for reduction(+:sum)
  for (int i = 0; i < N; i++){
    printf("Thread %d\n", omp_get_thread_num());
    sum += compute(i);
  }
  printf("The sum is %d (should be 332833500)\n", sum);
}  
