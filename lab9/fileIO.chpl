//----------------------------------------------------------------------------- 
// Program code for CS 415/515 Parallel Programming, Portland State University.
//----------------------------------------------------------------------------- 

// File I/O examples
//
use IO;

config const infile = "input";
config const outfile = "output";
const fin = open(infile, iomode.r);
const fout = open(outfile, iomode.cw);

const N = fin.length()/4;     // find integer count of infile
const D = {1..N};
var a:[D] int[32];            // note the size specification

// file input
var r = fin.reader(kind=ionative);  // create a channel for binary code
r.read(a);                    // read into array a
r.close();
writeln("N=", N, ", a: ", a);

// file output
var w = fout.writer(kind=ionative);
w.write(a);
w.close();

// parallel input
use BlockDist;
const MD = D dmapped Block(D);
var b:[MD] int[32];
forall i in MD do {
  var r = fin.reader(kind=ionative, start=i*4-4, end=i*4);  
  r.read(b[i]);
  r.close();
//  writef("i=%2i, r.locale: %i\n", i, r.locale.id);
}
writeln("N=", N, ", b: ", b);

fin.close();
fout.close();


