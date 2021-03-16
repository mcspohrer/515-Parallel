//----------------------------------------------------------------------------- 
// Program code for CS 415P/515 Parallel Programming, Portland State University
//----------------------------------------------------------------------------- 

// A prime-finding program (Sequential version).
//
// Usage: 
//   linux> ./prime N
//
#include <iostream>
#include <cmath> 
#include <omp.h>
using namespace std; 
#define NUM_THREADS 4

int main(int argc, char **argv) {
  int N = 2; 
  if (argc < 2) {
    cout << "Usage: ./prime N\n";
    exit(0);
  }
  if ((N=atoi(argv[1])) < 2) {
    cout << "N must be greater than 1\n";
    exit(0);
  }
  bool *compo = new bool[N+1]{};

  omp_set_num_threads(NUM_THREADS);
  printf("Searching primes in [1..%d] with %d threads\n", N, NUM_THREADS);
  for (int i = 2; i <= sqrt(N); i++)
    if (!compo[i]) {
#pragma omp parallel num_threads(NUM_THREADS)
    {
      printf("T[%d] working on prime [%d] (1st composite: %d)",
          omp_get_thread_num(), i, i+i);
#pragma omp for
      for (int j = i+i; j <= N; j += i){
        compo[j] = true;
    }}}

  int cnt = 0;
  for (int i = 2; i <= N; i++)
    if (!compo[i]) cnt++;
  cout << "Found " << cnt << " primes in 2.." << N << "\n";
  for (int i = 2; i < N; i++)
    if (!compo[i]) cout << to_string(i) + ",";
  cout << "\n";
}
