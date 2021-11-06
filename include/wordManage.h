#define WORDMAXSIZE 26

typedef struct
{
	char word[WORDMAXSIZE];
	int freq;
}Word;

int createWordStruct(Word *words, PartitionedWord *w, int count);
int word_count(Word *w, PartitionedWord *wordSplit, int count);
void writeResultCSV(Word *w, int size);