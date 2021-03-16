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
using namespace std; 

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

  for (int i = 2; i <= sqrt(N); i++)
    if (!compo[i]) 
      for (int j = i+i; j <= N; j += i)
        compo[j] = true;

  int cnt = 0;
  for (int i = 2; i <= N; i++)
    if (!compo[i]) cnt++;
  cout << "Found " << cnt << " primes in 2.." << N << "\n";
  for (int i = 2; i < N; i++)
    if (!compo[i]) cout << to_string(i) + ",";
  cout << "\n";
}
