// MPI_Gather sample code 
// Name: Muhammad Zeeshan Yousaf
// Roll no: 259-BSCS-19 (E1)

#include <iostream>
#include<mpi.h>
#include <cmath>
using namespace std;
main(int argc, char *argv[])
{
	int my_rank, size;
	int *rec;

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

	int inputArr = rand() % 100;

	if (my_rank == 0)
		rec = new int[size];
		
	MPI_Gather( &inputArr, 1, MPI_INT, rec, 1, MPI_INT, 0, MPI_COMM_WORLD);

	if (my_rank == 0)
		for(int i = 0; i < size; i++)
			cout << "\nData in process: " << my_rank << " = " << rec[i] << endl;

	MPI_Finalize();
}
