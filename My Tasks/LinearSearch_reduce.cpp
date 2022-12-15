// Parallel program that search a number synchronously with MPI_Reduce 
// Name: Muhammad Zeeshan Yousaf
// Roll no: 259-BSCS-19 (E1)

#include <mpi.h>
#include <iostream>

using namespace std;


int main(int argc, char *argv[]) {
    // Unique rank is assigned to each process in a communicator
    int rank;

    // Total number of ranks
    int size;

    // Initializes the MPI execution environment
    MPI_Init(&argc, &argv);

    // Get this process' rank (process within a communicator)
    // MPI_COMM_WORLD is the default communicator
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    // Get the total number ranks in this communicator
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int Number_to_find = 159;
    int Elements_per_process = 10;
    const int Data[50] = { 897, 802, 765, 992, 100, 521, 220, 380, 729, 969, 184, 887, 104, 641, 909, 378, 724, 582, 387, 583, 241, 294, 159, 198, 653, 369, 418, 692, 36, 901, 516, 623, 703, 971, 304, 394, 491, 525, 464, 219, 183, 648, 796, 287, 979, 395, 356, 702, 667, 743 };
    int start_index = rank * Elements_per_process;
    int end_index = start_index + Elements_per_process;

    bool is_found_local = false;

    for (int i = start_index; i < end_index; i++) {
        cout << "Process " << rank << " checking..." << Data[i] << endl;
        if (Data[i] == Number_to_find) {
            is_found_local = true;
            cout << "***(Process " << rank << " found: " << Number_to_find << ")***"<< endl;
            break;
        }
    }
    
    bool is_found_global;
    
    MPI_Reduce(&is_found_local, &is_found_global, 1, MPI_CXX_BOOL, MPI_LOR, 0, MPI_COMM_WORLD);


    if (rank == 0){
        if (is_found_global == true) 
            cout << Number_to_find << " Found :)" << endl;
        else
            cout << Number_to_find << " Not Found :(" << endl;
    }


    MPI_Barrier(MPI_COMM_WORLD);
    MPI_Finalize();

    return 0;
}
