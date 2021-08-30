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
  int i; 
    
  #pragma omp parallel for 
  for ( i=0; i<24; i++ )
  { 
    printf("OMP Thread# %d, loop variable %d\n", omp_get_thread_num(), i);
  }
}

