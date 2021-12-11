/*
 * Advent of code common code
 *
 *
 * File      day_06.c
 * Author    Fuzes Marcell
 * Date      2021.12.06
 */

#include <assert.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>

static int init_state[] = {1, 1, 1, 1, 3, 1, 4, 1, 4, 1, 1, 2, 5, 2, 5, 1, 1, 1, 4, 3, 1, 4, 1, 1, 1, 1, 1, 1, 1, 2, 1, 2, 4, 1, 1, 1, 1, 1, 1, 1, 3, 1, 1, 5, 1, 1, 2, 1, 5, 1, 1, 1, 1, 1, 1, 1, 1, 4, 3, 1, 1, 1, 2, 1, 1, 5, 2, 1, 1, 1, 1, 4, 5, 1, 1, 2, 4, 1, 1, 1, 5, 1, 1, 1, 1, 5, 1, 3, 1, 1, 4, 2, 1, 5, 1, 2, 1, 1, 1, 1, 1, 3, 3, 1, 5, 1, 1, 1, 1, 3, 1, 1, 1, 4, 1, 1, 1, 4, 1, 4, 3, 1, 1, 1, 4, 1, 2, 1, 1, 1, 2, 1, 1, 1, 1, 5, 1, 1, 3, 5, 1, 1, 5, 2, 1, 1, 1, 1, 1, 4, 4, 1, 1, 2, 1, 1, 1, 4, 1, 1, 1, 1, 5, 3, 1, 1, 1, 5, 1, 1, 1, 4, 1, 4, 1, 1, 1, 5, 1, 1, 3, 2, 2, 1, 1, 1, 4, 1, 3, 1, 1, 1, 2, 1, 3, 1, 1, 1, 1, 4, 1, 1, 1, 1, 2, 1, 4, 1, 1, 1, 1, 1, 4, 1, 1, 2, 4, 2, 1, 2, 3, 1, 3, 1, 1, 2, 1, 1, 1, 3, 1, 1, 3, 1, 1, 4, 1, 3, 1, 1, 2, 1, 1, 1, 4, 1, 1, 3, 1, 1, 5, 1, 1, 3, 1, 1, 1, 1, 5, 1, 1, 1, 1, 1, 2, 3, 4, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 3, 2, 2, 1, 3, 5, 1, 1, 4, 4, 1, 3, 4, 1, 2, 4, 1, 1, 3, 1};

#define MAX_GROWTH 8000000000

// Brute force solution ran each individiual fish
// and cached their results.
// Could have used a left rotate cycle style implementation
// their is no reason to track all the state of the fish!!
static uint64_t cache[10] = {
    [1] = 6206821033,
    [3] = 5217223242,
    [2] = 5617089148,
    [4] = 4726100874,
    [5] = 4368232009,
};

int main(void) {
  int8_t *arr = calloc(MAX_GROWTH, sizeof(int8_t));

  if (arr == NULL) {
    return -1;
  }

  uint64_t count = 0;
  for (int i = 0; i < __crt_countof(init_state); i++) {
    int n = init_state[i];
    printf("%d\n", n);

    if (cache[n] > 0) {
      count += cache[n];
    } else {
      uint64_t cnt = 0;
      arr[cnt++] = n;

      for (int j = 0; j < 256; j++) {
        uint64_t len = cnt;
        for (uint64_t k = 0; k < len; k++) {
          if (--arr[k] < 0) {
            arr[k] = 6;
            assert(cnt + 1 < MAX_GROWTH);
            arr[cnt++] = 8;
          }
        }

        // printf("%d, %llu\n", j, cnt);
      }

      if (cache[n] == 0) {
        cache[n] = cnt;
      }

      count += cnt;
    }
  }

  for (int i = 0; i < _countof(cache); i++) {
    printf("[%d]=%llu\n", i, cache[i]);
  }

  printf("%llu\n", count);

  return 0;
}