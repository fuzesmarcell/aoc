/*
 * Advent of code common code
 *
 *
 * File      day_16.cpp
 * Author    Fuzes Marcell
 * Date      2021.12.16
 */

#include <stdio.h>
#include <assert.h>

#include <algorithm>
#include <vector>
#include <numeric>

#include <aoc.h>

#define MAX_SIZE (4096 * 2)

struct BitBuffer {
  i32 buf_len;
  u8 *buffer;

  i32 offset;
};

static int get(BitBuffer *bits, i32 n) {
  assert((bits->offset + n) <= bits->buf_len);

  int result = 0;
  for (int i = 0; i < n; i++) {
    int bit = bits->buffer[bits->offset + (n - 1 - i)];
    result |= (bit << i);
  }

  bits->offset += n;

  return result;
}

static i64 parse_packet(BitBuffer *bits, bool cbb /* count by bits */, i32 cnt, std::vector<i64> &values) {

  i64 result = 0;
  i32 no_packets = 0;
  i32 offset = bits->offset;

  for (; cbb ? ((bits->offset - offset) < cnt) : (no_packets < cnt) ; no_packets++) {
    i32 version = get(bits, 3);
    i32 id = get(bits, 3);
    switch (id) {
      case 4 /* literal */: {
        i64 value = 0;

        while (1) {
          i32 first_bit = get(bits, 1);
          i32 n = get(bits, 4);
          printf("n: %d\n", n);
          value <<= 4;
          value |= n;
          if (first_bit == 0) { break; }
        }

        printf("value: %lld\n", (long long int)value);
        values.push_back(value);
        result += value;

      } break;

      default: /* operator */ {
        i32 len_id = get(bits, 1);
        std::vector<i64> numbers;

        if (len_id == 0) {
            int no_bits = get(bits, 15);
            parse_packet(bits, true, no_bits, numbers);
        }
        else if (len_id == 1) {
            int length = get(bits, 11);
            parse_packet(bits, false, length, numbers);
        }

        i64 value = 0;
        switch (id) {
            case 0: /* sum */ {
                value = std::accumulate(numbers.begin(), numbers.end(), (i64)0);
            } break;

            case 1: /* product */ {
                value = std::accumulate(numbers.begin(), numbers.end(), (i64)1, std::multiplies<i64>());
            } break;

            case 2: /* min */ {
                value = *std::min_element(numbers.begin(), numbers.end());
            } break;

            case 3: /* max */ {
                value = *std::max_element(numbers.begin(), numbers.end());
            } break;

            case 5: /* grt */ {
                assert(numbers.size() == 2);
                value = numbers[0] > numbers[1] ? 1 : 0;
            } break;

            case 6: /* lst */ {
                assert(numbers.size() == 2);
                value = numbers[0] < numbers[1] ? 1 : 0;
            } break;

            case 7: /* eq */ {
                assert(numbers.size() == 2);
                value = numbers[0] == numbers[1] ? 1 : 0;
            } break;
        }

        result += value;
        values.push_back(value);

      } break;
    }
  }

  return result;
}

int main(void) {

  i32 cnt = 0;
  u8 buffer[MAX_SIZE];

  for (char c = getchar(); c > 0; c = getchar()) {
    if (c == '\n') { break; }

    int n = 0;
    if (c >= '0' && c <= '9') {
      n = c - '0';
    }
    else if (c >= 'A' && c <= 'F') {
      n = (c - 'A') + 10;
    }
    else {
      fprintf(stderr, "Unexpected char %d\n", c);
      assert(false);
    }

    for (int i = 0; i < 4; i++) {
      buffer[cnt++] = (n & (1 << (3 - i))) == 0 ? 0 : 1;
    }
  }

  printf("Bits\n");
  for (int i = 0; i < cnt; i++) {
    printf("%d", buffer[i]);
  }
  printf("\n");


  // parse packets
  BitBuffer bits = {};
  bits.buffer = buffer;
  bits.buf_len = cnt;

  std::vector<i64> values;
  i64 result = parse_packet(&bits, false, 1, values);
  printf("Result: %lld\n", (long long int)result);

  return 0;
}
