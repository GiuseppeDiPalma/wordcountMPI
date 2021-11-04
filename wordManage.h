#define WORDMAXSIZE 26

typedef struct
{
	char word[WORDMAXSIZE];
	int freq;
}Word;

int createWordStruct(Word *words, PartitionedWord *w, int count);
int copyLineInStruct(Word *w, PartitionedWord *wordSplit, int count);
void wordsCount(Word *w, int length);
void writeResultCSV(Word *w, int size);