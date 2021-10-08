#include <dirent.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <sys/stat.h>
#include <string.h>
#include <stdbool.h>  

#define MAXFILENAME 50

typedef struct{
    char fileName[MAXFILENAME];
    long int rowSize;
}FileRowSize;

typedef struct{
    int start;
    int end;
    char fileName[MAXFILENAME];
    int rank;
}PartitionedRow;

long int countWordFile(char *file_name);
long readFiles(char *path, FileRowSize *fileSpec);
void elementSplit(long *wordForProcessor, long sumWords, int proc);