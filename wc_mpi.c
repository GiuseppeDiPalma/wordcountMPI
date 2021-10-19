#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <mpi.h>

#include "utils.h"
#include "fileManage.h"
#include "wordManage.h"

#define TOTALWORDS 400000 // questa variabile dovrebbe essere dinamica
#define SPLIT_PROCESSOR 100

void main(int argc, char *argv[])
{
    int rank, size;
    int tag = 1;
    int source = 0;


    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Request rq = MPI_REQUEST_NULL;
    MPI_Status stat;
    
    //Parameters for time MPI
    double start = MPI_Wtime();

    printf("(START) - MPI WORD COUNT - (START)\n\n");
    
    char *dirFile;
    int wordsForProcessor[size];
    PartitionedWord n_words[SPLIT_PROCESSOR]; //array di struct delle parole per ogni processo con start e end
    //Word words[TOTALWORDS]={" ",0}; //cercare di rendere dinamico questo row

    Word *wds=(Word *) malloc(sizeof(Word));

    parse_arg(argc, argv, &dirFile);
    int numFiles = countFilesInDirectory(dirFile);
    FileWordSize fileSpec[numFiles];
    int sumWord = readFilesAndSum(dirFile, fileSpec);

    int count=3;
    MPI_Datatype wordtype, filePerProcType, oldtypes[count], oldtypes1[2];
    int blockcounts[count], blockcounts1[2];
    MPI_Aint offsets[count], offsets1[2], lb, extent;

    offsets[0] = offsetof(PartitionedWord, rank);
    oldtypes[0] = MPI_INT;
    blockcounts[0] = 1;
    MPI_Type_get_extent(MPI_INT, &lb, &extent);

    offsets[1] = offsetof(PartitionedWord, start);
    oldtypes[1] = MPI_LONG;
    blockcounts[1] = 2;
    MPI_Type_get_extent(MPI_LONG, &lb, &extent);

    offsets[2] = offsetof(PartitionedWord, fileName);
    oldtypes[2] = MPI_CHAR;
    blockcounts[2] = 16;
    MPI_Type_get_extent(MPI_CHAR,&lb,&extent);

    //create type struct
    MPI_Type_create_struct(count, blockcounts, offsets, oldtypes, &filePerProcType);
    MPI_Type_commit(&filePerProcType); 

    offsets1[0] = 0;
    oldtypes1[0] = MPI_CHAR;
    blockcounts1[0] = WORDMAXSIZE;
    
    // space first field and second field
    MPI_Type_get_extent(MPI_CHAR, &lb, &extent);
    
    // save offset value after first field
    offsets1[1] = offsetof(Word, word);
    oldtypes1[1] = MPI_INT;
    blockcounts1[1] = 1;

    // create struct with fields
    MPI_Type_create_struct(2, blockcounts1, offsets1, oldtypes1, &wordtype);
    MPI_Type_commit(&wordtype);

    if(rank==0)
    {
        printf("(START) - MASTER(#%d) - (START)\n", rank);



        printf("Total Words [%d]\n", sumWord);

        elementSplit(wordsForProcessor, sumWord, size);

        for (int i = 0; i < numFiles; i++)
        {
            printf("file name: %s - %d parole totali\n", fileSpec[i].fileName, fileSpec[i].wordNumber);
        }

        for (int i = 0; i < size; i++)
        {
            printf("proc [%d] - processa [%d] parole\n", i, wordsForProcessor[i]);
        }
        printf("\n");

        wordForProcessor(n_words, wordsForProcessor, fileSpec, size, numFiles);

        for(int i = 0; i < 8; i++)
        {
            printf("Proc [%d] - FileName [%s] - Start [%d] - End [%d]\n", n_words[i].rank, n_words[i].fileName, n_words[i].start, n_words[i].end);
        }

        int k = 0; //indice di dove mi trovo all'interno della struttura
        int startper0 = 0;
        int grandezzaperzero = 0;
        
        //celle da passare a processo 1...
        while(n_words[k].rank==0)
        {
            k++;
            startper0++;
        }
        
        int q = startper0; //da dove devo partire
        for (int i=1 ; i < size; i++)
        {
            int j=0; //quanti elementi
            while(n_words[k].rank == i)
            {
                k++;
                j++;
            }
            MPI_Send(&n_words[q], j, filePerProcType, i, tag, MPI_COMM_WORLD);
            q=k;
        }

        printf("startper0: %d\n", startper0);
        grandezzaperzero = copyLineInStruct(wds, n_words, startper0);
        
        wordsCount(wds, grandezzaperzero);

        int grandezzaprocessi=0;
        int start2=grandezzaperzero;
        //int quant=row-start2; // questo row è il il numero di words massimo, ma io l'ho fatto dinamico

        for(int p = 1; p < size; p++)
		{
            //MPI_Recv(&wds[start2], quant, wordtype, p, tag, MPI_COMM_WORLD, &stat);
            //MPI_Get_count(&stat, wordtype, &grandezzaprocessi);
            //start2=start2+grandezzaprocessi;
            //quant=row-start2;
        }
        //writeResultCSV(wds, grandezzaperzero);


        printf("\n(END) - MASTER(#%d) - (END)\n", rank);
    }
    else
    {
        //printf("(START) - SLAVE(#%d) - (START)\n", rank);
        //int count=0;
        //int grandezzaStruttura=0;
        //MPI_Recv(n_words, SPLIT_PROCESSOR , filePerProcType, source, tag, MPI_COMM_WORLD, &stat);
        //MPI_Get_count(&stat, filePerProcType, &count);
        //grandezzaStruttura=creaStrutturaParole(wds,n_words,count);
        //contaOccorrenze(wds,grandezzaStruttura);
        //MPI_Ssend(wds, grandezzaStruttura, wordtype, 0, tag, MPI_COMM_WORLD); 
        //printf("(END) - SLAVE(#%d) - (END)\n", rank);
    }

    printf("(END) - MPI WORD COUNT - (END)\n\n");

    //Parameters for time and result MPI
    double end = MPI_Wtime();
    double totalTimeMPI = end - start;
    printf("Total time ---> %lf seconds\n", totalTimeMPI);
    
    MPI_Type_free(&wordtype);
    MPI_Type_free(&filePerProcType);
    MPI_Finalize();
}