//----------------------------------------------------------------------------- 
// Program code for CS 415/515 Parallel Programming, Portland State University.
//----------------------------------------------------------------------------- 

// Hello world program (OMP version).
//
#include <stdio.h>
#include <omp.h>

int main() {
#pragma omp parallel
  printf("Hello world! thread %d tot threads %d\n", omp_get_thread_num(), omp_get_num_threads());
}
