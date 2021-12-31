/**
 * Example MPI file for testing
 *
 */
#include "file2_mpi.h"

#include <mpi.h>
#include <iostream>
#include <cmath>


using namespace std;

namespace libqqc{

int function_mpi()
{
    int rank;
    int size;

    MPI_Status status;
    MPI_Comm comm;
    comm = MPI_COMM_WORLD;

    MPI_Init(NULL, NULL);

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    std::cout << "Hello from rank " << rank << std::endl;

    MPI_Finalize();

    return 0;

}

}
