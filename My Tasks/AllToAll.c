// Sample MPI program for All-to-All broadcast and All-to-one reduction
// Name: Muhammad Zeeshan Yousaf
// Roll no: 259-BSCS-19 (E1)

#include "mpi.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

int main( int argc, char *argv[] )
{
    int rank, size;
    int messages_per_process = 64;    // total messages per process
    int i;
    int *send_b;    // Sending buffer
    int *receive_b;    // Recieving buffer
    int local_status, global_status;    // local status, global status

    MPI_Init(&argc,&argv);
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    MPI_Comm_size(MPI_COMM_WORLD,&size);
    
    // allocate memory for send and revieve buffer
    send_b = (int *)malloc(size*messages_per_process*sizeof(int));
    if ( !send_b ) {
        perror( "can't allocate send buffer" );fflush(stderr);
        MPI_Abort(MPI_COMM_WORLD,1);
    }
    receive_b = (int *)malloc(size*messages_per_process*sizeof(int));
    if ( !receive_b ) {
        perror( "can't allocate recv buffer");fflush(stderr);
        free(send_b);
        MPI_Abort(MPI_COMM_WORLD, 1);
    }

    // Create messages buffer
    for ( i=0 ; i < size*messages_per_process ; ++i ) {
        send_b[i] = rank + 1;
        receive_b[i] = 0;
    }

    local_status = MPI_Alltoall(send_b, messages_per_process, MPI_INT, receive_b, messages_per_process, MPI_INT, MPI_COMM_WORLD);

    MPI_Allreduce( &local_status, &global_status, 1, MPI_INT, MPI_SUM, MPI_COMM_WORLD );
    
    if (rank == 0) {
        printf("MPI_Alltoall successfully executed returned %d\n",global_status);
        fflush(stdout);

        // printing messages of send buffer
        printf("Send Buffer = [ ");
        for (int j=0 ; j < size*messages_per_process ; j++ )
            printf("%d, ", send_b[j]);
        printf(" ]\n");

        // printing messages of recieve buffer
        printf("Recieve Buffer = [ ");
        for( int k=0; k < size*messages_per_process; k++ )
            printf("%d, ", receive_b[i]);
        printf(" ]\n");
    }

    free(send_b);
    free(receive_b);
    MPI_Finalize();
    return 0;
}

