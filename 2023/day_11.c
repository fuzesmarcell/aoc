#include <stdio.h>
#include <string.h>

#include <vector>

typedef struct {
	int x, y;
} Pixel;

#define MAX_PIXELS 100000

static Pixel pixels[MAX_PIXELS];
static int npixels;
static int nx, ny;

static char get_value(int x, int y);

int main() {
	char buffer[256];
	char* line;
	while (line = fgets(buffer, sizeof(buffer), stdin), line != NULL) {
		int len = (int)strlen(line)-1;
		nx = 0;
		for (int i = 0; i < len; i++, ++nx) {
			char c = line[i];
			if (c == '#') {
				Pixel* p = &pixels[npixels++];
				p->x = nx;
				p->y = ny;
			}
		}
		++ny;
	}

	std::vector<int> rows_to_expand;
	std::vector<int> cols_to_expand;

	return 0;
}

char get_value(int x, int y) {
	for (int i = 0; i < npixels; i++) {
		Pixel* p = &pixels[i];
		if (p->x == x && p->y == y)
			return '#';
	}
	return '.';
}