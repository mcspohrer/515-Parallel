#include <iostream> 
#include <string> 
#include <thread> 
#include <mutex> 
#include <condition_variable> 
#include "queue.h"
using namespace std; 

mutex mtx;                  //mutex for the conditional
condition_variable con_cv;  //consumer's cv, con wait on empty Q
condition_variable prod_cv; //producer's cv, prod wait on full Q
int BUFFSIZE = 20;
int NUMITEMS = 100; 
int * conRes;               //results array for the consumer
int * prodRes;              //results array for the producer
int numProds;               //number of producers
Queue pcq(BUFFSIZE);

/* Called from main. It waits on the producer's conditional
 * variable when the queue is full otherwise adds a number
 * to the queue.
*/
void producer(int tid) {
  int core = sched_getcpu();
  int range = NUMITEMS/numProds;
  int start = (range * tid) + 1;
  int stop = start + range - 1;
  int i, c = 0;
  if(tid == numProds - 1)
    stop = NUMITEMS;

  cout << "Producer[" + to_string(tid) + "] starting on core " + to_string(core)
    + " for segment [" + to_string(start) + ".." + to_string(stop) + "]\n";

  for(i = start; i <= stop; i++){
    unique_lock<mutex> lck(mtx);
    while(pcq.isFull()) 
    {
      con_cv.notify_all();
      prod_cv.wait(lck);
    } 
    pcq.add(i);
    con_cv.notify_one();
    lck.unlock();
    ++c;
    cout << "Producer[" + to_string(tid) + "] added " + to_string(i) 
      + " (qsz: " + to_string(pcq.size()) + ")\n"; 
  }
  prodRes[tid] = c;                             //records the count it added
  cout << "Producer[" + to_string(tid) + "] ending\n";
}
  
/* Called from main. It waits on the consumer's conditional
 * variable when the queue is empty otherwise removes a number
 * from the queue.
*/
void consumer(int tid) {
  int r, c = 0;
  int core = sched_getcpu();

  cout << "Consumer[" + to_string(tid) + "] starting on core " + to_string(core) + "\n";

  while(r != -1) {
    unique_lock<mutex> lck(mtx);
    while(pcq.isEmpty()) 
    {
      prod_cv.notify_all();
      con_cv.wait(lck);
    } 
    r = pcq.remove();
    prod_cv.notify_one();
    lck.unlock();
    if (r != -1) {
      ++c;
      cout << "Consumer[" + to_string(tid) + "] removed " + to_string(r) 
        + " (qsz: " + to_string(pcq.size()) + ")\n"; 
    }
  }

  prod_cv.notify_one();
  conRes[tid] = c;
  cout << "Consumer[" + to_string(tid) + "] ending\n";
}

int main(int argc, char **argv) {
  int numCons, tot = 0;

  if(argv[1]) numCons = atoi(argv[1]);   //gets the number of
  else numCons = 1;                      //consumers

  if(argv[2]) numProds = atoi(argv[2]);  //gets the number of
  else numProds = 1;                     //producers

  conRes = new int [numCons];            //inits consumer result
  prodRes = new int [numProds];          //inits prod results

  thread pthd[numProds], cthd[numCons];

  cout << "Prodcons3 with "+ to_string(numCons) + " consumers, "+ to_string(numProds) + " producers\n";

  for (int k = 0; k < numProds; k++)     //start ptoducers
    pthd[k] = thread(producer, k);
  for (int k = 0; k < numCons; k++)      //start consumers
    cthd[k] = thread(consumer, k);
  for (int k = 0; k < numProds; k++)     //join producers     
    pthd[k].join();

  for (int k = 0; k < numCons; ++k){     //add -1 to the queue
    unique_lock<mutex> lck(mtx);         //stop the consumers
    while(pcq.isFull()) 
    {
      con_cv.notify_all();
      prod_cv.wait(lck);
    } 
    pcq.add(-1);
    con_cv.notify_one();
    lck.unlock();
  }
  for (int k = 0; k < numCons; k++)     //join consumers
    cthd[k].join();

  cout << "Main added " + to_string(numCons) + " ENDSIGNs (qsz: " + to_string(pcq.size()) + ")\n";

  cout << "Producer stats: [";
  for(int i = 0; i < numProds; ++i) {
    cout << to_string(prodRes[i]) + ",";
    tot += prodRes[i];
  }
  cout << "] = " + to_string(tot) + "\n";

  tot = 0;                               //resets the total count

  cout << "Consumer stats: [";
  for(int i = 0; i < numCons; ++i) {
    cout << to_string(conRes[i]) + ",";
    tot += conRes[i];
  }
  cout << "] = " + to_string(tot) + "\n";
  cout << "Main: all done! Now stop bothering me!\n";
  delete(conRes);
  delete(prodRes);
}  
