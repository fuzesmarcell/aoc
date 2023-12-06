#include <stdio.h>
#include <inttypes.h>

#define countof(arr) (sizeof(arr)/sizeof((arr)[0]))

int main() {

	int64_t times[] = { 47847467, };
	int64_t distances[] = { 207139412091014, };

	size_t result = 1;
	for (int i = 0; i < countof(times); i++) {
		int64_t race_time = times[i];
		int64_t winning_distance = distances[i];
		int64_t wins = 0;
		for (int64_t time = 1; time < race_time; time++) {
			int64_t speed = time;
			int64_t distance = (race_time - time) * speed;
			if (distance > winning_distance)
				++wins;
		}

		result *= wins;
	}

	return 0;
}