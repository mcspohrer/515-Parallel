var a: [1..15, 1..10] int;
var b: [1..15, 1..10] int;
var sum$: sync int = 0;

forall (i,j) in {1..15,1..10} do
  a(i,j) = i + j;

forall (i,j) in {1..15,1..10} do
  b(i,j) = i * j;

var c = a + b;

for i in c do
  sum$ += i;
var sum = sum$;

writeln("c = ", c, "\t// c has ", c.numElements, " elements now\n");
writeln("sum = ", sum);


