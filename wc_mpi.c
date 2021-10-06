#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <mpi.h>

#include "utils.h"

void main(int argc, char *argv[])
{
    int rank, size;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Status stat;
    
    //Parameters for time MPI
    double start = MPI_Wtime();

    printf("(START) - MPI WORD COUNT - (START)\n\n");

    if(rank==0)
    {
        printf("(START) - MASTER(#%d) - (START)\n", rank);

        char *dirFile;
        parse_arg(argc, argv, &dirFile);
        printf("\nDirectory file ---> %s\n", dirFile);

        printf("\n(END) - MASTER(#%d) - (END)\n", rank);
    }
    else
    {
        printf("(START) - SLAVE(#%d) - (START)\n", rank);

        printf("(END) - SLAVE(#%d) - (END)\n", rank);
    }

    printf("(END) - MPI WORD COUNT - (END)\n\n");

    //Parameters for time and result MPI
    double end = MPI_Wtime();
    double totalTimeMPI = end - start;
    printf("Total time ---> %lf seconds\n", totalTimeMPI);

    MPI_Finalize();
}