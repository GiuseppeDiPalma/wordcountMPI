#include "fileManage.h"
/**
 * @brief Count number of words in each file.
 * 
 * @return int total number of worlds in file
 */
int countWordFile(char *file_name)
{
    char ch;
    int word_count = 1;
    int in_word = 0;
    FILE *fp;

    fp = fopen(file_name, "r");

    if (fp == NULL)
    {
        printf("Could not open the file %s\n", file_name);
        return 1;
    }

    while ((ch = fgetc(fp)) != EOF)
    {
        if (ch == ' ' || ch == '\t' || ch == '\0' || ch == '\n')
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
 * for any file, after this return sum of total words in all file.
 * 
 * @param path target directory path
 * @param fileSpec struct with file specification
 * @return int sum of all words in all file
 */
int readFilesAndSum(char *path, FileWordSize *fileSpec)
{
    DIR *dir;
    struct dirent *d;

    int wordSum = 0;
    int i = 0;

    if ((dir = opendir(path)) != NULL)
    {
        while ((d = readdir(dir)) != NULL)
        {
            if (strcmp(d->d_name, "..") != 0 && strcmp(d->d_name, ".") != 0)
            {
                char *mainPath = (char *)malloc(sizeof(char) * 256);
                strcpy(mainPath, path);
                strcat(mainPath, d->d_name);
                strcpy(fileSpec[i].fileName, mainPath);
                fileSpec[i].wordNumber = countWordFile(mainPath);
                wordSum = wordSum += fileSpec[i].wordNumber;
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
void elementSplit(int *wordForProcessor, long sumWords, int proc)
{
    int mod = sumWords % proc;
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

        int j = 0;
        while (mod != 0)
        {
            int module2 = j % proc;
            wordForProcessor[module2] = wordForProcessor[module2] + 1;
            mod--; j++;
        }
    }
}

/**
 * @brief fill structure to be sent to each processor for computation.
 * 
 * @param w Structure that will be filled and sent to the processes.
 * @param wordForProcessor Array of numbers containing in each position the number of words to be analysed.
 * @param fileInfo Structure whit file spec.
 * @param proc Number of processor in computation.
 * 
 * @return number of total split
 */
int wordForProcessor(PartitionedWord *w, int *wordForProcessor, FileWordSize *fileInfo, int proc)
{
    int i = 0;//processor
    int j = 0;//structure
    int k = 0;//file

    int offset = 0;

    while (i < proc)
    {
        w[j].rank = i;
        w[j].start = offset;
        int diff = wordForProcessor[i] - (fileInfo[k].wordNumber - offset);
        if (diff >= 0) 
        {
            wordForProcessor[i] = wordForProcessor[i] - fileInfo[k].wordNumber + offset;
            w[j].end = fileInfo[k].wordNumber - 1;
            strcpy(w[j].fileName, fileInfo[k].fileName);
            offset = 0;
            j++; k++;
            if(wordForProcessor[i] == 0)
                i++;
        }
        else
        {
            w[j].end = wordForProcessor[i] + offset - 1;
            offset = w[j].end + 1;
            wordForProcessor[i] = 0;
            strcpy(w[j].fileName, fileInfo[k].fileName);
            i++; j++;
        }
    }
    return j;
}