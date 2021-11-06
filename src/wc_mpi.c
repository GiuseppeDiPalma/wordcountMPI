#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <stddef.h>
#include <mpi.h>

#include "utils.h"
#include "fileManage.h"
#include "wordManage.h"

#define TOTALWORDS 4000000 // questa variabile dovrebbe essere dinamica
#define SPLIT_PROCESSOR 1000

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

    printf("Number of Processor: #[%d]\n", size);
    
    //Parameters for time MPI
    double start = MPI_Wtime();

    PartitionedWord *partitions = (PartitionedWord *)malloc(sizeof(PartitionedWord) * SPLIT_PROCESSOR);
    Word *words = (Word *)malloc(sizeof(Word) * TOTALWORDS);

    parse_arg(argc, argv, &dirFile);
    int totalFiles = countFilesInDirectory(dirFile);

    FileWordSize fileSpec[totalFiles];
    int totalWords = readFilesAndSum(dirFile, fileSpec);

    // ! DA QUI  
    int countBK = 4;
    int countBK1 = 2;
    int blockcount[countBK], blockcount1[countBK1];

    MPI_Datatype wordtype, filePerProcType, oldType[countBK], oldType1[countBK1];
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
    blockcount[3]  = 1;
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
    MPI_Type_create_struct(countBK1, blockcount1, offsets1, oldType1, &wordtype);
    MPI_Type_commit(&wordtype);
    // ! A QUI

    if (rank == 0)
    {
        printf("Total Words [%d]\n", totalWords);

        elementSplit(wordsForProcessor, totalWords, size);

        for (int i = 0; i < size; i++)
        {
            printf("proc [%d] - processa [%d] parole\n", i, wordsForProcessor[i]);
        }

        for (int i = 0; i < totalFiles; i++)
        {
            printf("file name: %s - %d parole totali\n", fileSpec[i].fileName, fileSpec[i].wordNumber);
        }

        numSplits = wordForProcessor(partitions, wordsForProcessor, fileSpec, size);
        printf("numSplitsxx: %d\n", numSplits);

        for (int i = 0; i < numSplits; i++)
        {
            printf("Struttura %d relativa al file %s --> Proc: %d - Start: %d -> End: %d\n", i,
                   partitions[i].fileName, partitions[i].rank, partitions[i].start, partitions[i].end);
        }

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
            sizeSend = 0;  //quanti elementi
            base = iStruct; // Da dove parto
            while (partitions[iStruct].rank == i)
            {
                iStruct++;
                sizeSend++;
            }
            MPI_Send(&partitions[base], sizeSend, filePerProcType, i, tag, MPI_COMM_WORLD);
            printf("Sending %d to %d struct to %d\n", base, base + sizeSend - 1, i);
        }
        printf("startForzero: %d\n", startForZero);


        sizeOfZero = word_count(words, partitions, startForZero);

        int sizeForProcessor = 0;
        Word *words_received = (Word *)malloc(sizeof(Word) * TOTALWORDS);
        for (int p = 1; p < size; p++)
        {
            MPI_Recv(words_received, TOTALWORDS, wordtype, p, tag, MPI_COMM_WORLD, &stat);
            MPI_Get_count(&stat, wordtype, &sizeForProcessor);
            
            printf("Received %d words from proc %d\n", sizeForProcessor, p);

            //merge words amd word_received
            int j;
            for (int i = 0; i < sizeForProcessor; i++){
                for (j = 0; j < sizeOfZero; j++)
                    if (strcmp(words_received[i].word, words[j].word) == 0)
                    {
                        words[j].freq += words_received[i].freq;
                        break;
                    }
                if (j == sizeOfZero) // La parola non c'è
                {
                    strcpy(words[j].word, words_received[i].word);
                    words[j].freq = words_received[i].freq;
                    sizeOfZero++;
                }
            }
            
        }
        writeResultCSV(words, sizeOfZero);
    }
    else
    {
        int count = 0;
        int word_vec_length = 0;
        MPI_Recv(partitions, SPLIT_PROCESSOR, filePerProcType, source, tag, MPI_COMM_WORLD, &stat);
        MPI_Get_count(&stat, filePerProcType, &count);

        word_vec_length = word_count(words, partitions, count);
        
        MPI_Ssend(words, word_vec_length, wordtype, 0, tag, MPI_COMM_WORLD);
        printf("xxReceived %d words || word_vec_length: %d\n", count, word_vec_length);
    }

    //Parameters for time and result MPI
    double end = MPI_Wtime();
    double totalTimeMPI = end - start;
    printf("Total time ---> %lf seconds\n", totalTimeMPI);

    MPI_Type_free(&wordtype);
    MPI_Type_free(&filePerProcType);
    MPI_Finalize();
}
