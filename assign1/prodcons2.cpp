#include "queue.h"
#include <iostream> 
#include <thread> 
#include <mutex> 
#include <condition_variable> 
using namespace std; 

mutex mtx;   
condition_variable cv;
int BUFFSIZE = 20;
int NUMITEMS = 100; 
int * conRes;
Queue pcq(BUFFSIZE);

void producer(int n) {
  int core = sched_getcpu();
  cout << "Producer starting on core " + to_string(core) + "\n";

  for(int i = 1; i <= NUMITEMS; ++i){
    unique_lock<mutex> lck(mtx);
    while(pcq.isFull()) 
    {
      cv.notify_all();
      cv.wait(lck);
    } 
    pcq.add(i);
    lck.unlock();
    cv.notify_one();
    cout << "Producer added " + to_string(i) + " (qsz: " + to_string(pcq.size()) + ")\n"; 
  }
  for(int i = 0; i < n; ++i){
    unique_lock<mutex> lck(mtx);
    while(pcq.isFull()) 
    {
      cv.notify_all();
      cv.wait(lck);
    } 
    pcq.add(-1);
    lck.unlock();
  }
  cv.notify_all();
  cout << "Producer ending\n";
}
  
void consumer(int tid) {
  int r, c = 0;
  int core = sched_getcpu();
  cout << "Consumer[" + to_string(tid) + "] starting on core " + to_string(core) + "\n";

  while(r != -1) {
    unique_lock<mutex> lck(mtx);
    while(pcq.isEmpty()) 
    {
      cv.notify_one();
      cv.wait(lck);
    } 
    r = pcq.remove();
    lck.unlock();
    cv.notify_one();

    if (r != -1) {
      ++c;
      cout << "Consumer[" + to_string(tid) + "] removed " + to_string(r) 
        + " (qsz: " + to_string(pcq.size()) + ")\n"; 
    }
  }
  cv.notify_one();
  cout << "Consumer[" + to_string(tid) + "] ending\n";
  conRes[tid] = c;
}

int main(int argc, char **argv) {
  int numCons, tot = 0;
  
  if(argv[1]) numCons = atoi(argv[1]);   //gets the number of
  else numCons = 1;                      //consumers

  conRes = new int [numCons];            //inits an array for to hold 
                                         //the number each consumer consumed
  thread pthd, cthd[numCons];

  cout << "Prodcons2 with "+ to_string(numCons) + " consumers and a single producer\n";
  
  pthd = thread(producer,numCons);       //start producer
  for (int k = 0; k < numCons; k++)      //start consumers
    cthd[k] = thread(consumer, k);

  pthd.join();                           //wait for the producer to finish
  for (int k = 0; k < numCons; k++)      //join consumers
    cthd[k].join();

  cout << "Consumer stats: [";
  for(int i = 0; i < numCons; ++i) {
    cout << to_string(conRes[i]) + ",";
    tot += conRes[i];
  }
  cout << "] = " + to_string(tot) + "\n";

  cout << "Main: all done! Now stop bothering me!\n";
  delete(conRes);
}  
