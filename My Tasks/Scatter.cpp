// MPI_Scatter sample program
// Name: Muhammad Zeeshan Yousaf
// Roll no: 259-BSCS-19 (E1)


#include <iostream>
#include<mpi.h>
#include <cmath>
using namespace std;

int main(int argc, char *argv[])
{
	int rank, size;
	
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	int *inputArr = new int[size];

	if(rank == 0)
		for(int i = 0; i < size; i++)
			inputArr[i] = rand() % 100;

	int *rec = new int[1];

	MPI_Scatter(inputArr, 1, MPI_INT, rec, 1, MPI_INT, 0, MPI_COMM_WORLD);

	cout << "Data in process: " << rank << " = " << rec[0] << endl;

	MPI_Finalize();
	return 0;
}