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

int main() {

	char buffer[512];
	char* line;

	size_t result = 0;
	while (line = fgets(buffer, sizeof(buffer), stdin), line != NULL) {
		char* tok = strtok(line, ":|");
		int card;
		sscanf(tok, "Card %d:", &card);

		tok = strtok(NULL, ":|");
		char* w = strdup(tok);

		tok = strtok(NULL, ":|");
		char* o = strdup(tok);

		Cards winning = { 0 };
		cards_parse(w, &winning);

		Cards our = { 0 };
		cards_parse(o, &our);

		free(w);
		free(o);

		int points = 0;
		int n = 0;
		for (int i = 0; i < our.ncards; i++) {
			for (int j = 0; j < winning.ncards; j++) {
				if (our.cards[i] == winning.cards[j]) {
					points = (int)pow(2, (double)n);
					++n;
				}
			}
		}

		result += points;
	}

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