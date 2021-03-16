//----------------------------------------------------------------------------- 
// Program code for CS 415P/515 Parallel Programming, Portland State University
//----------------------------------------------------------------------------- 

// A Pthread condition variable demo program.
//
//
#include <stdio.h>
#include <pthread.h>

pthread_mutex_t mtx;
pthread_cond_t cvar;

void sender() {
  printf("Sender (tid:%ld) starts ...\n", pthread_self());

  pthread_mutex_lock(&mtx);
  printf("bout to send\n");
  pthread_cond_broadcast(&cvar);
  printf("send\n");
  pthread_mutex_unlock(&mtx);

  printf("Signal sent!\n");
}

void receiver() {
  printf("Receiver (tid:%ld) starts ...\n", pthread_self());

  pthread_mutex_lock(&mtx);
  printf("wait\n");
  pthread_cond_wait(&cvar,&mtx);
  printf("rcv\n");
  pthread_mutex_unlock(&mtx);


  printf("Signal received!\n");
}

int main(int argc, char **argv) {
  pthread_t tid1, tid2, tid3;

  pthread_mutex_init(&mtx, NULL);
  pthread_cond_init(&cvar, NULL);
  pthread_create(&tid2, NULL, (void *)receiver, NULL);
  pthread_create(&tid3, NULL, (void *)receiver, NULL);
  pthread_create(&tid1, NULL, (void *)sender, NULL);
  pthread_join(tid1, NULL);
  pthread_join(tid2, NULL);
  pthread_join(tid3, NULL);
}
