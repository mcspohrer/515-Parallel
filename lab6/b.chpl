module bank {
  use Random;

  var rs = new owned RandomStream(uint);

  config const INIT_BALANCE:uint  = 1000;
  config const NUM_DEPOSITS       = 10;
  config const NUM_WITHDRAWS      = 10;
  config const DEPOSIT_AMT:uint   = 1000;
  config const WITHDRAW_AMT:uint  = 200;
  var total = INIT_BALANCE;
  var lck : sync bool;

  proc deposit(i) { 
    var amount = rs.getNext() % DEPOSIT_AMT;
    lck = true;
    var oldtotal = total;
    total += amount;
    var newtotal = total;
    var ulck = lck;
    writef("Deposit-%i %5i (%3i -> %3i)\n", i, amount, oldtotal, newtotal);
  }

  proc withdraw(i) { 
    var amount = rs.getNext() % WITHDRAW_AMT;
    if (amount < total) {
      lck = true;
      var oldtotal = total;
      total -= amount;
      var newtotal = total;
      var ulck = lck;
      writef("Withdraw-%i %4i (%3i -> %3i)\n", i, amount, oldtotal, newtotal);
    } else {
      var oldtotal = total;
      writef("Withdraw-%i %4i (%3i) **aborted**\n", i, amount, oldtotal);
    }
  }

  proc main() {
    forall i in 1..(NUM_DEPOSITS) do 
      begin deposit(i);
    forall i in 1..(NUM_WITHDRAWS) do 
      begin withdraw(i);
  }

}

