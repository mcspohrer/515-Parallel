module bank {
  use Random;

  var rs = new owned RandomStream(uint);

  config const INIT_BALANCE:uint  = 1000;
  config const NUM_DEPOSITS       = 10;
  config const NUM_WITHDRAWS      = 10;
  config const DEPOSIT_AMT:uint   = 1000;
  config const WITHDRAW_AMT:uint  = 200;
  var total = INIT_BALANCE;

  proc deposit(i) { 
    var amount = rs.getNext() % DEPOSIT_AMT;
    var oldtotal = total;
    total += amount;
    writef("Deposit-%i %5i (%3i -> %3i)\n", i, amount, oldtotal, total);
  }

  proc withdraw(i) { 
    var amount = rs.getNext() % WITHDRAW_AMT;
    var oldtotal = total;
    if (amount < total) {
      total -= amount;
      writef("Withdraw-%i %4i (%3i -> %3i)\n", i, amount, oldtotal, total);
    } else {
      writef("Withdraw-%i %4i (%3i) **aborted**\n", i, amount, oldtotal);
    }
  }

  proc main() {
    for i in 1..(NUM_DEPOSITS) do 
      deposit(i);
    for i in 1..(NUM_WITHDRAWS) do 
      withdraw(i);
  }

}

