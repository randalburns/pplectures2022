/*
 * passitforward : a simple MPI program that forwards a 1
 *  circularly around all of the MPI processes
 */

#include <stdio.h>
#include "mpi.h"

int main ( int argc, char** argv )
{
  /* Simulation variables */
  int value = 0;
  int incoming;

  int iterations = 8;

  /* MPI Standard variable */
  int num_procs;
  int ID;

  /* Messaging variables */
  MPI_Status stat;
  int prev;
  int next; 

  /* MPI Setup */
  if ( MPI_Init(&argc, &argv) != MPI_SUCCESS )
  {
    printf ( "MPI_INit error\n" );
  }

  MPI_Comm_size ( MPI_COMM_WORLD, &num_procs );
  MPI_Comm_rank ( MPI_COMM_WORLD, &ID );

  /* Initialize process specific state */
  next =  ( ID + 1 ) % num_procs;
  prev = ID == 0 ? num_procs -1 : ID-1;   

  /* Start one ball in the system */
  if ( ID == 0 ) 
  {
    value = 1;
  }
  else
  {
    value = 0;
  }

  int i = 0;
  for ( i =0; i < iterations; i++ )
  {
    /* Send and receive point to point messages */
    MPI_Ssend ( &value, 1, MPI_INT, next, 2, MPI_COMM_WORLD ); 
    MPI_Recv ( &incoming, 1, MPI_INT, prev, 2, MPI_COMM_WORLD, &stat );
    if ( incoming == 1 )
    {
      printf ( "Iteration %d: Process %d received message from %d, value %d \n", i, ID, prev, incoming );
    }

    /* update the value */
    value = incoming;
  }
  MPI_Finalize();
}

