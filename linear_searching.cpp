// This program search a number synchronously.
// Name: Muhammad Zeeshan Yousaf

#include <mpi.h>
#include <iostream>

using namespace std;


int main(int argc, char *argv[]) {
    // Unique rank is assigned to each process in a communicator
    int rank;

    // Total number of ranks
    int size;

    // The machine we are on
    char name[80];

    // Length of the machine name
    int length;

    // Initializes the MPI execution environment
    MPI_Init(&argc, &argv);

    // Get this process' rank (process within a communicator)
    // MPI_COMM_WORLD is the default communicator
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    // Get the total number ranks in this communicator
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Gets the name of the processor
    // Implementation specific (may be gethostname, uname, or sysinfo)
    MPI_Get_processor_name(name, &length);

    // Initialize shared resources
    int Number_to_find = 0;
    if (argc < 2) {
        cout << "Please provide element in argument after filename." << endl;
        MPI_Finalize();
        return 0;
    }
    else Number_to_find = std::stoi(argv[1]);
    const int N = 50;
    const int Data[N] = { 897, 802, 765, 992, 1, 521, 220, 380, 729, 969, 184, 887, 104, 641, 909, 378, 724, 582, 387, 583, 241, 294, 159, 198, 653, 369, 418, 692, 36, 901, 516, 623, 703, 971, 304, 394, 491, 525, 464, 219, 183, 648, 796, 287, 979, 395, 356, 702, 667, 743 };
    const int Elements_per_process = N/size;
    const int extra_elements = N%size;
    int found_by_process_rank = -1;


    if(rank == 0)
    {

       bool is_found = false;

       for (int i = 0; i <= Elements_per_process; i++) 
            if (Data[i] == Number_to_find) {
                found_by_process_rank = 0;
                is_found = true;
                break;
            }

       if (is_found == false) 
       { 
            MPI_Status status;
            for (int j = 1; j < size; j++) 
            {
                // Receive is_found from every process other than first process
                MPI_Recv(&is_found, 1, MPI_CXX_BOOL, j, j, MPI_COMM_WORLD, &status);
                if (is_found) {
                    found_by_process_rank = j;
                    break;
                }
            }
       }
       

        // After receiving signal from every process, print output...
        if (is_found) cout << Number_to_find << " found by process number " << found_by_process_rank + 1 << endl;
        else cout << Number_to_find << " not found :(" << endl;

    } 
    else
    {
        int start_index = rank * Elements_per_process;
        int end_index = start_index + Elements_per_process;
        if (rank == size - 1) end_index += extra_elements;
        bool is_found = false;

        for (int i = start_index; i <= end_index; i++) 
            if (Data[i] == Number_to_find) {
                is_found = true;
                break;
            }

        // Send is_found to the first process
        MPI_Send(&is_found, 1, MPI_CXX_BOOL, 0, rank, MPI_COMM_WORLD);
    }

    // Terminate MPI execution environment
    MPI_Finalize();

    return 0;
}
