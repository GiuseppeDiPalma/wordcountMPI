#include "fileManage.h"
#include "wordManage.h"
/**
 * @brief Fills the Words structure with the words each processor needs to analyse
 * 
 * @param words struct with words for each processor Word{word,freq}
 * @param w struct with specifications for each processor
 * @param count number of struct w that have same processor
 * @return int number of line for specific processor
 */
int copyLineInStruct(Word *words, PartitionedWord *w, int count)
{
    int n_line = 0;
    int lineCounter = 0;

    int j = 0; //struct words

    for (int p = 0; p < count; p++)
    {
        FILE *file = fopen(w[p].fileName, "r");
        char c[WORDMAXSIZE];
        printf("Proc %d || filename: %s || lineaStart: %d || lineaEnd: %d\n", w[p].rank, w[p].fileName, w[p].start, w[p].end);
        while ((fgets(c, sizeof(c), file) != NULL))
        {
            n_line++;
            if (n_line >= w[p].start && n_line <= w[p].end)
            {
                char *stringaParola = (char *)malloc(sizeof(char));
                strcpy(stringaParola, c);
                strtok(stringaParola, "\n"); //rimuovo "\n"
                //strcpy(words[p].word, stringaParola);
                strcpy(words[j].word, stringaParola);
                words[j].freq = 1;
                free(stringaParola);
                //printf("line: %d || word[%d]: %s\n", n_line, j, words[j].word);
                lineCounter++;
            }
            j++;
        }
        n_line = 0;
        fclose(file);
    }
    return lineCounter;
}

/**
 * @brief Word frequency counts
 * 
 * @param w struct with specifications for each processor
 * @param size struct size
 */
void wordsCount(Word *w, int size)
{
    int count = 0;
    //per ogni parola presente nella struttura dobbiamo contare la frequenza di questa
    for (count = 0; count < size; count++)
    {
        //questo for mi consente di analizzare dalla parola immediatamente successiva a quella che ho
        //con il resto della struttura
        for (int i = count + 1; i < size; i++)
        {
            //se trova la corrispondenza allora vado ad aumentare la frequenza di tale parola
            //ovviamente deve continuare a cercare nel caso in cui trova altre parole uguali
            //printf("Confronto: w[%d].word: %s --> w[%d].word: %s\n", i, w[i].word, count, w[count].word);
            if (strcmp(w[i].word, w[count].word) == 0)
            {
                //essendo parole[a].frequenza = 1 è come se facessi +1
                w[count].freq += w[i].freq;
                //printf("w[%d].word: %s || w[%d].word: %s || w[count].freq: %d\n", i, w[i].word, count, w[count].word, w[count].freq);
                //printf("i: %d - parola: %s || freq: %d\n", i, w[count].word, w[count].freq);
            }
        }
    }
}

/**
 * @brief Counts word occurrences and writes them into the csv file
 * 
 * @param w struct with word and occurrences
 * @param size number of word to write
 */
void writeResultCSV(Word *w, int size)
{
    FILE *csv;

    csv = fopen("results.csv", "w+");
    fprintf(csv, "Word,Count\n");

    for (int n = 0; n < size; n++)
    {
        for (int a = n + 1; a < size; a++)
        {
            if (strcmp(w[n].word, w[a].word) == 0)
            {
                w[a].freq += w[n].freq;
            }
        }
        if (strcmp(w[n].word, "") != 0)
        {
            fprintf(csv, "%s,%d\n", w[n].word, w[n].freq);
        }
    }
}