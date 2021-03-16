var D = {1..100};
var a: [D] int = [i in D] i + 1;
var b: [D] int = [i in D/2] i;
var c: [D] int = a + b;
var sum$: sync int = 0;

for i in c do
  sum$ += i;
var sum = sum$;

writeln("c = ", c, "\t// c has ", c.numElements, " elements now\n");
writeln("sum = ", sum);


