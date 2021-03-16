//----------------------------------------------------------------------------- 
// Program code for CS 415/515 Parallel Programming, Portland State University.
//----------------------------------------------------------------------------- 

// Domain Map Example 
//
// linux> ./domMap1 -nl 4
//

use BlockDist;

const D: domain(1) = {1..8};
const BD1 = D dmapped Block(D);
const BD2 = D dmapped Block({2..6});
const BD3 = D dmapped Block({1..12});

var a: [D] int;
var b1: [BD1] int;
var b2: [BD2] int;
var b3: [BD3] int;

forall e in a do e = here.id;
forall e in b1 do e = here.id;
forall e in b2 do e = here.id;
forall e in b3 do e = here.id;

writeln(a); 
writeln(b1); 
writeln(b2); 
writeln(b3); 
