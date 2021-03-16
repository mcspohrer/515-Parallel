/*

This program will numerically compute the integral of

                  4/(1+x*x) 
				  
from 0 to 1.  The value of this integral is pi -- which 
is great since it gives us an easy way to check the answer.

The is the original sequential program.  It uses the timer
from the OpenMP runtime library

History: Written by Tim Mattson, 11/99.

*/
#include <stdio.h>
#include <omp.h>
static long num_steps = 100000000;
#define NUM_THREADS 2
double step;
int main ()
{
	  int i, nthreads;
	  double x, pi, sum = 0.0;
	  double start_time, run_time;
    pi = 0.0;

	  step = 1.0/(double) num_steps;

    omp_set_num_threads(NUM_THREADS);

	  start_time = omp_get_wtime();
#pragma omp parallel
    {
      int i,id, nthrds = omp_get_num_threads();
      id = omp_get_thread_num();
      if (id == 0) nthreads = nthrds;
      sum = 0.0;
      for (i=id;i<= num_steps; i+=nthreads){
        x = (i-0.5)*step;
        sum = sum + 4.0/(1.0+x*x);
      }
#pragma omp critical
	  pi += step * sum;
    }
	  run_time = omp_get_wtime() - start_time;
	  printf("\n pi with %ld steps is %lf in %lf seconds\n ",num_steps,pi,run_time);
}	  





