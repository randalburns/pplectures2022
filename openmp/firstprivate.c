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

void main () 
{
  int i;
  int sh = 0;
  int pr = 5;

  #pragma omp parallel for firstprivate(pr) shared(sh)
  for ( i=0; i<10000; i++ )
  { 
    printf("OMP Thread# %d, pr++=%d, sh++=%d\n", omp_get_thread_num(), pr++, sh++);
  }
}

