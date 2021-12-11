/*
 * Advent of code common code
 *
 *
 * File      day_08.c
 * Author    Fuzes Marcell
 * Date      2021.12.08
 */

#include <assert.h>
#include <stdio.h>
#include <string.h>

#include <aoc.h>

typedef struct {
  int pattern_len[10];
  char pattern[10][8];

  int output_len[10];
  char output[4][8];
} entry_t;

bool parse_entry(entry_t *entry) {
  memset(entry, 0, sizeof(entry_t));
  for (int i = 0; i < 10; i++) {
    if (scanf("%s", entry->pattern[i]) != 1) {
      return false;
    }
    entry->pattern_len[i] = (int)strlen(entry->pattern[i]);
  }

  scanf("%*s");

  for (int i = 0; i < 4; i++) {
    if (scanf("%s", entry->output[i]) != 1) {
      return false;
    }
    entry->output_len[i] = (int)strlen(entry->output[i]);
  }

  return true;
}

void part_01(void) {
  int easy_numbers_count[] = {2, 4, 3, 7};
  entry_t entry;
  int easy_digit_count = 0;
  while (parse_entry(&entry)) {
    for (int i = 0; i < COUNT_OF(easy_numbers_count); i++) {
      int n = easy_numbers_count[i];

      for (int k = 0; k < 10; k++) {
        if (entry.pattern_len[k] == n) {
          for (int j = 0; j < 4; j++) {
            if (entry.output_len[j] == n) {
              easy_digit_count++;
              printf("%s ", entry.output[j]);
            }
          }
        }
      }
    }

    printf("\n");
  }

  printf("%d\n", easy_digit_count);
}

typedef struct {
  // The segment 0 is always known so it contains
  // only valid data in index 0
  struct {
    char x;
    char y[2];
    char w[2];
    char z[2];
  } uncoded;
  // The final form of the digits with the correct
  // segments decoded
  char decoded[7];

} digit_decoder_t;

void print_decoder(digit_decoder_t *decoder) {
  printf("--- Decoder state ---\n");
  printf("Uncoded:\n");
  printf("x = %c\n", decoder->uncoded.x);
  printf("y = %c || %c\n", decoder->uncoded.y[0], decoder->uncoded.y[1]);
  printf("z = %c || %c\n", decoder->uncoded.z[0], decoder->uncoded.z[1]);
  printf("w = %c || %c\n", decoder->uncoded.w[0], decoder->uncoded.w[1]);
  printf("Decoded:\n");
  for (int i = 0; i < 7; i++) {
    printf("%d -> %c\n", i, decoder->decoded[i]);
  }
  printf("--- Decoder state end ---\n");
}

bool contains(char *arr, int len, char c) {
  for (int i = 0; i < len; i++) {
    if (arr[i] == c) {
      return true;
    }
  }

  return false;
}

int digit_from_pattern(digit_decoder_t *decoder, char *pattern) {
  uint8_t lit = 0;
  for (char *c = pattern; *c; c++) {
    for (int i = 0; i < 7; i++) {
      if (decoder->decoded[i] == *c) {
        lit |= (1 << i);
        break;
      }
    }
  }

  uint8_t digits[] = {
      [0] = 0b00111111,
      [1] = 0b00000110,
      [2] = 0b01011011,
      [3] = 0b01001111,
      [4] = 0b01100110,
      [5] = 0b01101101,
      [6] = 0b01111101,
      [7] = 0b00000111,
      [8] = 0b01111111,
      [9] = 0b01101111};

  for (int i = 0; i < COUNT_OF(digits); i++) {
    if (lit == digits[i]) {
      return i;
    }
  }

  assert(false);
  return -1;
}

void part_02(void) {
  // contains the length of those digits which
  // we know for certain. The order is important
  // as this way we can deduce segments from it!
  int known_digit_order[] = {2, 4, 3, 7};
  entry_t entry;
  int64_t result = 0;
  while (parse_entry(&entry)) {
    digit_decoder_t decoder = {0};
    //
    // Decode known pairs from fix digits
    //
    for (int len_idx = 0; len_idx < COUNT_OF(known_digit_order); len_idx++) {
      for (int i = 0; i < 10; i++) {
        if (entry.pattern_len[i] == known_digit_order[len_idx]) {
          // Order is given to be correct!
          switch (entry.pattern_len[i]) {
            case 2: {  // Digit 1
              decoder.uncoded.y[0] = entry.pattern[i][0];
              decoder.uncoded.y[1] = entry.pattern[i][1];

            } break;

            case 4: {  // Digit 4
              int cnt = 0;
              for (int c_idx = 0; c_idx < 4; c_idx++) {
                if (entry.pattern[i][c_idx] != decoder.uncoded.y[0] &&
                    entry.pattern[i][c_idx] != decoder.uncoded.y[1]) {
                  decoder.uncoded.z[cnt++] = entry.pattern[i][c_idx];
                }
              }

              assert(cnt == 2);

            } break;

            case 3: {  // Digit 7
              int cnt = 0;
              for (int c_idx = 0; c_idx < 3; c_idx++) {
                if (entry.pattern[i][c_idx] != decoder.uncoded.y[0] &&
                    entry.pattern[i][c_idx] != decoder.uncoded.y[1]) {
                  decoder.uncoded.x = entry.pattern[i][c_idx];
                  cnt++;
                }
              }

              decoder.decoded[0] = decoder.uncoded.x;

              assert(cnt == 1);

            } break;

            case 7: {  // Digit 8

              int cnt = 0;
              for (int c_idx = 0; c_idx < 7; c_idx++) {
                int c = entry.pattern[i][c_idx];
                if (c != decoder.uncoded.x && c != decoder.uncoded.y[0] &&
                    c != decoder.uncoded.y[1] && c != decoder.uncoded.z[0] &&
                    c != decoder.uncoded.z[1]) {
                  decoder.uncoded.w[cnt++] = c;
                }
              }

              assert(cnt == 2);

            } break;
          }
          break;
        }
      }
    }

    //
    // Find Digit 3 with the information from step 3
    //
    for (int i = 0; i < 10; i++) {
      // Digits 3, 5, 2 can have a length of 5
      char *pattern = entry.pattern[i];
      int len = entry.pattern_len[i];
      if (len == 5) {
        if (contains(pattern, len, decoder.uncoded.x) &&
            contains(pattern, len, decoder.uncoded.y[0]) &&
            contains(pattern, len, decoder.uncoded.y[1])) {
          // Find plane z
          if (contains(pattern, len, decoder.uncoded.z[0])) {
            decoder.decoded[6] = decoder.uncoded.z[0];
            decoder.decoded[5] = decoder.uncoded.z[1];
          } else if (contains(pattern, len, decoder.uncoded.z[1])) {
            decoder.decoded[6] = decoder.uncoded.z[1];
            decoder.decoded[5] = decoder.uncoded.z[0];
          } else {
            assert(false);
          }

          // Find plane w
          if (contains(pattern, len, decoder.uncoded.w[0])) {
            decoder.decoded[3] = decoder.uncoded.w[0];
            decoder.decoded[4] = decoder.uncoded.w[1];
          } else if (contains(pattern, len, decoder.uncoded.w[1])) {
            decoder.decoded[3] = decoder.uncoded.w[1];
            decoder.decoded[4] = decoder.uncoded.w[0];
          } else {
            assert(false);
          }

          break;
        }
      }
    }

    //
    // Find Digit 2. This is the final step to decode.
    //
    for (int i = 0; i < 10; i++) {
      char *pattern = entry.pattern[i];
      int len = entry.pattern_len[i];
      if (len == 5) {
        if (contains(pattern, len, decoder.decoded[0]) &&
            contains(pattern, len, decoder.decoded[6]) &&
            contains(pattern, len, decoder.decoded[4]) &&
            contains(pattern, len, decoder.decoded[3])) {
          if (contains(pattern, len, decoder.uncoded.y[0])) {
            decoder.decoded[1] = decoder.uncoded.y[0];
            decoder.decoded[2] = decoder.uncoded.y[1];
          } else if (contains(pattern, len, decoder.uncoded.y[1])) {
            decoder.decoded[1] = decoder.uncoded.y[1];
            decoder.decoded[2] = decoder.uncoded.y[0];
          } else {
            assert(false);
          }
        }
      }
    }

    // print_decoder(&decoder);

    int base = 1000;
    for (int i = 0; i < 4; i++) {
      int digit = digit_from_pattern(&decoder, entry.output[i]);
      result += digit * base;
      base /= 10;

      // printf("%s: %d\n", entry.output[i], digit);
    }

    // printf("\n");
  }

  printf("Result: %llu\n", (long long unsigned int)result);
}

int main(void) {
  // part_01();
  part_02();

  return 0;
}
