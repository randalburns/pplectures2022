#include <stdio.h>
#include <sys/time.h>

#include <stdlib.h>
#include <omp.h>

/*
 *  stencil.c
 *
 *  This program has sucessively optimized implementations of stencil computations.
 *  It has functions to 
 *     * randomly intialize 2-d array with doubles in [0,1]
 *     * Average a stencil of 2*HWIDTH+1 for each cell
 *     * Sum two averaged stencils
 *
 *  This computing pattern is common in numerical simulations and similar
 *  to the computation of convulation functions in ML.
 *
 *  Compile with:
 *      gcc -Xpreprocessor -fopenmp -O3 -lomp stencil.c (clang MacOSX)
 *      gcc -fopenmp -O3 stencil.c (gcc)
 */

// Dimension of the array.  Data will be DIM x DIM
//const int DIM = 16384;
//const int DIM = 8192;
const int DIM = 4096;
// Number of trials.  Set to get desired confidence intervals.
const int TRIALS = 4;
// HWIDTH = 2 matches the unrolled code.  If you change, comparisons will break.
const int HWIDTH = 2;

/* Helper function to deal with gettimeofday()

Subtract the `struct timeval' values X and Y,
    storing the result in RESULT.
    Return 1 if the difference is negative, otherwise 0. */
int timeval_subtract (struct timeval * result, struct timeval * y, struct timeval * x)
{
  /* Perform the carry for the later subtraction by updating y. */
  if (x->tv_usec < y->tv_usec) {
    int nsec = (y->tv_usec - x->tv_usec) / 1000000 + 1;
    y->tv_usec -= 1000000 * nsec;
    y->tv_sec += nsec;
  }
  if (x->tv_usec - y->tv_usec > 1000000) {
    int nsec = (x->tv_usec - y->tv_usec) / 1000000;
    y->tv_usec += 1000000 * nsec;
    y->tv_sec -= nsec;
  }
    
  /* Compute the time remaining to wait.
     tv_usec is certainly positive. */
  result->tv_sec = x->tv_sec - y->tv_sec;
  result->tv_usec = x->tv_usec - y->tv_usec;

  /* Return 1 if result is negative. */
  return x->tv_sec < y->tv_sec;
}

/* Randomly Initialize in column major order. 
  Strided access should be less efficient. */
void initializeyx ( double* array )
{
    /* Initialize the array to random values */
    for (int y=0; y<DIM; y++) {
        for (int x=0; x<DIM; x++) {
            array[x*DIM+y] = (double)rand()/RAND_MAX;
        }        
    }
}

/* Randomly Initialize in row major order. 
  Sequential access should be more efficient. */
void initializexy ( double* array )
{
    /* Initialize the array to random values */
    for (int x=0; x<DIM; x++) {
        for (int y=0; y<DIM; y++) {
            array[x*DIM+y] = (double)rand()/RAND_MAX;
        }        
    }
}

/* Compute an averaging stencil where it is defined on the
    interior of the array.

  This is the naive, base implementation. */
void stencil_average ( double* input_ar, double* output_ar )
{
    double partial = 0.0;

    for (int x=HWIDTH; x<DIM-HWIDTH; x++) {
        for (int y=HWIDTH; y<DIM-HWIDTH; y++) {
            for (int xs=-1*HWIDTH; xs<=HWIDTH; xs++) {
                for (int ys=-1*HWIDTH; ys<=HWIDTH; ys++) {
                    partial += input_ar[DIM*(x+xs)+(y+ys)];
                }   
            }   
            output_ar[DIM*x+y] = partial/((2*HWIDTH+1)*(2*HWIDTH+1));
            partial=0.0;
        }       
    }
}

/* Parallelize the stencil computation with OpenMP. */
void stencil_average_omp ( double* input_ar, double* output_ar )
{
    omp_set_num_threads(4);
    #pragma omp parallel for 
    for (int x=HWIDTH; x<DIM-HWIDTH; x++) {
        for (int y=HWIDTH; y<DIM-HWIDTH; y++) {
            double partial = 0.0;
            for (int xs=-1*HWIDTH; xs<=HWIDTH; xs++) {
                for (int ys=-1*HWIDTH; ys<=HWIDTH; ys++) {
                    partial += input_ar[DIM*(x+xs)+(y+ys)];
                }   
            }   
            output_ar[DIM*x+y] = partial/((2*HWIDTH+1)*(2*HWIDTH+1));
            partial=0.0;
        }       
    }
}

/* Parallelize the stencil computation with OpenMP. */
void stencil_average_omp_inner ( double* input_ar, double* output_ar )
{
    omp_set_num_threads(4);
    for (int x=HWIDTH; x<DIM-HWIDTH; x++) {
        #pragma omp parallel for 
        for (int y=HWIDTH; y<DIM-HWIDTH; y++) {
            double partial = 0.0;
            for (int xs=-1*HWIDTH; xs<=HWIDTH; xs++) {
                for (int ys=-1*HWIDTH; ys<=HWIDTH; ys++) {
                    partial += input_ar[DIM*(x+xs)+(y+ys)];
                }   
            }   
            output_ar[DIM*x+y] = partial/((2*HWIDTH+1)*(2*HWIDTH+1));
            partial=0.0;
        }       
    }
}

/* Bad version of parallelized stencil that loses performance
    because it has a shared variable. */
void stencil_average_omp_bad ( double* input_ar, double* output_ar )
{
    double partial = 0.0;
    omp_set_num_threads(4);
    #pragma omp parallel for 
    for (int x=HWIDTH; x<DIM-HWIDTH; x++) {
        for (int y=HWIDTH; y<DIM-HWIDTH; y++) {
            for (int xs=-1*HWIDTH; xs<=HWIDTH; xs++) {
                for (int ys=-1*HWIDTH; ys<=HWIDTH; ys++) {
                    partial += input_ar[DIM*(x+xs)+(y+ys)];
                }   
            }   
            output_ar[DIM*x+y] = partial/((2*HWIDTH+1)*(2*HWIDTH+1));
            partial=0.0;
        }       
    }
}


/* Unroll the inner loop of the stencil to increase performance. */
void stencil_average_unrolled ( double* input_ar, double* output_ar )
{
    double partial;

    for (int x=HWIDTH; x<DIM-HWIDTH; x++) {
        for (int y=HWIDTH; y<DIM-HWIDTH; y++) {

            partial = input_ar[DIM*(x-2)+(y-2)];
            partial += input_ar[DIM*(x-2)+(y-1)];
            partial += input_ar[DIM*(x-2)+(y)];
            partial += input_ar[DIM*(x-2)+(y+1)];
            partial += input_ar[DIM*(x-2)+(y+2)];

            partial += input_ar[DIM*(x-1)+(y-2)];
            partial += input_ar[DIM*(x-1)+(y-1)];
            partial += input_ar[DIM*(x-1)+(y)];
            partial += input_ar[DIM*(x-1)+(y+1)];
            partial += input_ar[DIM*(x-1)+(y+2)];

            partial += input_ar[DIM*(x)+(y-2)];
            partial += input_ar[DIM*(x)+(y-1)];
            partial += input_ar[DIM*(x)+(y)];
            partial += input_ar[DIM*(x)+(y+1)];
            partial += input_ar[DIM*(x)+(y+2)];

            partial += input_ar[DIM*(x+1)+(y-2)];
            partial += input_ar[DIM*(x+1)+(y-1)];
            partial += input_ar[DIM*(x+1)+(y)];
            partial += input_ar[DIM*(x+1)+(y+1)];
            partial += input_ar[DIM*(x+1)+(y+2)];

            partial += input_ar[DIM*(x+2)+(y-2)];
            partial += input_ar[DIM*(x+2)+(y-1)];
            partial += input_ar[DIM*(x+2)+(y)];
            partial += input_ar[DIM*(x+2)+(y+1)];
            partial += input_ar[DIM*(x+2)+(y+2)];

            output_ar[DIM*x+y] = partial/((2*HWIDTH+1)*(2*HWIDTH+1));
            partial = 0.0;
        }
    }
}

/* Parallelize the unrolled stencil computation with OpenMP. */
void stencil_average_unrolled_omp ( double* input_ar, double* output_ar )
{
    omp_set_num_threads(4);
    #pragma omp parallel for 
    for (int x=HWIDTH; x<DIM-HWIDTH; x++) {
        for (int y=HWIDTH; y<DIM-HWIDTH; y++) {

            double partial;

            partial = input_ar[DIM*(x-2)+(y-2)];
            partial += input_ar[DIM*(x-2)+(y-1)];
            partial += input_ar[DIM*(x-2)+(y)];
            partial += input_ar[DIM*(x-2)+(y+1)];
            partial += input_ar[DIM*(x-2)+(y+2)];

            partial += input_ar[DIM*(x-1)+(y-2)];
            partial += input_ar[DIM*(x-1)+(y-1)];
            partial += input_ar[DIM*(x-1)+(y)];
            partial += input_ar[DIM*(x-1)+(y+1)];
            partial += input_ar[DIM*(x-1)+(y+2)];

            partial += input_ar[DIM*(x)+(y-2)];
            partial += input_ar[DIM*(x)+(y-1)];
            partial += input_ar[DIM*(x)+(y)];
            partial += input_ar[DIM*(x)+(y+1)];
            partial += input_ar[DIM*(x)+(y+2)];

            partial += input_ar[DIM*(x+1)+(y-2)];
            partial += input_ar[DIM*(x+1)+(y-1)];
            partial += input_ar[DIM*(x+1)+(y)];
            partial += input_ar[DIM*(x+1)+(y+1)];
            partial += input_ar[DIM*(x+1)+(y+2)];

            partial += input_ar[DIM*(x+2)+(y-2)];
            partial += input_ar[DIM*(x+2)+(y-1)];
            partial += input_ar[DIM*(x+2)+(y)];
            partial += input_ar[DIM*(x+2)+(y+1)];
            partial += input_ar[DIM*(x+2)+(y+2)];

            output_ar[DIM*x+y] = partial/((2*HWIDTH+1)*(2*HWIDTH+1));
            partial = 0.0;
        }
    }
}

/* Sum two arrays into a third.  Base implementation. */
void array_sum ( double* input_ar1, double* input_ar2, double* output_ar )
{
    for (int x=0; x<DIM; x++) {
        for (int y=0; y<DIM; y++) {
            output_ar[x*DIM+y] = input_ar1[x*DIM+y] + input_ar2[x*DIM+y];
        }        
    }
}

/* Parallelize array_sum with OpenMP */
void array_sum_omp ( double* input_ar1, double* input_ar2, double* output_ar )
{
    omp_set_num_threads(4);
    #pragma omp parallel for 
    for (int x=0; x<DIM; x++) {
        for (int y=0; y<DIM; y++) {
            output_ar[x*DIM+y] = input_ar1[x*DIM+y] + input_ar2[x*DIM+y];
        }        
    }
}


/* Function that computes stencils on two arrays and adds them in a single loop.
     This is loop fusion on three functions. */
void fused_stencil_sum_omp ( double* input_ar1, double* input_ar2, double* output_ar )
{
    omp_set_num_threads(4);
    #pragma omp parallel for 
    for (int x=HWIDTH; x<DIM-HWIDTH; x++) {
        for (int y=HWIDTH; y<DIM-HWIDTH; y++) {
            double partial1 = 0.0;
            double partial2 = 0.0;
            for (int xs=-1*HWIDTH; xs<=HWIDTH; xs++) {
                for (int ys=-1*HWIDTH; ys<=HWIDTH; ys++) {
                    partial1 += input_ar1[DIM*(x+xs)+(y+ys)];
                    partial2 += input_ar2[DIM*(x+xs)+(y+ys)];
                }   
            }   
            output_ar[DIM*x+y] = partial1/((2*HWIDTH+1)*(2*HWIDTH+1)) + partial1/((2*HWIDTH+1)*(2*HWIDTH+1));
            partial1=0.0;
            partial2=0.0;
        }       
    }
}

/* Compute a max element. This an incorrect implementation because it is not thread safe. */
void max_el_shared ( double* input_ar )
{
    double max_el = 0;
    omp_set_num_threads(4);
    
    #pragma omp parallel for shared(max_el)
    for (int x=0; x<DIM; x++) {
        for (int y=0; y<DIM; y++) {
            max_el = max_el > input_ar[x*DIM+y] ? max_el : input_ar[x*DIM+y]; 
        }        
    }
}

/* Compute a max element with a critical section */
void max_el_critical ( double* input_ar )
{
    double max_el = 0;
    omp_set_num_threads(4);
    
    #pragma omp parallel 
    for (int x=0; x<DIM; x++) {
        for (int y=0; y<DIM; y++) {
            #pragma omp critical
            max_el = max_el > input_ar[x*DIM+y] ? max_el : input_ar[x*DIM+y]; 
        }        
    }
}
    
/* Compute a max element with a reduction */
void max_el_reduce ( double* input_ar )
{
    double max_el = 0;
    omp_set_num_threads(4);
    
    #pragma omp parallel for reduction ( max: max_el )
    for (int x=0; x<DIM; x++) {
        for (int y=0; y<DIM; y++) {
            max_el = max_el > input_ar[x*DIM+y] ? max_el : input_ar[x*DIM+y]; 
        }        
    }
}

int main()
{
    double* rand_ar1 = (double*) malloc ( DIM * DIM * sizeof(double));
    double* rand_ar2 = (double*) malloc ( DIM * DIM * sizeof(double));
    double* avg_ar1 = (double*) malloc ( DIM * DIM * sizeof(double));
    double* avg_ar2 = (double*) malloc ( DIM * DIM * sizeof(double));
    double* sum_ar = (double*) malloc ( DIM * DIM * sizeof(double));

    struct timeval begin, end, tresult;

    // warm up arrays w.r.t. caching
    initializexy(rand_ar1);
    initializexy(rand_ar2);


    // Compare iteration order w.r.t. memory layout
    for (int x=0; x<TRIALS; x++)
    {    
        gettimeofday(&begin, NULL);
        initializexy(rand_ar1);
        gettimeofday(&end, NULL);
        timeval_subtract ( &tresult, &begin, &end );
        printf ("xy = %f\n", (double)tresult.tv_sec + (double)tresult.tv_usec/1000000 );

        gettimeofday(&begin, NULL);
        initializeyx(rand_ar1);
        gettimeofday(&end, NULL);
        timeval_subtract ( &tresult, &begin, &end );
        printf ("yx = %f\n", (double)tresult.tv_sec + (double)tresult.tv_usec/1000000 );
    }

    // warm up avg_ar w.r.t. cache
    stencil_average(rand_ar1, avg_ar1);
    stencil_average(rand_ar2, avg_ar2);

    // Performance of serial stencil
    for (int x=0; x<TRIALS; x++)
    {    
        gettimeofday(&begin, NULL);
        stencil_average(rand_ar1, avg_ar1);
        gettimeofday(&end, NULL);
        timeval_subtract ( &tresult, &begin, &end );
        printf ("stencil avg= %f\n", (double)tresult.tv_sec + (double)tresult.tv_usec/1000000 );
    }
    
    // OpenMP parallel stencil
    for (int x=0; x<TRIALS; x++)
    {    
        gettimeofday(&begin, NULL);
        stencil_average_omp(rand_ar1, avg_ar1);
        gettimeofday(&end, NULL);
        timeval_subtract ( &tresult, &begin, &end );
        printf ("ompstencil avg= %f\n", (double)tresult.tv_sec + (double)tresult.tv_usec/1000000 );
    }

    // parallel inner loop loses performance 
    for (int x=0; x<TRIALS; x++)
    {    
        gettimeofday(&begin, NULL);
        stencil_average_omp_inner(rand_ar1, avg_ar1);
        gettimeofday(&end, NULL);
        timeval_subtract ( &tresult, &begin, &end );
        printf ("inner loop parallel ompstencil avg= %f\n", (double)tresult.tv_sec + (double)tresult.tv_usec/1000000 );
    }

    // Bad implementation with shared variable.
    for (int x=0; x<TRIALS; x++)
    {    
        gettimeofday(&begin, NULL);
        stencil_average_omp_bad(rand_ar1, avg_ar1);
        gettimeofday(&end, NULL);
        timeval_subtract ( &tresult, &begin, &end );
        printf ("bad ompstencil avg= %f\n", (double)tresult.tv_sec + (double)tresult.tv_usec/1000000 );
    }

    // Unrolled loop stencil
    for (int x=0; x<TRIALS; x++)
    {    
        gettimeofday(&begin, NULL);
        stencil_average_unrolled(rand_ar1, avg_ar1);
        gettimeofday(&end, NULL);
        timeval_subtract ( &tresult, &begin, &end );
        printf ("unrolled avg= %f\n", (double)tresult.tv_sec + (double)tresult.tv_usec/1000000 );
    }

    // Unrolled and Parallelized
    for (int x=0; x<TRIALS; x++)
    {    
        gettimeofday(&begin, NULL);
        stencil_average_unrolled_omp(rand_ar1, avg_ar1);
        gettimeofday(&end, NULL);
        timeval_subtract ( &tresult, &begin, &end );
        printf ("unrolled omp avg= %f\n", (double)tresult.tv_sec + (double)tresult.tv_usec/1000000 );
    }


    // warm up avg_ar w.r.t. cache
    stencil_average(rand_ar1, avg_ar1);
    stencil_average(rand_ar2, avg_ar2);
  
    // warm up sum w.r.t. cache
    array_sum(avg_ar1, avg_ar2, sum_ar);

    // Array sum base implementation
/*    for (int x=0; x<TRIALS; x++)
    {    
        gettimeofday(&begin, NULL);
        array_sum(avg_ar1, avg_ar2, sum_ar);
        gettimeofday(&end, NULL);
        timeval_subtract ( &tresult, &begin, &end );
        printf ("sum avg= %f\n", (double)tresult.tv_sec + (double)tresult.tv_usec/1000000 );
    }

    // Array sum parallelized with OpenMP
    for (int x=0; x<TRIALS; x++)
    {    
        gettimeofday(&begin, NULL);
        array_sum_omp(avg_ar1, avg_ar2, sum_ar);
        gettimeofday(&end, NULL);
        timeval_subtract ( &tresult, &begin, &end );
        printf ("omp sum avg= %f\n", (double)tresult.tv_sec + (double)tresult.tv_usec/1000000 );
    }
*/

    // Now sum two averaged arrays
    // Perform all tasks sequentially.
    // Each task is parallelized independently.
    for (int x=0; x<TRIALS; x++)
    {    
        gettimeofday(&begin, NULL);
        stencil_average_omp(rand_ar1, avg_ar1);
        stencil_average_omp(rand_ar2, avg_ar2);
        array_sum_omp(avg_ar1, avg_ar2, sum_ar);
        gettimeofday(&end, NULL);
        timeval_subtract ( &tresult, &begin, &end );
        printf ("separate loops= %f\n", (double)tresult.tv_sec + (double)tresult.tv_usec/1000000 );
    }

    // Fused all tasks into a loop.
    for (int x=0; x<TRIALS; x++)
    {    
        gettimeofday(&begin, NULL);
        fused_stencil_sum_omp(rand_ar1, rand_ar2, sum_ar);
        gettimeofday(&end, NULL);
        timeval_subtract ( &tresult, &begin, &end );
        printf ("fused loops= %f\n", (double)tresult.tv_sec + (double)tresult.tv_usec/1000000 );
    }

    // Get the maximum value out of a filtered array
    for (int x=0; x<TRIALS; x++)
    {    
        gettimeofday(&begin, NULL);
        max_el_shared(avg_ar1);
        gettimeofday(&end, NULL);
        timeval_subtract ( &tresult, &begin, &end );
        printf ("max el loops= %f\n", (double)tresult.tv_sec + (double)tresult.tv_usec/1000000 );
    }

    // Reduce the maximum value out of a filtered array
    for (int x=0; x<TRIALS; x++)
    {    
        gettimeofday(&begin, NULL);
        max_el_reduce(avg_ar1);
        gettimeofday(&end, NULL);
        timeval_subtract ( &tresult, &begin, &end );
        printf ("max el reduced= %f\n", (double)tresult.tv_sec + (double)tresult.tv_usec/1000000 );
    }

    // Get the maximum value out of a filtered array with a critical section
    for (int x=0; x<TRIALS; x++)
    {    
        gettimeofday(&begin, NULL);
        max_el_critical(avg_ar1);
        gettimeofday(&end, NULL);
        timeval_subtract ( &tresult, &begin, &end );
        printf ("max el critical= %f\n", (double)tresult.tv_sec + (double)tresult.tv_usec/1000000 );
    }
}
