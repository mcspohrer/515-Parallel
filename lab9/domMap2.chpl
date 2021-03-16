//----------------------------------------------------------------------------- 
// Program code for CS 415/515 Parallel Programming, Portland State University.
//----------------------------------------------------------------------------- 

// Domain Map Example 
//
// linux> ./domMap2 -nl 4
//

use BlockDist, CyclicDist, BlockCycDist;

const D = {1..8, 1..8};
const MD1 = D dmapped Block(D);
const MD2 = D dmapped Cyclic(startIdx=D.low);
const MD3 = D dmapped Cyclic(startIdx=D.high);
const MD4 = D dmapped BlockCyclic(startIdx=D.low, blocksize=(2,3));
const MD5 = D dmapped BlockCyclic(startIdx=D.low, blocksize=(3,2));

var a1: [MD1] int;
var a2: [MD2] int;
var a3: [MD3] int;
var a4: [MD4] int;
var a5: [MD5] int;

forall e in a1 do e = here.id;
forall e in a2 do e = here.id;
forall e in a3 do e = here.id;
forall e in a4 do e = here.id;
forall e in a5 do e = here.id;

writeln("a1: "); writeln(a1); 
writeln("a2: "); writeln(a2); 
writeln("a3: "); writeln(a3);
writeln("a4: "); writeln(a4);
writeln("a5: "); writeln(a5);
