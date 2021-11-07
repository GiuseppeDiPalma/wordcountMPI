#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <stddef.h>
#include <mpi.h>

#include "utils.h"
#include "fileManage.h"
#include "wordManage.h"

#define MAX_SPLITS 1000

void main(int argc, char *argv[])
{
    int rank, size;
    int tag = 1;
    int source = 0;
    int numSplits = 0;
    char *dirFile;
    int wordsForProcessor[size];

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Request rq = MPI_REQUEST_NULL;
    MPI_Status stat;

    //Parameters for time MPI
    double start = MPI_Wtime();

    PartitionedWord *partitions = (PartitionedWord *)malloc(sizeof(PartitionedWord) * MAX_SPLITS);
    Word *words = (Word *)malloc(sizeof(Word) * TOTALWORDS);
    Word *words_received = (Word *)malloc(sizeof(Word) * TOTALWORDS);

    int countBK = 4;
    int countBK1 = 2;
    int blockcount[countBK], blockcount1[countBK1];

    MPI_Datatype wordsType, filePerProcType, oldType[countBK], oldType1[countBK1];
    MPI_Aint offsets[countBK], offsets1[countBK1], lb, extent;

    offsets[0] = offsetof(PartitionedWord, fileName);
    oldType[0] = MPI_CHAR;
    blockcount[0] = MAXFILENAME;
    MPI_Type_get_extent(MPI_CHAR, &lb, &extent);

    offsets[1] = offsetof(PartitionedWord, rank);
    oldType[1] = MPI_INT;
    blockcount[1] = 1;
    MPI_Type_get_extent(MPI_INT, &lb, &extent);

    offsets[2] = offsetof(PartitionedWord, start);
    oldType[2] = MPI_INT;
    blockcount[2] = 1;
    MPI_Type_get_extent(MPI_INT, &lb, &extent);

    offsets[3] = offsetof(PartitionedWord, end);
    oldType[3] = MPI_INT;
    blockcount[3] = 1;
    MPI_Type_get_extent(MPI_INT, &lb, &extent);

    //create type struct
    MPI_Type_create_struct(countBK, blockcount, offsets, oldType, &filePerProcType);
    MPI_Type_commit(&filePerProcType);

    //save offset value after first field
    offsets1[0] = offsetof(Word, word);
    oldType1[0] = MPI_CHAR;
    blockcount1[0] = WORDMAXSIZE;
    MPI_Type_get_extent(MPI_CHAR, &lb, &extent);

    //space first field and second field
    offsets1[1] = offsetof(Word, freq);
    oldType1[1] = MPI_INT;
    blockcount1[1] = 1;
    MPI_Type_get_extent(MPI_INT, &lb, &extent);

    //create struct with fields
    MPI_Type_create_struct(countBK1, blockcount1, offsets1, oldType1, &wordsType);
    MPI_Type_commit(&wordsType);

    if (rank == 0)
    {
        parse_arg(argc, argv, &dirFile);
        int totalFiles = countFilesInDirectory(dirFile);

        FileWordSize fileSpec[totalFiles];
        int totalWords = readFilesAndSum(dirFile, fileSpec);
        printf("Total processor: #%d\n", size);
        printf("Total Words [%d]\n", totalWords);

        elementSplit(wordsForProcessor, totalWords, size);

        numSplits = wordForProcessor(partitions, wordsForProcessor, fileSpec, size);

        int iStruct = 0; //indice di dove mi trovo all'interno della struttura
        int startForZero = 0;
        int sizeOfZero = 0;

        //celle da passare a processo 1...
        while (partitions[iStruct].rank == 0)
        {
            iStruct++;
            startForZero++;
        }

        int base = -1;
        int sizeSend = -1;
        for (int i = 1; i < size; i++)
        {
            sizeSend = 0;   //quanti elementi
            base = iStruct; // Da dove parto
            while (partitions[iStruct].rank == i)
            {
                iStruct++;
                sizeSend++;
            }
            MPI_Send(&partitions[base], sizeSend, filePerProcType, i, tag, MPI_COMM_WORLD);
        }
        sizeOfZero = wordCount(words, partitions, startForZero);

        int sizeForProcessor = 0;
        for (int p = 1; p < size; p++)
        {
            MPI_Recv(words_received, TOTALWORDS, wordsType, p, tag, MPI_COMM_WORLD, &stat);
            MPI_Get_count(&stat, wordsType, &sizeForProcessor);

            sizeOfZero = mergeWords(words, words_received, sizeForProcessor, sizeOfZero);
        }
        writeResultCSV(words, sizeOfZero);

        //Parameters for time and result MPI
        double end = MPI_Wtime();
        double totalTimeMPI = end - start;
        printf("Total time: %lf seconds\n", totalTimeMPI);
    }
    else
    {
        int count = 0;
        int wordVectorLen = 0;
        MPI_Recv(partitions, MAX_SPLITS, filePerProcType, source, tag, MPI_COMM_WORLD, &stat);
        MPI_Get_count(&stat, filePerProcType, &count);

        wordVectorLen = wordCount(words, partitions, count);
        MPI_Ssend(words, wordVectorLen, wordsType, 0, tag, MPI_COMM_WORLD);
    }

    MPI_Type_free(&wordsType);
    MPI_Type_free(&filePerProcType);
    MPI_Finalize();
}
