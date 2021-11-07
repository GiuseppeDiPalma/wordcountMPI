#define TOTALWORDS 4000000
#define WORDMAXSIZE 26

typedef struct
{
	char word[WORDMAXSIZE];
	int freq;
}Word;

int createWordStruct(Word *words, PartitionedWord *w, int count);
int wordCount(Word *w, PartitionedWord *wordSplit, int count);
int mergeWords(Word *words, Word *words_received, int sizeForProcessor, int sizeOfZero);
void writeResultCSV(Word *w, int size);