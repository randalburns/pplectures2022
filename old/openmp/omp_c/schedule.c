#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <assert.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "omp.h"

int main () 
{

//  omp_set_num_threads ( 6 );
  int count=0;

//  #pragma omp parallel for schedule(dynamic,4) firstprivate(count)
#pragma omp parallel for schedule(static) firstprivate(count)
// schedule(dynamic,1)
// schedule(static,2)
  for ( int i=0; i<40; i++ )
  { 
    int tid = omp_get_thread_num();
    count++;
    printf ("%d th iteration of block in thread %d \n", count, tid );
  }
}

