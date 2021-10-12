#include "fileManage.h"
#include "wordManage.h"

int createWordStruct(Word *words, PartitionedWord * w, int count)
{
    for (int p = 0; p < count; p++)
    {
        FILE *file = fopen(w[p].fileName , "r");
        //printf("rank: %d || %s || start: %d || end: %d \n", w[p].rank, w[p].fileName, w[p].start, w[p].end);
        if ( file != NULL )
        {
            char line[WORDMAXSIZE];
            for (int i = w[p].start; i <= w[i].end; i++)
            {
                fgets(line, i, file); /* read a line */
                //printf("%d - %s\n", i, line);
                //words[i].freq=1;
            }
            fclose(file);
        }
        else
        {
            printf("Impossibile aprire il file, non esiste!\n");
        }
    }
    return 0;
}

void copyLineInStruct(Word *words, PartitionedWord * w, int count)
{
    for(int p = 0; p < count; p++)
    {
        //apro il file in lettura
        int n_line = 0;
        FILE *file = fopen(w[p].fileName , "r");
        printf("Sto aprendo file %s\n", w[p].fileName);
        char c[WORDMAXSIZE];
        int start = w[p].start;
        int end = w[p].end;
        while ((fgets(c, sizeof(c), file) != NULL))
        {
            n_line++;
            if(n_line >= start && n_line <= end)
            {
                char *stringaParola = (char *)malloc(sizeof(char));
                strcpy(stringaParola, c);
                strtok(stringaParola, "\n"); //rimuovo "\n"
                strcpy(words[p].word, stringaParola);
                //pause();
                printf("linea: %d || parola: %s\n", n_line, stringaParola);
                //n_line++;
            }
        }
        fclose(file);
        //pause();
    }
}

void wordsCount(Word *w, int length)
{
    int count=0;
    //per ogni parola presente nella struttura dobbiamo contare la frequenza di questa
    for(count = 0 ; count < length ; count++)
    {
        //questo for mi consente di analizzare dalla parola immediatamente successiva a quella che ho
        //con il resto della struttura
        for(int i = count+1; i < length; i++)
        {
            //se trova la corrispondenza allroa vado ad aumentare la frequenza di tale parola
            //ovviamente deve continuare a cercare nel caso in cui trova altre parole uguali
            if(strcmp(w[i].word, w[count].word) == 0)
            {
                //essendo parole[a].frequenza = 1 è come se facessi +1
                w[count].freq = w[count].freq+w[i].freq; 
                printf("parola: %s || freq: %d", w[count].word, w[count].freq);
            }
        }
    }
}