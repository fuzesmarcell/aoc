#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <ctype.h>
#include <inttypes.h>

int main() {

	char buffer[512];
	char* line;
	line = fgets(buffer, sizeof(buffer), stdin);

	char* tok = strtok(line, " ");
	tok = strtok(NULL, " ");

	int nseeds = 0;
	int64_t seeds[128] = { 0 };

	while (tok != NULL) {
		seeds[nseeds++] = strtoull(tok, NULL, 10);
		tok = strtok(NULL, " ");
	}

	int map_sizes[7] = { 0 };
	int64_t maps[7][128][3] = { 0 };

	int map_idx = -1;

	while (1) {

		while (1) {
			line = fgets(buffer, sizeof(buffer), stdin);
			if (line == NULL)
				goto end;
			if (isalpha(line[0])) {
				++map_idx;
				break;
			}
			if (!isdigit(line[0]))
				break;

			char* tok = strtok(line, " ");
			maps[map_idx][map_sizes[map_idx]][0] = strtoull(tok, NULL, 10);

			tok = strtok(NULL, " ");
			maps[map_idx][map_sizes[map_idx]][1] = strtoull(tok, NULL, 10);

			tok = strtok(NULL, " ");
			maps[map_idx][map_sizes[map_idx]][2] = strtoull(tok, NULL, 10);

			++map_sizes[map_idx];
		}
	}
end:

	assert(nseeds % 2 == 0);

	int64_t min = INT64_MAX;
	for (int i = 0; i < nseeds/2; i++) {
		int64_t begin = seeds[i*2];
		int64_t range = seeds[i*2+1];

		int64_t rangeIdx;
	#pragma omp parallel for
		for (rangeIdx = 0; rangeIdx < range; rangeIdx++) {
			int64_t value = begin + rangeIdx;
			for (int j = 0; j < 7; j++) {
				int conversion_count = 0;
				int64_t conv_value = -1;
				for (int k = 0; k < map_sizes[j]; k++) {
					int64_t dst = maps[j][k][0];
					int64_t src = maps[j][k][1];
					int64_t len = maps[j][k][2];

					int64_t src_min = src;
					int64_t src_max = src + len;
					if (value >= src_min && value < src_max) {
						conv_value = value + (dst - src);
						++conversion_count;
					}
				}

				if (conv_value != -1)
					value = conv_value;

				assert(conversion_count == 0 ||
					   conversion_count == 1);
			}

			if (value < min)
				min = value;
		}
	}

	printf("Result: %zu\n", min);

	return 0;
}