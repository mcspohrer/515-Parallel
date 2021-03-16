import java.util.LinkedList;
import java.util.Queue;

class ProdCons2 extends Thread {
  static final int BUFSIZE = 20; 
  static final int NUMITEMS = 100; 
  static int ret[];
  static Object lck = new Object();
  static Queue<Integer> q = new LinkedList<>();

  public static void main(String[] args) throws InterruptedException {
    int k = 0;
    int tot = 0;
    if(args.length == 0){ 
      k = 1;
    } else { 
      k = Integer.parseInt(args[0]);
    }
    final Prod p = new Prod();
    final Con[] cons = new Con[k];
    ret = new int [k];
    
    
    Thread prod = new Thread() {
      public void run() 
      {
        try {
          p.producer();
        } 
        catch (InterruptedException e) {
          e.printStackTrace();
        }
      }
    };
    prod.start();

    for(int i = 0; i < k; ++i){
      cons[i] = new Con(i);
    }
    for(int i = 0; i < k; ++i) {
      cons[i].start();
    }

    prod.join();
    for(int i = 0; i < k; ++i) {  
      cons[i].join();
    }

    System.out.print("Consumer Stats: [");
    for(int i = 0; i < k; ++i) {  
      System.out.print(ret[i] + ",");
      tot += ret[i];
    }
    System.out.println("] total = " + tot);
    
        
    System.out.println("Main: all done! So leave me alone!");
  }
  public static class Prod {
    Prod() {}

    private void producer() throws InterruptedException{ 
      int i = 0;
      System.out.println("Producer is starting...");
      while(i < NUMITEMS){
        ++i;
        synchronized(lck) {
          while(q.size() == BUFSIZE) {
            lck.notifyAll();
            lck.wait();
          }
          q.add(i);
          lck.notifyAll();
        }
        System.out.println("Producer added " + i + " (qsz: " + q.size() + ")");
      }
      synchronized(lck) {
        for(i = 0; i < NUMITEMS; ++i)
          q.add(-1);
          lck.notifyAll();
      }
      System.out.println("Producer is ending...");
    }
  }

  public static class Con extends Thread{
    private int nme = 0;
    Con(int tid) {
      nme = tid;
    }

    private int consumer()  throws InterruptedException{ 
      System.out.println("Consumer[" + currentThread().getName() + "] is starting...");
      int num = 0;
      int i = 0;

      while(num != -1){
        synchronized(lck) {
          while(q.size() == 0) {
            lck.wait();
          }
          num = q.remove();
          lck.notify();
        }
        if( num != -1) {
          System.out.println("Consumer[" + currentThread().getName() 
              + "] consumed " + num + " (qsz: " + q.size() + ")");
          ++i;
        }
      }

      System.out.println("Consumer[" + currentThread().getName() + "] is ending...");
      return i;
    }

    public void run(){
      try {
        int i = this.consumer();
        ret[this.nme] = i;

      } catch (InterruptedException e) {
        e.printStackTrace();
      }
    }
  }
}

