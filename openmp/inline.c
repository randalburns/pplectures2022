#include <stdio.h>
#include <stdlib.h>
#include <time.h>

double square(int x) { return x*x; }
double cube (int x) { return x*x*x; }

int main ()
{
  // Initialize an A and B arrays
  int len = 1024;

  double a [len];
  double b [len];
  double c [len];

  int i;

  clock_t begin, end;
  int time_spent;

  // Initialize vectors
  for (i=0; i<len; i++)
      a[i] = rand_double();
      b[i] = rand_double();
  }

  begin = clock();

  // ridiculous function calls
  for (i=0; i<len; i++)
  {
    c[i] = square(a[i])/cube(a[i]) + square(b[i])/cube(b[i]); 
  }
  end = clock();

  time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
  printf("Addition took %d seconds", time_spent);

  return 0;
}
