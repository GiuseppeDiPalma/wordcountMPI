#include <dirent.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <sys/stat.h>
#include <string.h>
#include <stdbool.h>
#include <dirent.h>

#define MAXFILENAME 50

typedef struct
{
    char fileName[MAXFILENAME];
    int wordNumber;
}FileWordSize;

typedef struct
{
    char fileName[MAXFILENAME];
    int rank;
    int start;
    int end;
}PartitionedWord;

long int countWordFile(char *file_name);
long readFilesAndSum(char *path, FileWordSize *fileSpec);
void elementSplit(int *wordForProcessor, long sumWords, int proc);
int wordForProcessor(PartitionedWord *w, int *wordForProcessor, FileWordSize *wordForFile, int proc, int numeroFile);

void preProcessingFile(char *path);