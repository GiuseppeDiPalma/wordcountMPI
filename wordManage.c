#include "fileManage.h"
#include "wordManage.h"

int copyLineInStruct(Word *words, PartitionedWord *w, int count)
{
    int n_line = 0;
    for (int p = 0; p <= count; p++)
    {
        FILE *file = fopen(w[p].fileName, "r");
        char c[WORDMAXSIZE];
        int start = w[p].start;
        int end = w[p].end;
        //printf("Proc %d || Sto aprendo file: %s || lineaStart: %d || lineaEnd: %d\n", w[p].rank, w[p].fileName, w[p].start, w[p].end);
        while ((fgets(c, sizeof(c), file) != NULL))
        {
            n_line++;
            if (n_line >= start && n_line <= end)
            {
                char *stringaParola = (char *)malloc(sizeof(char));
                strcpy(stringaParola, c);
                //rimuovo "\n"
                strtok(stringaParola, "\n");
                strcpy(words[p].word, stringaParola);
                printf("linea: %d || parola: %s\n", n_line, words[p].word);
            }
        }
        fclose(file);
    }
    printf("n_line: %d\n", n_line);
    return n_line;
}

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
            //se trova la corrispondenza allroa vado ad aumentare la frequenza di tale parola
            //ovviamente deve continuare a cercare nel caso in cui trova altre parole uguali
            if (strcmp(w[i].word, w[count].word) == 0)
            {
                //essendo parole[a].frequenza = 1 è come se facessi +1
                w[count].freq = w[count].freq + w[i].freq;
                printf("i: %d - parola: %s || freq: %d\n", i, w[count].word, w[count].freq);
            }
        }
    }
}

void writeResultCSV(Word *w, int size)
{
    FILE *csv;
    int n = 0;

    csv = fopen("results.csv", "w+");
    fprintf(csv, "Word:Count");
    for (n = 0; n < size; n++)
    {
        for (int a = n + 1; a < size; a++)
        {
            if (strcmp(w[n].word, w[a].word) == 0)
            {
                w[a].freq = w[n].freq + w[a].freq;
            }
        }
        if (strcmp(w[n].word, "") != 0)
        {
            fprintf(csv, "\n%s,%d\n", w[n].word, w[n].freq);
        }
    }
}