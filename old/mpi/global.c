/*******************************************************************************
*
*    $Id: $
*
*    Randal C. Burns
*    Department of Computer Science
*    Johns Hopkins University
*
*    $Source: $
*    $Date: $        
*    $Revision: $
*
*******************************************************************************/


#include <stdio.h>
#include "mpi.h"

int main ( int argc, char** argv )
{
  int num_procs;
  int ID;

  if ( MPI_Init(&argc, &argv) != MPI_SUCCESS )
  {
    printf ( "MPI_INit error\n" );
  }

  MPI_Comm_size ( MPI_COMM_WORLD, &num_procs );
  MPI_Comm_rank ( MPI_COMM_WORLD, &ID );
  printf ( "\n Never miss a good chance to shut up %d \n", ID );

  MPI_Finalize();
}

/*******************************************************************************
*
*  Revsion History 
*    
*  $Log: $
*    
*******************************************************************************/
