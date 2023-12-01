#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <stdlib.h>

#define DIM 50

static int nx, ny;
static int input[DIM][DIM];

int main(int argc, char** argv) {

	char buffer[1024];
	char* line;
	while (line = fgets(buffer, sizeof(buffer), stdin), line != NULL) {
		nx = 0;
		char* token = strtok(line, "\t");
		while (token != NULL) {
			int n = atoi(token);
			input[ny][nx++] = n;
			token = strtok(NULL, "\t");
		}
		++ny;
	}

	size_t result = 0;
	for (int y = 0; y < ny; y++) {
		for (int i = 0; i < nx; i++) {
			for (int j = 0; j < nx; j++) {
				if (i == j) continue;
				if (input[y][i] % input[y][j] == 0) {
					result += input[y][i] / input[y][j];
					goto next_row;
				}
			}
		}
	next_row:;
	}

	printf("%zu\n", result);

	return 0;
}