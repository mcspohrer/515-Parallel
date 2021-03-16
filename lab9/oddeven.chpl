//----------------------------------------------------------------------------- 
// Program code for CS 415/515 Parallel Programming, Portland State University.
//----------------------------------------------------------------------------- 

// Oddven sort (shared-memory version)
//
use Random;

config const N = 10;
const D = {1..N};
var a:[D] int;

fillRandom(a);           // fill a with random values from 0..N-1
a = ((a % N) + N) % N; 
writeln("Init:   ", a);
 
// main loop
for t in D by 2 {
  forall i in D by 2 do             // even phase
    if i < N && a(i) > a(i+1) then 
      a(i) <=> a(i+1);
  forall i in D by 2 do             // odd phase
    if i > 1 && a(i-1) > a(i) then 
      a(i-1) <=> a(i);
} 
writeln("Result: ", a);
