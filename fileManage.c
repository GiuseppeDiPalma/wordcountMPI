#include "fileManage.h"

long int countWordFile(char *file_name)
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

    if ((dir = opendir(path)) != NULL)
    {
        while ((d = readdir(dir)) != NULL)
        {
            if (strcmp(d->d_name, "..") != 0 && strcmp(d->d_name, ".") != 0)
            {
                char *mainPath = (char *)malloc(sizeof(char));
                strcpy(mainPath, path);
                strcat(mainPath, d->d_name);
                strcpy(fileSpec[i].fileName, mainPath);
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
void elementSplit(int *wordForProcessor, long sumWords, int proc)
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

        int j = 0;

        while (mod != 0)
        {
            int module2 = j % proc;
            wordForProcessor[module2] = wordForProcessor[module2] + 1;
            mod--;
            j++;
        }
    }
}

/**
 * @brief Questa funzione deve riempire la struttura da inviare ad ogni processore per la computazione.
 *        La struttura deve contenere i campi RANK, FILE_NAME, START, END, che poi saranno passati al processore.
 * 
 * @param w Struttura che sarà riempita e inviata ai processi
 * @param wordForProcessor Array di numeri che contiene in ogni i-esima posizione il numero di parole che l'i-esimo processore deve analizzare
 * @param fileInfo Struttura che contiene le info di un file, path del file e il numero di parole contenuto in esso.
 * @param proc Numero di processori che effettuano la computazione
 */
void wordForProcessor(PartitionedWord *w, int *wordForProcessor, FileWordSize *fileInfo, int proc, int numeroFile)
{
    int i = 0; //processi
    int j = 0; //stuttura
    int k = 0; //file

    int offset = 0;

    while (i < proc)
    {
        w[j].rank = i;
        w[j].start = offset;
        int diff = wordForProcessor[i] - fileInfo[k].wordNumber;
        //printf("w[%d].rank: %d || w[%d].start: %d ||diff: %d\n", j, i, j, offset, diff);
        if (diff >= 0) 
        {
            //printf("processo %d deve analizzare %d", i, wordForProcessor[i]);

            wordForProcessor[i] = wordForProcessor[i] - fileInfo[k].wordNumber + offset;
            w[j].end = fileInfo[k].wordNumber;
            strcpy(w[j].fileName, fileInfo[k].fileName);

            //printf(", con file %s, parto da %d, arrivo a %d, ancora da analizzare %d\n", w[j].fileName, w[j].start, w[j].end, wordForProcessor[i]);

            offset = 0;
            j++;
            k++;
            
            if(wordForProcessor[i] == 0)
                i++;
        }
        else
        {
            //printf("processo %d deve analizzare %d", i, wordForProcessor[i]);

            // fileInfo[k].wordNumber = fileInfo[k].wordNumber - wordForProcessor[i];
            w[j].end = wordForProcessor[i] + offset - 1;
            offset = w[j].end + 1;
            wordForProcessor[i] = 0;

            //printf(", con file %s, parto da %d, arrivo a %d, ancora da analizzare %d\n", fileInfo[k].fileName, w[j].start, w[j].end, wordForProcessor[i]);
            
            strcpy(w[j].fileName, fileInfo[k].fileName);

            i++;
            j++;
        }
    }
}