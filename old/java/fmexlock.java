import java.util.concurrent.atomic.AtomicInteger;

class FMEXLock {    
    
    // barrier 1 and 2
    volatile int x_fmex = 0;
    volatile int y_fmex = 0;

    // b array -- Java doesn't provide volatile access to array elements
    AtomicInteger b_fmex[];
    
    int numthreads;
    
    public void lock(int id)
    {
        while(true)
        {
            b_fmex[id].set(1);
            x_fmex=id;
            if (y_fmex!=0) 
            {
                b_fmex[id].set(0);
                while (y_fmex!=0) {}
                continue;
            }
            y_fmex=id;
            if (x_fmex!=id) 
            {
                b_fmex[id].set(0);
                for (int u=0; u<numthreads; u++)
                {
                    while (b_fmex[u].get()==0) {}
                }
                if (y_fmex!=id)
                {
                    while(y_fmex!=0) {}
                    continue;
                }
            }
            return;
        }
    }
    
    public void unlock(int id)
    {        
        y_fmex=0;
        b_fmex[id].set(0);
    }
    
    FMEXLock (int nt)
    {
        numthreads = nt;
        b_fmex = new AtomicInteger[numthreads];
        for (int u=0; u<numthreads; u++)
        {
            b_fmex[u] = new AtomicInteger(0);
        }
    }
}

