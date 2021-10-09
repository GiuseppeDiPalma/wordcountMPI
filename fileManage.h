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
    long int wordNumber;
}FileWordSize;

typedef struct{
    char fileName[MAXFILENAME];
    int rank;
    int start;
    int end;
}PartitionedWord;

long int countWordFile(char *file_name);
long readFilesAndSum(char *path, FileWordSize *fileSpec);
void elementSplit(long *wordForProcessor, long sumWords, int proc);
void wordForProcessor(PartitionedWord *w, long *wordForProcessor, FileWordSize *wordForFile, int proc);