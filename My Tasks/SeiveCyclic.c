// Sieve of Eratosthenes prime cyclic partitioning parallel Algorithm.
// Name: Muhammad Zeeshan Yousaf
// Roll no: 259-BSCS-19 (E1)

#include <mpi.h>
#include <stdio.h>


int getNoOfTasksToSolve(int processNumber, int totalSize, int noOfProcesses) {
    int remain = totalSize % noOfProcesses;
    int addMore = 1;
    if (remain <= processNumber)
        addMore = 0;
    int noOfTasks = (totalSize / noOfProcesses) + addMore;

    return noOfTasks;
}


int findNextIndexOfMultipleToMark(int arr[] ,int size,int currentIndex)
{
    int i;
    for(i=currentIndex; i < size; i++)
    {
        if(arr[i]!= -1)
            return i;
    }

    return -1;
}

int getTotalTasksSolved(int processNumber, int totalSize, int noOfProcesses)
{
    int extraTasks = 0;
    int equalTasks = (totalSize / noOfProcesses);
    if(totalSize < noOfProcesses)
    {
        if(processNumber+1 <= totalSize)
            extraTasks = processNumber;
    }
    else if(totalSize > noOfProcesses)
    {
        extraTasks = totalSize % noOfProcesses;
        if(processNumber+1 <= extraTasks)
            extraTasks = processNumber;
    }
    else
        extraTasks = 0;
    return  (equalTasks * processNumber) + extraTasks;
}

void getMyNumbersArray(int arr[] , int size,int processNumber, int noOfProcesses)
{
    int j = 0;
    int i;
    for (i = processNumber+2; j < size; i+=noOfProcesses, j++) {
        arr[j] = i;
    }
}

void main(int argc, char **argv)
{
    int myrank=0;
    int noOfProcesses = 0;
    MPI_Init(&argc,&argv);
    MPI_Comm_rank(MPI_COMM_WORLD,&myrank);
    MPI_Comm_size(MPI_COMM_WORLD,&noOfProcesses);

    int totalNumbers = 50;
    if(argc > 1)
    {
        totalNumbers = atoi(argv[1]);
        if(totalNumbers  < 1)
            totalNumbers = 50;
    }

    int totalSize = totalNumbers - 1;

    const int mySize = getNoOfTasksToSolve(myrank,totalSize,noOfProcesses);

    if(mySize != 0)
    {
        int myNumbers[mySize];
        getMyNumbersArray(myNumbers,mySize,myrank,noOfProcesses);
        int current_Index = 0 , i;
        int numberToMarks = 0;
        int flagHolder = 0;
        int j;
        for(j=0; j < (totalSize % noOfProcesses) + (totalSize / noOfProcesses); j++)
        {
            if(flagHolder == myrank)
            {
                current_Index = findNextIndexOfMultipleToMark(myNumbers,mySize,current_Index);
                if(current_Index != -1)
                    numberToMarks = myNumbers[current_Index];
                else
                    numberToMarks = -1;
                current_Index++;
            }

            MPI_Bcast(&numberToMarks,1,MPI_INT,flagHolder,MPI_COMM_WORLD);
            if(numberToMarks != -1)
            {
                for(i=current_Index; i < mySize; i++)
                {
                    if(myNumbers[i] % numberToMarks == 0)
                        myNumbers[i] = -1;
                }
            }

            flagHolder = (flagHolder +1) % noOfProcesses;

        }

        for(i=0; i<mySize; i++)
        {
            if(myNumbers[i] != -1)
                printf("%i is Prime\n",myNumbers[i]);
        }
    }

    MPI_Finalize();
}