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

/**
 * @brief read all file in a directory, fill a struct with fileName e number of words
 * for any file, after this return sum of all words in all file.
 * 
 * @param path target directory path
 * @param fileSpec struct with file specification
 * @return long sum of all words in all file
 */
long readFilesAndSum(char *path, FileWordSize *fileSpec)
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
                fileSpec[i].wordNumber = countWordFile(mainPath);
                wordSum = wordSum += fileSpec[i].wordNumber;
                //printf("\til file: %s ---> ha %ld words\n", fileSpec[i].fileName, fileSpec[i].wordNumber);
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
 * @brief It fills an array, where at each position is the number 
 * of word that a process will have to compute. 
 * If the number is not divisible some position will be oversized.
 *  
 * @param wordForProcessor: array of number of word per processor
 * @param sumWords: sum of all word of files in directory
 * @param proc: numer of processor in execution
 */
void elementSplit(long *wordForProcessor, long sumWords, int proc)
{

    int mod = sumWords % proc;
    //printf("sumWords: %ld || proc: %d || MOD: %ld\n", sumWords, proc, mod);

    if (mod == 0)
    {
        for (int i = 0; i < proc; i++)
        {
            wordForProcessor[i] = sumWords / proc;
        }
    }
    else
    {
        for (int i = 0; i < proc; i++)
        {
            wordForProcessor[i] = sumWords / proc;
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

void wordForProcessor(PartitionedWord *w, long *wordForProcessor, FileWordSize *wordForFile, int proc)
{
    int j = 0; //stuttura
    int k = 0; //file
    int i = 0;
    signed long diff = 0;
    
    long surplus = 0;

    while(i < proc)
    { 
        w[j].rank=i;
        w[j].start = surplus;
        signed long diff = wordForProcessor[i]-wordForFile[k].wordNumber;   
        if (diff >= 0 && k<10)
        {
            //printf("1= Il processore %d ha %ld parole da processare", i, wordForProcessor[i]);
            wordForProcessor[i]=wordForProcessor[i]-wordForFile[k].wordNumber;
            w[j].end = wordForFile[k].wordNumber + surplus;
            strcpy(w[j].fileName, wordForFile[k].fileName);
            //printf(" in %s || Inizio %d, Fine %d || restano altre %ld\n", w[j].fileName, w[j].start, w[j].end, wordForProcessor[i]);
            surplus = 0;
            j++; k++;
        }
        else
        {
            signed long diff_2 = 0;
            diff_2 = wordForFile[k].wordNumber - wordForProcessor[i];

            if(diff_2 < 0)
            {
                w[j].end = wordForFile[k].wordNumber;
            }
            else
            {
                wordForFile[k].wordNumber = wordForFile[k].wordNumber - wordForProcessor[i];
                surplus = wordForProcessor[i] + 1;
                w[j].end = wordForProcessor[i];
                //printf("2= Il process %d ha taglia rimanente %ld\n", i, wordForProcessor[i]);
                wordForProcessor[i] = 0;
            }
            i++;
        }
    }
}