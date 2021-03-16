config const N = 8;
var D = {1..N, 1..N};
var a: [D] int;
var b: [D] int;
var lock$: sync bool;
var c: [D] int;
//var sum$: sync int = 0;

coforall (i,j) in D do {
  a(i,j) = i+j+2;
  b(i,j) = i+j;
}

coforall (i,j) in D do {
  coforall k in {1..N} do{
    lock$ = true;
    c(i,j) += a(i,k) * b(k,j);
    var unlock = lock$;
  }
}

var sum = + reduce c; 

//forall (i,j) in D do
//  sum$ += c(i,j);

writeln("a = ", a, "\t// a has ", a.numElements, " elements now\n");
writeln("b = ", b, "\t// b has ", b.numElements, " elements now\n");
writeln("c = ", c, "\t// c has ", c.numElements, " elements now\n");
writeln("sum = ", sum);
//writeln("sum$ = ", sum$);

