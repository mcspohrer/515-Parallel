//----------------------------------------------------------------------------- 
// Program code for CS 415/515 Parallel Programming, Portland State University.
//----------------------------------------------------------------------------- 

// The sum program (Chapel version 1).
//
//
config const N = 1000;	/* problem domain size */
const D = {0..N-1};
var sum: int;

// note: with type-inference, you may obmit types in declarations
// 
proc compute(i) { return i*i; }

sum = + reduce [i in D] (compute(i));
writeln("The sum is ", sum, " (should be 332833500)");
