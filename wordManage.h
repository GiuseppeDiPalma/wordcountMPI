#define WORDMAXSIZE 26

typedef struct {
	char word[WORDMAXSIZE];
	int freq;
}Word;

int createWordStruct(Word *words, PartitionedWord * w, int count);
void wordsCount(Word *w, int length);
void copyLineInStruct(Word *words, PartitionedWord * w, int count);