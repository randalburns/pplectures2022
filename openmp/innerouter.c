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
  int x = 1;
  {
      printf ("Inner scope x = %d\n", x);
      {
        int x = 0;
        printf ("Inner Inner scope x = %d\n", x);
      }
  }
  printf ("Outer scope x = %d\n", x);
}

