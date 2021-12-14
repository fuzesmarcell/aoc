/*
 * Advent of code
 *
 *
 * File      day_14.c
 * Author    Fuzes Marcell
 * Date      2021.12.14
 */

#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <aoc.h>

typedef struct {
	char pair[3];
	char inserter[2];

	uint64_t count;

} pair_insertion_rule_t;

static int no_pairs_rules;
static pair_insertion_rule_t pair_rules[128];

static void pair_inc(uint64_t n, const char *p) {
	for (int i = 0; i < no_pairs_rules; i++) {
		pair_insertion_rule_t *pair = &pair_rules[i];
		if (pair->pair[0] == p[0] &&
			pair->pair[1] == p[1]) {
			pair->count += n;
			return;
		}
	}

	// unreachable code!!!
	assert(false);
}

static uint64_t letter_table[] = {
	['A'] = 0,
	['Z'] = 0,
};

int main(void) {
	char template[128];
	if (!fgets(template, sizeof(template), stdin)) {
		fprintf(stderr, "Error getting line!");
		return -1;
	}
	int no_templates = (int)strlen(template) - 1;

	char buffer[128];
	while (fgets(buffer, sizeof(buffer), stdin)) {
		char pair[3];
		char inserter[2];

		int no_args_parsed = sscanf(buffer, "%s -> %s", pair, inserter);
		if (no_args_parsed != 2) {
			continue;
		}

		pair_insertion_rule_t *rule = &pair_rules[no_pairs_rules++];
		rule->count = 0;
		strcpy(rule->pair, pair);
		strcpy(rule->inserter, inserter);
	}

	for (int i = 1; i < no_templates; i++) {
		char pair[3] = { template[i - 1], template[i], '\0' };
		pair_inc(1, pair);
	}

	for (int i = 0; i < no_templates; i++) {
		letter_table[template[i]]++;
	}

	for (int i = 0; i < 40; i++) {
		int no_rules_to_inc = 0;
		pair_insertion_rule_t rules_to_inc[512] = { 0 };
		for (int k = 0; k < no_pairs_rules; k++) {
			pair_insertion_rule_t *rule = &pair_rules[k];
			assert(rule->count >= 0);
			if (!rule->count) { continue; }

			letter_table[rule->inserter[0]] += rule->count;

			char pair_a[3] = { rule->pair[0], rule->inserter[0], '\0' };
			char pair_b[3] = { rule->inserter[0], rule->pair[1], '\0' };

			pair_insertion_rule_t *rule_a = &rules_to_inc[no_rules_to_inc++];
			pair_insertion_rule_t *rule_b = &rules_to_inc[no_rules_to_inc++];

			rule_a->count = rule->count;
			strcpy(rule_a->pair, pair_a);

			rule_b->count = rule->count;
			strcpy(rule_b->pair, pair_b);
			
			rule->count -= rule->count;
		}

		for (int k = 0; k < no_rules_to_inc; k++) {
			pair_insertion_rule_t *rule = &rules_to_inc[k];
			pair_inc(rule->count, rule->pair);
		}
	}

	uint64_t min = 0xffffffffffffffff, max = 0;
	for (char c = 'A'; c <= 'Z'; c++) {
		uint64_t n = letter_table[c];
		if (!n) { continue; }
		if (n < min) { min = n; }
		if (n > max) { max = n; }
	}

	printf("Result: %llu\n", max - min);

	return 0;
}