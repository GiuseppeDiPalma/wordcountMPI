#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <mpi.h>

#include "utils.h"
#include "fileManage.h"

#define WORDS_PROCESSOR 100

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
    
    char *dirFile;

    FileRowSize fileSpec[10];
    parse_arg(argc, argv, &dirFile);

    PartitionedWord n_words[WORDS_PROCESSOR];
    long int wordsForProcessor[size];

    printf("size: [%d]\n\n", size);
    if(rank==0)
    {
        printf("(START) - MASTER(#%d) - (START)\n", rank);
        printf("Directory file ---> %s\n", dirFile);

        long int sumWord = readFiles(dirFile, fileSpec);
        elementSplit(wordsForProcessor, sumWord, size);
        printf("total words %ld\n", sumWord);
        for (int i = 0; i < size; i++)
        {
            printf("proc %d - processa %ld parole\n", i, wordsForProcessor[i]);
        }

        wordForProcessor(n_words, wordsForProcessor, fileSpec, size);

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