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

//  #pragma omp parallel for //schedule(dynamic,4)
// schedule(dynamic,1)
// schedule(static,2)
//schedule(dynamic,4)
//#pragma omp parallel for schedule (guided)

  int i; 
    
  #pragma omp parallel for 
  for ( i=0; i<30; i++ )
  { 
    printf("OMP Thread# %d, loop variable %d\n", omp_get_thread_num(), i);
  }
}

