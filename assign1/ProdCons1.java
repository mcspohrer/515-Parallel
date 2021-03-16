import java.util.LinkedList;
import java.util.Queue;

class ProdCons1 extends Thread {
  static final int BUFSIZE = 5; 
  static final int NUMITEMS = 50; 
  static Object lck = new Object();
  static Queue<Integer> q = new LinkedList<>();

  ProdCons1() {}


  public static void main(String[] args) throws InterruptedException {
      final ProdCon pc = new ProdCon();

      Thread prod = new Thread() {
        public void run() 
        {
          try {
            pc.producer();
          } 
          catch (InterruptedException e) {
            e.printStackTrace();
          }
        }
      };
  
      Thread con = new Thread() {
        public void run() 
        {
          try {
            pc.consumer();
          } 
          catch (InterruptedException e) {
            e.printStackTrace();
          }
        }
      };

      prod.start();
      con.start();
      prod.join();
      con.join();
        
      System.out.println("Main: all done! So leave me alone!");
  }
  public static class ProdCon {
    private void producer() throws InterruptedException{ 
        int i = 0;
        System.out.println("Producer is starting...");
        while(i < NUMITEMS){
          ++i;
          synchronized(lck) {
            while(q.size() == BUFSIZE) {
                lck.wait();
            }
            System.out.println("Producer added " + i + " (qsz: " + q.size() + ")");
            q.add(i);
            lck.notify();
          }
        }
        System.out.println("Producer is ending...");
    }

    private void consumer()  throws InterruptedException{ 
        System.out.println("Consumer is starting...");
        int i = 0;
        int num;
        while(i < NUMITEMS){
          synchronized(lck) {
            while(q.size() == 0) {
                lck.wait();
            }
            num = q.remove();
            System.out.println("Consumer consumed " + num + " (qsz: " + q.size() + ")");
            lck.notify();
          }
          ++i;
        }
        System.out.println("Consumer is ending...");
    }
  }
}

