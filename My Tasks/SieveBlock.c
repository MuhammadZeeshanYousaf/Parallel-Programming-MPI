// Sieve of Eratosthenes prime block partitioning parallel Algorithm.
// Name: Muhammad Zeeshan Yousaf
// Roll no: 259-BSCS-19 (E1)

#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>

int a[12] = {2,3,4,5,6,7,8,9,10,11,12,13};

int main(int argc, char* argv[])
{
	int rank, size, elements_per_process, n_elements_recieved;
	int n = 12;

	MPI_Status status;

	MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (rank == 0) 
    {
    	int index, i;
        elements_per_process = n / size;

        if (size > 1) {
            for (i = 1; i < size - 1; i++) {
                index = i * elements_per_process;
                MPI_Send(&elements_per_process,1, MPI_INT, i, 999,MPI_COMM_WORLD);
                MPI_Send(&a[index],elements_per_process,MPI_INT, i, 999,MPI_COMM_WORLD);
            }

            index = i * elements_per_process;
            int elements_left = n - index;
            MPI_Send(&elements_left,1, MPI_INT,i, 999,MPI_COMM_WORLD);
            MPI_Send(&a[index],elements_left,MPI_INT, i, 999,MPI_COMM_WORLD);
        }

        for (int i = 0; i < elements_per_process && i <= (int)sqrt(n); ++i)
        {
            MPI_Bcast(&a[i], 1, MPI_INT, 0, MPI_COMM_WORLD);
            for (int j = 0; j < elements_per_process; i++)
            if(a[j] % a[i] == 0)
                a[j] = -1;
        }
         
    }
    else
    {
    	MPI_Recv(&n_elements_recieved,1, MPI_INT, 0, 999, MPI_COMM_WORLD, &status);
        MPI_Recv(&a, n_elements_recieved,MPI_INT, 0, 999,MPI_COMM_WORLD, &status);

        for (int i = 0; i < elements_per_process && i <= (int)sqrt(n); ++i)
        {
            MPI_Bcast(&a[i], 1, MPI_INT, 0, MPI_COMM_WORLD);
            for (int j = 0; j < elements_per_process; i++)
            if(a[j] % a[i] == 0)
                a[j] = -1;
        }

    }


    MPI_Finalize();
    return 0;
}