#include "fileManage.h"
#include "wordManage.h"

#define TOTALWORDS 4000000 // questa variabile dovrebbe essere dinamica

// Find the index of a word into an array of words
int findWord(Word *w, char *word, int n_line)
{
    int i = 0;
    for (i = 0; i < n_line; i++)
    {
        if (strcmp(w[i].word, word) == 0)
        {
            return i;
        }
    }
    return -1;
}

/**
 * @brief Fills the Words structure with the words each processor needs to analyse
 * 
 * @param w struct with words for each processor Word{word,freq}
 * @param wordSplit struct with specifications for each processor
 * @param count number of struct w that have same processor
 * 
 * @return int number of line for specific processor
 */
int word_count(Word *w, PartitionedWord *wordSplit, int count)
{
    int n_line = 0;
    int word_vector_size = 0;

    int index_word = 0; //index of word in file

    for (int p = 0; p < count; p++)
    {
        FILE *file = fopen(wordSplit[p].fileName, "r");
        char c[WORDMAXSIZE];
        while ((fgets(c, sizeof(c), file) != NULL))
        {
            n_line++;
            if (n_line >= wordSplit[p].start && n_line <= wordSplit[p].end)
            {
                strtok(c, "\n"); //rimuovo "\n"
                if ((index_word = findWord(w, c, TOTALWORDS)) == -1)
                {
                    strcpy(w[word_vector_size].word, c);
                    w[word_vector_size].freq = 1;
                    word_vector_size++;
                }
                else
                    w[index_word].freq++;
            }
            
        }
        n_line = 0;
        fclose(file);
    }
    return word_vector_size;
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

    fprintf(csv, "word,frequency\n");
    for (int n = 0; n < size; n++)
        fprintf(csv, "%s,%d\n", w[n].word, w[n].freq);
}