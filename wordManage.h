#define WORDMAXSIZE 26

typedef struct {
	char word[WORDMAXSIZE];
	int freq;
}Word;

int creaStrutturaParole(Word *words, PartitionedWord * w, int count);