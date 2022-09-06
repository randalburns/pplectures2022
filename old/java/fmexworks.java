class FMEXWorks implements Runnable
{
  int thread_id;
  FMEXLock lock;
  
  static int sharedvar = 0;

  public void run ()
  { 
    for ( int i=0; i<1000; i++ )
    {
        lock.lock(this.thread_id);
        sharedvar++;
        lock.unlock(this.thread_id);      
    }
  }
  
  FMEXWorks(int tid, FMEXLock lk)
  {
    this.thread_id = tid;
    this.lock = lk;
  }

  public static void main ( String[] args )
  {
    int numthreads = 4;
    FMEXLock lock = new FMEXLock(4);
    Thread[] threads = new Thread[numthreads];

    // create and start thread objects
    for ( int i=0; i<numthreads; i++ )
    {
        threads[i] = new Thread ( new FMEXWorks(i, lock) );
        threads[i].start();
    }

    // Await the completion of all threads
    for ( int i=0; i<numthreads; i++ )
    {
        try {
            threads[i].join();
        } 
        catch(InterruptedException e)
        {
            System.out.println("join interrupted");
        }
    }
    System.out.println("Shared variable = " + FMEXWorks.sharedvar);
  }
}

