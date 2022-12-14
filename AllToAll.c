// Sample MPI program for All-to-All broadcast and All-to-one reduction
// Name: Muhammad Zeeshan Yousaf

#include "mpi.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

#ifndef EXIT_SUCCESS
#define EXIT_SUCCESS 0
#define EXIT_FAILURE 1
#endif

int main( int argc, char *argv[] )
{
    int rank, size;
    int chunk = 128;    // message chunk per process
    int i;
    int *sb;    // Sending buffer
    int *rb;    // Recieving buffer
    int status, gstatus;    // local status, global status

    MPI_Init(&argc,&argv);
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    MPI_Comm_size(MPI_COMM_WORLD,&size);

    // getting user number of messages per process if passed with as "-m <number>"
    for ( i=1 ; i < argc ; ++i ) {
        if ( argv[i][0] != '-' )
            continue;
        switch(argv[i][1]) {
            case 'm':
                chunk = atoi(argv[++i]);
                break;
            default:
                fprintf(stderr, "Unrecognized argument %s\n", argv[i]);fflush(stderr);
                MPI_Abort(MPI_COMM_WORLD,EXIT_FAILURE);
        }
    }
    
    // allocate memory for send and revieve buffer
    sb = (int *)malloc(size*chunk*sizeof(int));
    if ( !sb ) {
        perror( "can't allocate send buffer" );fflush(stderr);
        MPI_Abort(MPI_COMM_WORLD,EXIT_FAILURE);
    }
    rb = (int *)malloc(size*chunk*sizeof(int));
    if ( !rb ) {
        perror( "can't allocate recv buffer");fflush(stderr);
        free(sb);
        MPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
    }

    // Populate buffer
    for ( i=0 ; i < size*chunk ; ++i ) {
        sb[i] = rank + 1;
        rb[i] = 0;
    }
    
    // printf("Message %d \n", *(sb+1));
    // printf("After %d \n", (int)sizeof(sb));
    // printf("size %d \n", (size * chunk));

    status = MPI_Alltoall(sb, chunk, MPI_INT, rb, chunk, MPI_INT, MPI_COMM_WORLD);
    MPI_Allreduce( &status, &gstatus, 1, MPI_INT, MPI_SUM, MPI_COMM_WORLD );
    
    if (rank == 0) {
        printf("all_to_all executed and returned %d\n",gstatus);fflush(stdout);
        
        // printing messages of send buffer
        printf("Send Buffer = [ ");
        for (int j=0 ; i < size*chunk ; ++i )
            printf("%d, ", sb[i]);
        printf("\b\b ]");

        // printing messages of recieve buffer
        printf("Recieve Buffer = [ ");
        for( int k=0; k < size*chunk; k++ )
            printf("%d, ", rb[i]);
        printf(" ]");
    }

    // Clean up
    free(sb);
    free(rb);
    MPI_Finalize();
    return(EXIT_SUCCESS);
}
