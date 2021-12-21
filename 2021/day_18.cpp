/*
 * Advent of code
 *
 * File      day_18.cpp
 * Author    Fuzes Marcell
 * Date      2021.12.18
 */
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <ctype.h>

#include <vector>

#include <aoc.h>

#define ASSERT(expr) do { if (!(expr)) { printf("Assertion %s:%d -> %s\n", __FILE__, __LINE__, #expr); __debugbreak(); } }while(0);

enum {
	OPEN_BRACKET = 0xffffffff,
	CLOSE_BRACKET = 0xfffffff,
};

static bool is_number(int c) {
	return (c != OPEN_BRACKET) && (c != CLOSE_BRACKET);
}

static void explode(std::vector<int>& numbers, int left_idx, int right_idx) {
	for (int i = left_idx - 1; i >= 0; i--) {
		int c = numbers[i];
		if (is_number(c)) {
			numbers[i] += numbers[left_idx + 1];
			break;
		}
	}

	for (int i = right_idx + 1; i < numbers.size(); i++) {
		int c = numbers[i];
		if (is_number(c)) {
			numbers[i] += numbers[right_idx - 1];
			break;
		}
	}

	numbers.insert(numbers.begin() + right_idx + 1, 0);
	// remove the four elements of a pair [ n n ]
	for (int i = 0; i < 4; i++) {
		numbers.erase(numbers.begin() + left_idx);
	}
}

static void split(std::vector<int>& numbers, int idx) {
	float n = (float)numbers[idx] / 2.0f;
	int a = (int)floorf(n);
	int b = (int)ceilf(n);

	numbers.erase(numbers.begin() + idx);
	numbers.insert(numbers.begin() + idx, OPEN_BRACKET);
	numbers.insert(numbers.begin() + idx + 1, a);
	numbers.insert(numbers.begin() + idx + 2, b);
	numbers.insert(numbers.begin() + idx + 3, CLOSE_BRACKET);
}

static void print_numbers(std::vector<int>& numbers) {
	int color_counter = 0;
	for (int c : numbers) {
		if (c == OPEN_BRACKET) {
			printf("[");
		}
		else if (c == CLOSE_BRACKET) {
			printf("]");
		}
		else {
			if (color_counter % 2 == 0) {
				printf(KRED "%d" RESET, c);
			}
			else {
				printf(KBLU "%d" RESET, c);
			}
			color_counter++;
		}
	}
	printf("\n");
}

static void read_into_vec(const char* input, std::vector<int>& numbers, bool sourround = false) {
	for (const char* at = input; *at; at++) {
		char c = *at;
		if (c == ',') { continue; }
		if (isdigit(c)) {
			numbers.push_back(c - '0');
		}
		else {
			ASSERT(c == '[' || c == ']');
			numbers.push_back(c == '[' ? OPEN_BRACKET : CLOSE_BRACKET);
		}
	}

	if (sourround) {
		numbers.insert(numbers.begin(), OPEN_BRACKET);
		numbers.insert(numbers.end(), CLOSE_BRACKET);
	}
}

static void reduce(std::vector<int>& numbers) {
	for (;;) {
	start_actions:
		bool found_stuff_to_explode = false;
		int depth = 0;
		for (int i = 0; i < numbers.size(); i++) {
			int c = numbers[i];
			if (c == OPEN_BRACKET) {
				if (depth == 4) { // explode
					explode(numbers, i, i + 3);
					found_stuff_to_explode = true;
					break;
				}
				depth++;
			}
			else if (c == CLOSE_BRACKET) {
				depth--;
			}
		}

		if (found_stuff_to_explode) {
			goto start_actions;
		}

		bool found_stuff_to_split = false;
		for (int i = 0; i < numbers.size(); i++) {
			int c = numbers[i];
			if (is_number(c) && c >= 10) {
				split(numbers, i);
				found_stuff_to_split = true;
				break;
			}
		}

		// we are done nothing to explode nor split
		if (!found_stuff_to_split) {
			break;
		}
	}
}

static int get_magnitude(std::vector<int>& numbers) {
	// calculate magnitude
	int deepest = INT_MIN;
	int depth = 0;
	for (int c : numbers) {
		if (c == OPEN_BRACKET) {
			if (depth > deepest) {
				deepest = depth;
			}
			depth++;
		}
		else if (c == CLOSE_BRACKET) {
			depth--;
		}
	}

	for (int i = deepest; i >= 0; i--) {
		int d = 0;
		for (int j = 0; j < numbers.size(); j++) {
			int c = numbers[j];
			if (c == OPEN_BRACKET) {
				if (d == i) {
					int a = numbers[j + 1];
					ASSERT(a != OPEN_BRACKET && a != CLOSE_BRACKET);

					int b = numbers[j + 2];
					ASSERT(b != OPEN_BRACKET && b != CLOSE_BRACKET);

					int c = a * 3 + b * 2;

					numbers.insert(numbers.begin() + j + 4, c);
					// remove the four elements of a pair [ n n ]
					for (int i = 0; i < 4; i++) {
						numbers.erase(numbers.begin() + j);
					}

					//print_numbers(numbers);
				}
				else {
					d++;
				}
			}
			else if (c == CLOSE_BRACKET) {
				d--;
			}
		}
	}

	ASSERT(numbers.size() == 1);
	return numbers[0];
}

int main(void) {
#if 1
	const char* input[] = {
		"[[6,[[9,4],[5,5]]],[[[0,7],[7,8]],[7,0]]]",
		"[[[[2,1],[8,6]],[2,[4,0]]],[9,[4,[0,6]]]]",
		"[[[[4,2],[7,7]],4],[3,5]]",
		"[8,[3,[[2,3],5]]]",
		"[[[[0,0],[4,7]],[[5,5],[8,5]]],[8,0]]",
		"[[[[5,2],[5,7]],[1,[5,3]]],[[4,[8,4]],2]]",
		"[[5,[[2,8],[9,3]]],[[7,[5,2]],[[9,0],[5,2]]]]",
		"[[9,[[4,3],1]],[[[9,0],[5,8]],[[2,6],1]]]",
		"[[0,6],[6,[[6,4],[7,0]]]]",
		"[[[9,[4,2]],[[6,0],[8,9]]],[[0,4],[3,[6,8]]]]",
		"[[[[3,2],0],[[9,6],[3,1]]],[[[3,6],[7,6]],[2,[6,4]]]]",
		"[5,[[[1,6],[7,8]],[[6,1],[3,0]]]]",
		"[2,[[6,[7,6]],[[8,6],3]]]",
		"[[[[0,9],1],[2,3]],[[[7,9],1],7]]",
		"[[[[1,8],3],[[8,8],[0,8]]],[[2,1],[8,0]]]",
		"[[2,9],[[5,1],[[9,3],[4,0]]]]",
		"[9,[8,4]]",
		"[[[3,3],[[6,2],8]],5]",
		"[[[9,[4,8]],[[1,3],[6,7]]],[9,[[4,4],2]]]",
		"[[[[1,3],6],[[5,6],[1,9]]],[9,[[0,2],9]]]",
		"[7,[[[0,6],[1,2]],4]]",
		"[[[[5,0],[8,7]],[[7,3],0]],[[6,7],[0,1]]]",
		"[[[[5,4],7],[[8,2],1]],[[[7,0],[6,9]],0]]",
		"[[[3,[5,6]],[[9,5],4]],[[[9,4],[8,1]],[5,[7,4]]]]",
		"[[[3,[7,5]],[[8,1],8]],[[[6,3],[9,2]],[[5,7],7]]]",
		"[8,[[2,0],[[2,6],8]]]",
		"[[[[5,8],9],1],[9,6]]",
		"[[[9,9],[8,8]],[[[3,5],[8,0]],[[4,6],[3,2]]]]",
		"[[5,[[5,1],6]],[[5,8],9]]",
		"[[7,[[1,6],6]],[[[8,6],7],[6,6]]]",
		"[[0,[[9,5],0]],[4,[[7,9],[4,9]]]]",
		"[[[[4,3],[3,5]],[[1,9],[7,6]]],[3,[[6,4],[6,0]]]]",
		"[[[2,6],6],[6,3]]",
		"[[[[1,5],[3,7]],0],[3,7]]",
		"[4,[[[5,5],4],[[5,5],[9,3]]]]",
		"[[3,[8,6]],[8,[7,7]]]",
		"[8,[9,5]]",
		"[[[6,3],[2,[3,6]]],[[[6,0],[0,2]],[[8,7],5]]]",
		"[[[8,[1,2]],2],7]",
		"[[[[8,4],[2,7]],[[3,9],7]],[[4,[8,8]],[[7,4],9]]]",
		"[[[8,[2,5]],[3,[1,2]]],[[4,[5,0]],3]]",
		"[[8,[0,3]],[[5,1],[1,1]]]",
		"[[[8,[3,6]],6],[[7,[1,5]],[[4,8],9]]]",
		"[[[5,0],[0,3]],[[2,[7,8]],[1,[4,8]]]]",
		"[9,[4,[9,4]]]",
		"[[[9,[0,4]],2],3]",
		"[[9,[7,[8,9]]],3]",
		"[[[8,6],[[3,5],[9,2]]],[[3,[9,7]],5]]",
		"[[6,[[7,4],2]],[2,[7,[6,0]]]]",
		"[1,[[[2,2],6],8]]",
		"[[[6,[1,8]],[[9,3],[1,8]]],[[[8,2],[9,3]],[[8,2],[9,9]]]]",
		"[[[[2,9],[1,7]],[[4,0],8]],[[8,9],[6,3]]]",
		"[[[[2,4],[6,1]],[[5,4],[2,8]]],[8,[1,[2,4]]]]",
		"[[[4,6],[1,6]],[3,[1,1]]]",
		"[[[[8,3],8],8],[1,[[4,2],3]]]",
		"[[[9,[8,7]],[5,9]],[8,[[5,6],[4,5]]]]",
		"[[[[4,1],2],[[7,8],4]],[0,6]]",
		"[[[9,7],[[8,6],[6,9]]],[[8,[8,4]],[[9,0],2]]]",
		"[[[8,5],[1,9]],[[[2,4],5],6]]",
		"[[[9,[9,3]],[9,[2,3]]],[7,7]]",
		"[[[8,[7,4]],[2,6]],[[[4,5],[9,9]],[0,[5,2]]]]",
		"[7,[2,2]]",
		"[[[[1,8],[5,2]],3],[0,[2,[4,5]]]]",
		"[[5,[[4,8],[5,5]]],[4,[[3,4],[6,0]]]]",
		"[[3,1],[4,[3,[8,2]]]]",
		"[[3,7],[3,[[6,1],[0,2]]]]",
		"[[4,[6,2]],[[3,9],8]]",
		"[[[[2,9],3],[[5,6],4]],[8,2]]",
		"[[4,[[7,9],[4,9]]],[[4,3],[7,[0,7]]]]",
		"[[[3,[8,9]],[[3,4],[9,5]]],3]",
		"[0,[[[3,0],[8,7]],[[0,9],[9,1]]]]",
		"[[[5,[9,9]],2],[4,8]]",
		"[[[[4,4],4],5],[3,4]]",
		"[[[3,[2,2]],7],[[3,2],0]]",
		"[[[[0,5],[5,2]],2],[2,[[1,2],2]]]",
		"[[[4,6],6],[[0,1],6]]",
		"[2,[[[3,9],7],[[9,8],8]]]",
		"[[7,9],[7,[[3,0],9]]]",
		"[[[1,[6,2]],[0,8]],[[[7,2],4],9]]",
		"[[[[4,7],[1,5]],[5,9]],[[2,[0,4]],[7,[7,0]]]]",
		"[[1,[[2,0],[0,4]]],[[[4,6],9],[[6,8],[0,1]]]]",
		"[[[[6,0],7],[7,[9,6]]],[[7,[4,9]],[9,4]]]",
		"[[[5,[4,6]],[[1,9],[5,8]]],[[[3,6],[2,6]],[[7,3],7]]]",
		"[[[6,0],[6,6]],[2,8]]",
		"[[[4,[7,2]],[[5,6],[2,4]]],[[[6,8],5],[4,6]]]",
		"[[[[9,0],9],[4,0]],[[[9,1],8],[6,4]]]",
		"[[6,3],[1,[[5,0],[9,9]]]]",
		"[[[2,7],[5,6]],[[6,[1,4]],[9,9]]]",
		"[[[[0,5],3],[8,7]],[[[9,9],[6,2]],[0,7]]]",
		"[[[5,6],[1,7]],[[[0,4],9],9]]",
		"[[[7,3],3],[6,[0,[8,9]]]]",
		"[[[0,6],[[8,5],[4,6]]],[[[2,7],[4,2]],[[8,7],[0,5]]]]",
		"[[[8,[7,3]],1],8]",
		"[[8,[8,[8,2]]],[[5,4],[1,[2,6]]]]",
		"[[[[1,1],[8,6]],5],9]",
		"[[[[2,4],[5,7]],[[5,8],[3,1]]],7]",
		"[[4,[[0,1],9]],[[3,8],[4,2]]]",
		"[3,2]",
		"[[3,4],[8,[[6,5],[6,6]]]]",
		"[[[[7,0],[3,8]],[[3,3],[2,6]]],[[8,0],9]]",
	};
#else
	const char *input[] = {
		"[[[0,[5,8]],[[1,7],[9,6]]],[[4,[1,2]],[[1,4],2]]]",
		"[[[5,[2,8]],4],[5,[[9,9],0]]]",
		"[6,[[[6,2],[5,6]],[[7,6],[4,7]]]]",
		"[[[6,[0,7]],[0,9]],[4,[9,[9,0]]]]",
		"[[[7,[6,4]],[3,[1,3]]],[[[5,5],1],9]]",
		"[[6,[[7,3],[3,2]]],[[[3,8],[5,7]],4]]",
		"[[[[5,4],[7,7]],8],[[8,3],8]]",
		"[[9,3],[[9,9],[6,[4,9]]]]",
		"[[2,[[7,7],7]],[[5,8],[[9,3],[0,2]]]]",
		"[[[[5,2],5],[8,[3,7]]],[[5,[7,5]],[4,4]]]",
	};
#endif

	std::vector<int> numbers;
	read_into_vec(input[0], numbers);
	for (int i = 1; i < COUNT_OF(input); i++) {
		read_into_vec(input[i], numbers, true);
		reduce(numbers);
	}

	print_numbers(numbers);
	get_magnitude(numbers);
	print_numbers(numbers);

	int max = INT_MIN;
	for (int i = 0; i < COUNT_OF(input); i++) {
		printf("\r%d", i);
		fflush(stdout);
		for (int j = 0; j < COUNT_OF(input); j++) {
			if (i == j) { continue; }
			int additions[][2] = {
				{i, j},
				{j, i},
			};

			for (int k = 0; k < COUNT_OF(additions); k++) {
				int indices[2] = { additions[k][0], additions[k][1] };
				std::vector<int> n;
				read_into_vec(input[indices[0]], n);
				read_into_vec(input[indices[1]], n, true);

				reduce(n);
				int res = get_magnitude(n);
				if (res > max) {
					max = res;
				}
			}
		}
	}

	printf("\nLargest magnitude produced: %d\n", max);

	return 0;
}
