#include <stdio.h>
#include <inttypes.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

#define countof(arr) (sizeof(arr) / sizeof((arr)[0]))

typedef struct {
	int nx, ny;
	int64_t values[128][128];
} History;

#define MAX_HISTORIES 500
static History histories[MAX_HISTORIES];
static int num_histories;

int main() {

	char buffer[1024];
	char* line;

	while (line = fgets(buffer, sizeof(buffer), stdin), line != NULL) {
		History* h = &histories[num_histories++];

		char* tok = strtok(line, " ");

		while (tok != NULL) {
			h->values[h->ny][h->nx++] = atoi(tok);
			tok = strtok(NULL, " ");
		}

		++h->ny;

		int j = h->ny;
		int nx = h->nx;

		for (;;) {
			for (int i = 0; i < nx-1; i++)
				h->values[h->ny][i] = h->values[h->ny-1][i+1] - h->values[h->ny-1][i];
			--nx;
			bool all_z = true;
			for (int i = 0; i < nx; i++) {
				if (h->values[h->ny][i] != 0) {
					all_z = false;
					break;
				}
			}
			++h->ny;
			if (all_z)
				break;
		}
	}

	size_t result = 0;
	for (int i = 0; i < num_histories; i++) {
		int64_t n = 0;

		History* h = &histories[i];
		for (int i = h->ny - 2; i >= 0; i--) {
			int j = (h->nx - h->ny)+((h->ny - 1)-i);
			n += h->values[i][j];
		}

		result += n;
	}

	return 0;
}