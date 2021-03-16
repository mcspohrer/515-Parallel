import java.util.LinkedList;
import java.util.Queue;

class ProdCons3 extends Thread {
  static final int BUFSIZE = 20; 
  static final int NUMITEMS = 100; 
  static int cc[], pc[];
  static Object lck = new Object();
  static Queue<Integer> q = new LinkedList<>();

  public static void main(String[] args) throws InterruptedException {
    int num_p, num_c = 0;
    int tot = args.length;
    if(tot == 0){ 
      num_c = nump = 1;
    } else if (tot == 1) { 
      num_c = Integer.parseInt(args[0]);
      num_p = 1;
    } else {
      num_c = Integer.parseInt(args[0]);
      num_p = Integer.parseInt(args[1]);
    }

    final Prod p = new Prod();
    final Con[] cons = new Con[k];
    ret = new int [k];
    
    
    Thread prod = new Thread() {
    };
    prod.start();

    for(int i = 0; i < k; ++i){
      cons[i] = new Con(i);
    }
    for(int i = 0; i < k; ++i) {
      cons[i].start();
    }
    for(int i = 0; i < k; ++i) {  
      cons[i].join();
    }
    prod.join();

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
            lck.notify();
            lck.wait();
          }
          System.out.println("Producer added " + i + " (qsz: " + q.size() + ")");
          q.add(i);
          lck.notify();
        }
      }
      synchronized(lck) {
        for(i = 0; i < NUMITEMS; ++i)
          q.add(-1);
          lck.notifyAll();
      }
      System.out.println("Producer is ending...");
    }

    public void run() 
    {
      try {
        int i = this.producer();
        pc[this.nme] = i;
      } 
      catch (InterruptedException e) {
        e.printStackTrace();
      }
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
          if( num != -1) {
            System.out.println("Consumer[" + currentThread().getName() 
                + "] consumed " + num + " (qsz: " + q.size() + ")");
            ++i;
          }
          lck.notify();
        }
      }

      System.out.println("Consumer[" + currentThread().getName() + "] is ending...");
      return i;
    }

    public void run(){
      try {
        int i = this.consumer();
        cc[this.nme] = i;
      } catch (InterruptedException e) {
        e.printStackTrace();
      }
    }
  }
}

