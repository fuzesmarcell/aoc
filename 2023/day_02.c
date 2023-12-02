#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <stdbool.h>

int main() {
	int n;
	char buffer[128];

	size_t result = 0;
	while (scanf("%s %d:", buffer, &n) == 2) {
		int id = n;
		int r = 0, g = 0, b = 0;
		bool possible = true;
		while (scanf("%d %s", &n, buffer) == 2) {
			char c = buffer[strlen(buffer) - 1];

			if (buffer[0] == 'r') {
				r = n;
			}
			else if (buffer[0] == 'g') {
				g = n;
			}
			else if (buffer[0] == 'b') {
				b = n;
			}

			if (c != ',') {
				if (r > 12 || g > 13 || b > 14)
					possible = false;
				r = 0, g = 0, b = 0;
			}

			if (c != ';' && c != ',')
				break;
		}

		if (possible)
			result += id;
	}

	printf("%zu\n", result);

	return 0;
}