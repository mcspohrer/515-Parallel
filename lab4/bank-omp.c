#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <omp.h>

#define INIT_BALANCE  1000
#define NUM_DEPOSITS  10
#define NUM_WITHDRAWS 10
#define DEPOSIT_AMT   100
#define WITHDRAW_AMT  200

int total;  // account balance

void deposit(int i) { 
  int amount = rand() % DEPOSIT_AMT;
  int oldtotal = total;
#pragma omp atomic
  total += amount;
  printf("%d Deposit-%d %5d (%3d -> %3d)\n",omp_get_thread_num(), i, amount, oldtotal, total);
}

void withdraw(int i) { 
  int amount = rand() % WITHDRAW_AMT;
  int oldtotal = total;
  if (amount < total) {
#pragma omp atomic
    total -= amount;
    printf("%d Withdraw-%d %4d (%3d -> %3d)\n",omp_get_thread_num(), i, amount, oldtotal, total);
  } else {
    printf("Withdraw-%d %4d (%3d) **aborted**\n", i, amount, oldtotal);
  }
}
void
omp_section(void){
  int i = 0;
#pragma omp parallel shared(i)
  {
  while(i < NUM_DEPOSITS || i < NUM_WITHDRAWS){
    if(i < NUM_DEPOSITS){
#pragma omp sections
      {
#pragma omp section
        {deposit(i);}}}
    if(i < NUM_WITHDRAWS){
#pragma omp sections
      {
#pragma omp section
        {withdraw(i);}}}
#pragma omp atomic
    ++i;
}}}

void
omp_task(void){
  int i = 0;
#pragma omp parallel shared(i)
#pragma omp single
  while(i < NUM_DEPOSITS || i < NUM_WITHDRAWS){
    if(i < NUM_DEPOSITS){
#pragma omp task
      deposit(i);}
    if(i < NUM_WITHDRAWS){
#pragma omp task
      withdraw(i);}
#pragma omp atomic
      ++i;
}
}

void
omp_for(void){
#pragma omp parallel for
  for (int i = 1; i <= NUM_DEPOSITS; i++)
    deposit(i);
#pragma omp parallel for
  for (int i = 1; i <= NUM_WITHDRAWS; i++)
    withdraw(i);
}
int main() {
  total = INIT_BALANCE;
  srand(time(NULL));
  printf("OMP section:\n");
//  omp_section();
  total = INIT_BALANCE;
  printf("OMP task:\n");
  omp_task();
  total = INIT_BALANCE;
  printf("OMP for:\n");
  omp_for();
}


