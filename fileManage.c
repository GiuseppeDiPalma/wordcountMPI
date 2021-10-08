#include "fileManage.h"

long int countWordFile(char *file_name)
{
    char ch;
    int word_count = 1;
    int in_word = 0;
    FILE *fp;

    fp = fopen(file_name, "r");

    if(fp == NULL) {
        printf("Could not open the file %s\n", file_name);
        return 1;
    }

    while ((ch = fgetc(fp)) != EOF) 
    {
        if(ch == ' ' || ch == '\t' || ch == '\0' || ch == '\n') 
        {
            if (in_word) 
            {
                in_word = 0;
                word_count++;
            }

        }
            else
            {
                in_word = 1;
            }
  }
  return word_count;
}

long readFiles(char *path, FileRowSize *fileSpec)
{
    DIR *dir;
    struct dirent *d;

    long int wordSum = 0;
    int i = 0;
    
    if((dir = opendir(path)) != NULL)
    {
        while((d = readdir(dir)) != NULL)
        {
            if(strcmp(d->d_name, "..")!=0 && strcmp(d->d_name, ".")!=0)
            {
                strcpy(fileSpec[i].fileName, d->d_name);
                char *mainPath = (char *)malloc(sizeof(char));
                strcpy(mainPath, path);
                strcat(mainPath, d->d_name);
                fileSpec[i].rowSize = countWordFile(mainPath);
                wordSum = wordSum += fileSpec[i].rowSize;
                printf("\til file: %s ---> ha %ld words\n", fileSpec[i].fileName, fileSpec[i].rowSize);
                i++;
            }
        }
        closedir(dir);
    }
    else
    {
        perror("Could not open directory!");
        exit(0);
    }

    return wordSum;
}

/**
 * Function:  elementSplit|| 
 *  
 * It fills an array, where at each position is the number 
 * of bytes that a process will have to compute. 
 * If the number is not divisible some position will be oversized.
 *  
 * @param wordForProcessor: array of number of bytes per processor
 * @param fileSize: sum of all bytes of files in directory
 * @param proc: numer of processor in execution
 */
void elementSplit(long *wordForProcessor, long fileSize, int proc)
{

    int mod = fileSize % proc;

    if (mod == 0)
    {
        for (int i = 0; i < proc; i++)
        {
            wordForProcessor[i] = fileSize / proc;
        }
    }
    else
    {
        for (int i = 0; i < proc; i++)
        {
            wordForProcessor[i] = fileSize / proc;
        }

        int j=0;

        while(mod != 0)
        {
            int module2 = j % proc;
            wordForProcessor[module2] = wordForProcessor[module2] + 1;
            mod--; j++;
        }
    }
}