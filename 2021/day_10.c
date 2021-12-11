/*
 * Advent of code common code
 *
 *
 * File      day_10.c
 * Author    Fuzes Marcell
 * Date      2021.12.10
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdbool.h>
#include <inttypes.h>

static bool is_open_bracket(char c) {
	return (c == '{' || c == '[' || c == '<' || c == '(');
}

static bool is_closing_bracket(char c) {
	return (c == '}' || c == ']' || c == '>' || c == ')');
}

static char open_to_close_bracket(char c) {
	return (c == '(') ? c + 1 : c + 2;
}

int score_table[] = {
	[')'] = 3,
	[']'] = 57,
	['}'] = 1197,
	['>'] = 25137,
};

int score_table_part2[] = {
	[')'] = 1,
	[']'] = 2,
	['}'] = 3,
	['>'] = 4,
};

int cmpfunc(const void* a, const void* b) {
	int64_t aa = *(int64_t*)a;
	int64_t bb = *(int64_t*)b;
	if (aa - bb < 0) { return -1; }
	if (aa - bb > 0) { return  1; }
	return 0;
}

int main(void) {

	uint64_t result = 0;
	int score_count = 0;
	int64_t scores[0xff] = { 0 };
	char buffer[1024];
	while (fgets(buffer, sizeof(buffer), stdin)) {
		int raw_len = (int)strlen(buffer);
		int len = (buffer[raw_len - 1] == '\n') ? raw_len - 1 : raw_len;

		int count = 0;
		char brackets[1024] = { 0 };
		bool syntax_error = false;
		for (int i = 0; i < len; i++) {
			char c = buffer[i];

			if (is_open_bracket(c)) {
				assert(count + 1 < sizeof(brackets));
				brackets[count++] = c;
			}
			else if (is_closing_bracket(c)) {
				assert((count - 1) >= 0);
				char closing = open_to_close_bracket(brackets[--count]);
				brackets[count] = '\0';
				if (closing != c) {
					printf("Syntax error. Expected %c, but found %c instead.\n", closing, c);
					assert(c == ')' || c == ']' || c == '}' || c == '>');
					result += score_table[c];
					syntax_error = true;
					break;
				}
			}
		}

		if (!syntax_error) { // incomplete line
			uint64_t score = 0;

			while (--count >= 0) {
				char c = open_to_close_bracket(brackets[count]);
				assert(c == ')' || c == ']' || c == '}' || c == '>');
				score *= 5;
				score += score_table_part2[c];
			}

			scores[score_count++] = score;
		}
	}

	assert(score_count % 2 != 0);
	qsort(scores, score_count, sizeof(int64_t), cmpfunc);

	printf("Part 01: %llu\n", (long long unsigned int)result);
	printf("Part 02: %lld\n", (long long int)scores[score_count / 2]);

	return 0;
}