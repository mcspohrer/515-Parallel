
//----------------------------------------------------------------------------- 
// Program code for CS 415/515 Parallel Programming, Portland State University.
//----------------------------------------------------------------------------- 

// A simple recursive routine (OMP version 2).
//
#include <stdio.h>
#include <omp.h>

int array[8] = {1,2,3,4,5,6,7,8};

// array a[]'s size n is assumped to be a power of 2
//
void rec(int a[], int n) {
  //printf("tid=%d count=%d\n", omp_get_thread_num(),
  if (n == 1) {
    printf("%d by thread %d count:%d\n", a[0], omp_get_thread_num(), omp_get_num_threads());
  } else {
    {
#pragma omp task
    rec(a,n/2);
#pragma omp task 
    rec(a+n/2, n/2);
    }
  }
}

int main() {
#pragma omp parallel num_threads(4) 
#pragma omp single
  rec(array, 8);
}
