/* One producer, One consumer, 3 threads 
 * Using circular Queue, a conditional variable
 * and a mutex lock */
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
Queue pcq(BUFFSIZE);

void producer(void) {
  int core = sched_getcpu();
  cout << "Producer starting on core " + to_string(core) + "\n";

  for(int i = 1; i <= NUMITEMS; ++i){
    unique_lock<mutex> lck(mtx);
    while(pcq.isFull()) 
    {
      cv.notify_one();
      cv.wait(lck);
    } 
    pcq.add(i);
    cv.notify_one();
    lck.unlock();
    cout << "Producer added " + to_string(i) + " (qsz: " + to_string(pcq.size()) + ")\n"; 
  }
  cv.notify_one();
  cout << "Producer ending\n";
}
  
void consumer(void) {
  int core = sched_getcpu();
  cout << "Consumer starting on core " + to_string(core) + "\n";
  for(int i = 1; i <= NUMITEMS; ++i){
    unique_lock<mutex> lck(mtx);
    while(pcq.isEmpty()) 
    {
      cv.notify_one();
      cv.wait(lck);
    } 
    pcq.remove();
    cv.notify_one();
    lck.unlock();
    cout << "Consumer removed " + to_string(i) + " (qsz: " + to_string(pcq.size()) + ")\n"; 
  }
  cv.notify_one();
  cout << "Consumer ending\n";
}

int main(int argc, char **argv) {
  thread t1, t2;
  t1 = thread(producer);
  t2 = thread(consumer);
  cv.notify_all();
  t1.join();
  t2.join();
  cout << "Main: all done! Now leave me alone!\n";
}  
