#include <stdio.h>
#include <assert.h>
#include <ctype.h>

#define DIM 2048

static int length;
static int input[DIM];

int main(int argc, char** argv) {
	int c;
	while (c = getchar(), c != EOF) {
		if (!isdigit(c)) continue;
		assert(length < DIM);
		input[length++] = c - '0';
	}

	assert(length % 2 == 0);

	int step = length / 2;
	size_t result = 0;
	for (size_t i = 0; i < length; i++) {
		int a = input[i], b = input[(i + step) % length];
		if (a == b) result += a;
	}

	printf("%zu\n", result);
	return 0;
}