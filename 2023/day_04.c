#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>

#define MAX_CARDS 100

typedef struct {
	int ncards;
	int cards[MAX_CARDS];
} Cards;

static inline void cards_parse(char* str, Cards* card);

typedef struct {
	int nmatches;
	int n;
	Cards winning;
	Cards ours;
} Scratch;

#define MAX_SCRATCH 500

static int nscratch;
static Scratch cards[MAX_SCRATCH];

int main() {

	char buffer[512];
	char* line;

	size_t result1 = 0;
	while (line = fgets(buffer, sizeof(buffer), stdin), line != NULL) {
		char* tok = strtok(line, ":|");
		int card;
		sscanf(tok, "Card %d:", &card);

		tok = strtok(NULL, ":|");
		char* w = strdup(tok);

		tok = strtok(NULL, ":|");
		char* o = strdup(tok);

		assert(nscratch + 1 < MAX_SCRATCH);

		Scratch* scratch = &cards[nscratch++];
		scratch->nmatches = 0;
		scratch->n = 1;
		cards_parse(w, &scratch->winning);
		cards_parse(o, &scratch->ours);

		free(w);
		free(o);

		int points = 0;
		int n = 0;
		for (int i = 0; i < scratch->ours.ncards; i++) {
			for (int j = 0; j < scratch->winning.ncards; j++) {
				if (scratch->ours.cards[i] == scratch->winning.cards[j]) {
					points = (int)pow(2, (double)n);
					++n;
				}
			}
		}

		result1 += points;
		if (points)
			scratch->nmatches = n;
	}

	for (int i = 0; i < nscratch; i++) {
		const Scratch* scratch = &cards[i];

		if (!scratch->n)
			continue;

		for (int j = 0; j < scratch->n; j++) {
			for (int k = 0; k < scratch->nmatches; k++) {
				int idx = i + 1 + k;
				if (idx < nscratch)
					++cards[idx].n;
			}
		}
	}

	size_t result2 = 0;
	for (int i = 0; i < nscratch; i++)
		result2 += cards[i].n;

	return 0;
}

void cards_parse(char* str, Cards* card) {
	char* tok = strtok(str, " ");
	while (tok != NULL) {
		assert(card->ncards + 1 < MAX_CARDS);
		card->cards[card->ncards++] = atoi(tok);
		tok = strtok(NULL, " ");
	}
}