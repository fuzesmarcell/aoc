#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <assert.h>
#include <stdbool.h>

typedef enum {
	CARD_NIL,

	CARD_J,
	CARD_2,
	CARD_3,
	CARD_4,
	CARD_5,
	CARD_6,
	CARD_7,
	CARD_8,
	CARD_9,
	CARD_T,
	CARD_Q,
	CARD_K,
	CARD_A,

	CARD_COUNT,
} CardType;


typedef enum {
	HAND_NIL,

	HAND_HIGH_CARD,
	HAND_ONE_PAIR,
	HAND_TWO_PAIR,
	HAND_THREE_OF_A_KIND,
	HAND_FULL_HOUSE,
	HAND_FOUR_OF_A_KIND,
	HAND_FIVE_OF_A_KIND,

	HAND_COUNT,
} HandType;

typedef struct {
	HandType type;
	CardType cards[5];
	int rank;
	int bid;
} Hand;

#define MAX_HANDS 2000
static Hand hands[MAX_HANDS];
static int nhands;

static CardType to_card(char c);
static HandType determine_hand(const CardType hcards[5]);
static HandType compute_joker_hand(HandType hand, const CardType cards[5]);

int cmpfunc_type(const void* a, const void* b) {
	const Hand* A = a;
	const Hand* B = b;
	return (int)A->type - B->type;
}

int cmpfunc_strength(const void* a, const void* b) {
	const Hand* A = a;
	const Hand* B = b;

	for (int i = 0; i < 5; i++) {
		if (A->cards[i] > B->cards[i])
			return 1;
		else if (A->cards[i] < B->cards[i])
			return -1;
	}

	return 0;
}

int main() {

	char buf[256];
	char* line;

	while (line = fgets(buf, sizeof(buf), stdin), line != NULL) {
		char* tok = strtok(line, " ");

		Hand* h = &hands[nhands++];
		for (int i = 0; i < 5; i++)
			h->cards[i] = to_card(tok[i]);

		tok = strtok(NULL, " ");
		h->bid = atoi(tok);

		h->type = determine_hand(&h->cards[0]);
		h->type = compute_joker_hand(h->type, h->cards);
	}

	qsort(hands, nhands, sizeof(Hand), cmpfunc_type);

	for (int i = 0; i < nhands;) {
		Hand* base = &hands[i];
		int j;
		for (j = i; j < nhands; j++)
			if (base->type != hands[j].type)
				break;

		int range = j - i;
		if (range > 1) // we need to sort
			qsort(base, range, sizeof(Hand), cmpfunc_strength);

		i += range;
	}

	size_t result1 = 0;
	for (int i = 0; i < nhands; i++)
		result1 += (size_t)hands[i].bid * (i+1);

	return 0;
}

CardType to_card(char c) {
	if (isdigit(c)) {
		return (c - '2') + 2;
	}

	switch (c) {
	case 'T': return CARD_T;
	case 'J': return CARD_J;
	case 'Q': return CARD_Q;
	case 'K': return CARD_K;
	case 'A': return CARD_A;
	default: assert(0); break;
	}

	return CARD_NIL;
}

HandType determine_hand(const CardType hcards[5]) {
	//
	// Pairs or x of a kind cards remain
	//
	int cards[CARD_COUNT] = { 0 };
	for (int i = 0; i < 5; i++)
		++cards[hcards[i]];

	int pairs = 0;
	int triplets = 0;
	int quads = 0;
	int pentas = 0;

	for (int i = 0; i < CARD_COUNT; i++) {
		if (cards[i] == 2)
			++pairs;
		else if (cards[i] == 3)
			++triplets;
		else if (cards[i] == 4)
			++quads;
		else if (cards[i] == 5)
			++pentas;
	}

	assert(triplets == 0 || triplets == 1);
	assert(pairs == 0 || pairs == 1 || pairs == 2);
	assert(pentas == 0 || pentas == 1);
	assert(quads == 0 || quads == 1);

	HandType type = HAND_NIL;
	if (pentas)
		type = HAND_FIVE_OF_A_KIND;
	else if (quads)
		type = HAND_FOUR_OF_A_KIND;
	else if (triplets && pairs)
		type = HAND_FULL_HOUSE;
	else if (triplets)
		type = HAND_THREE_OF_A_KIND;
	else if (pairs == 2)
		type = HAND_TWO_PAIR;
	else if (pairs == 1)
		type = HAND_ONE_PAIR;
	else
		type = HAND_HIGH_CARD;

	return type;
}

HandType compute_joker_hand(HandType hand, const CardType cards[5]) {
	CardType mcards[5];
	mcards[0] = cards[0];
	mcards[1] = cards[1];
	mcards[2] = cards[2];
	mcards[3] = cards[3];
	mcards[4] = cards[4];

	int num_jokers = 0;
	for (int i = 0; i < 5; i++) {
		if (mcards[i] == CARD_J)
			++num_jokers;
	}

	if (num_jokers == 1) {
		int jp = -1;
		for (jp = 0; jp < 5; jp++)
			if (cards[jp] == CARD_J)
				break;

		assert(jp != -1);

		for (int i = CARD_2; i < CARD_COUNT; i++) {
			mcards[jp] = i;

			HandType type = determine_hand(mcards);
			if (type > hand)
				hand = type;
		}

		return hand;
	}
	else if (num_jokers == 2) {
		int njp = 0;
		int jp[2] = { -1, -1 };

		for (int i = 0; i < 5; i++) {
			if (cards[i] == CARD_J) {
				jp[njp++] = i;
				if (njp == 2)
					break;
			}
		}

		assert(jp[0] != -1);
		assert(jp[1] != -1);

		for (int i = CARD_2; i < CARD_COUNT; i++) {
			for (int j = CARD_2; j < CARD_COUNT; j++) {
				mcards[jp[0]] = i;
				mcards[jp[1]] = j;

				HandType type = determine_hand(mcards);
				if (type > hand)
					hand = type;
			}
		}

		return hand;
	}
	else if (num_jokers == 3) {
		int njp = 0;
		int jp[3] = { -1, -1, -1 };

		for (int i = 0; i < 5; i++) {
			if (cards[i] == CARD_J) {
				jp[njp++] = i;
				if (njp == 3)
					break;
			}
		}

		assert(jp[0] != -1);
		assert(jp[1] != -1);
		assert(jp[2] != -1);

		for (int i = CARD_2; i < CARD_COUNT; i++) {
			for (int j = CARD_2; j < CARD_COUNT; j++) {
				for (int k = CARD_2; k < CARD_COUNT; k++) {
					mcards[jp[0]] = i;
					mcards[jp[1]] = j;
					mcards[jp[2]] = k;

					HandType type = determine_hand(mcards);
					if (type > hand)
						hand = type;
				}
			}
		}

		return hand;
	}
	else if (num_jokers == 4) {
		return HAND_FIVE_OF_A_KIND;
	}

	return hand;
}
