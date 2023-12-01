#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <limits.h>
#include <stddef.h>

static char* digits[][2] = {
	{"one", "eno"},
	{"two", "owt"},
	{"three", "eerht"},
	{"four", "ruof"},
	{"five", "evif"},
	{"six", "xis"},
	{"seven", "neves"},
	{"eight", "thgie"},
	{"nine", "enin"},
};

static int find(const char* str, int idx, int b);
static inline char* str_rev(char* str);

int main() {
	size_t result = 0;
	char buffer[512];
	char* line;
	while (line = fgets(buffer, sizeof(buffer), stdin), line != NULL) {
		result += find(line, 0, 10);
		str_rev(line);
		result += find(line, 1, 1);
	}

	printf("Result %zu\n", result);

	return 0;
}

int find(const char* str, int idx, int b) {
	size_t len = strlen(str);
	ptrdiff_t min0 = INT_MAX;
	int n[2] = { 0 };

	for (size_t i = 0; i < len; i++) {
		if (isdigit(str[i])) {
			min0 = i;
			n[0] = (str[i] - '0')*b;
			break;
		}
	}

	ptrdiff_t min1 = INT_MAX;
	for (int i = 0; i < 9; i++) {
		char* ptr = strstr(str, digits[i][idx]);
		ptrdiff_t diff = ptr - str;
		if (ptr && diff < min1) {
			min1 = diff;
			n[1] = (i+1)*b;
		}
	}

	return (ptrdiff_t)min0 < min1 ? n[0] : n[1];
}

char* str_rev(char* str) {
	if (str == NULL)
		return NULL;

	size_t len = strlen(str);
	for (size_t i = 0; i < len/2; i++) {
		size_t j = len-i-1;
		char tmp = str[i];
		str[i] = str[j];
		str[j] = tmp;
	}

	return str;
}
