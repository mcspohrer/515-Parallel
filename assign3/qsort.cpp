//----------------------------------------------------------------------------- 
// Program code for CS 415P/515 Parallel Programming, Portland State University
//----------------------------------------------------------------------------- 

// A quicksort program (C++ sequential version)
//
// Usage: 
//   linux> ./qsort-oo0 N
// 
#include <iostream>
#include <cmath> 
using namespace std; 

int MINSIZE = 10; // threshold for switching to bubblesort
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
  a = new int[N]{};
  srand(time(NULL));
  for (int i = 0; i < N; i++) 
    a[i] = (rand()*1./RAND_MAX) * N;
}

// Print array
//
void printArray(string msg) {
  cout << msg + ": ";
  for (int i = 0; i < N; i++) 
    cout << a[i] << ",";
  cout << "\n";
}

// Verify that array is sorted
//
void verifyArray() {
  for (int i = 0; i < N-1; i++)
    if (a[i] > a[i+1]) {
      cout << "FAILED: a[" << i << "]=" << a[i] <<
	", a[" << (i+1) << "]=" << a[i+1] << "\n";
      return;
    }
  printf("Result verified!\n");
}

// Bubble sort for the base cases
//
void bubbleSort(int low, int high) {
  if (low >= high) return;
  for (int i = low; i <= high; i++)
    for (int j = i+1; j <= high; j++) 
      if (a[i] > a[j])
	swap(i, j);
}

// Partition an array range a[low, high] into two sub-ranges
// - Use highest element as pivot
// - Return pivot's index
//
int partition(int low, int high) {
  int pivot = a[high]; 	
  int middle = low;
  for(int i = low; i < high; i++)
    if(a[i] < pivot)
      swap(i, middle++);
  swap(high, middle);
  return middle;
}
 
// QuickSort array range [low, high]
// 
void quickSort(int low, int high) {
  if (high - low < MINSIZE) {
    bubbleSort(low, high);
    return;
  }
  int middle = partition(low, high);
  quickSort(low, middle-1);
  quickSort(middle+1, high);
}
 
// Main routine for testing quickSort
// 
int main(int argc, char **argv) {
  if (argc < 2) {
    cout << "Usage: ./qsort N\n";
    exit(0);
  }
  if ((N = atoi(argv[1])) < 1) {
    cout << "N must be positive\n";
    exit(0);
  }

  initArray();
  printArray("Initial array");

  quickSort(0, N-1);
  printArray("After quicksort");

  verifyArray();
}

