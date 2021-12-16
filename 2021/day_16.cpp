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

#include <aoc.h>

#define MAX_SIZE (4096 * 2)

struct BitBuffer {
  i32 buf_len;
  u8 *buffer;

  i32 offset;
};


static int get(BitBuffer *bits, i32 n) {
  assert((bits->offset + n) < bits->buf_len);

  int result = 0;
  for (int i = 0; i < n; i++) {
    int bit = bits->buffer[bits->offset + (n - 1 - i)];
    result |= (bit << i);
  }

  bits->offset += n;

  return result;
}

static void parse_packet(BitBuffer *bits, bool cbb /* count by bits */, i32 cnt) {

  i32 no_packets = 0;
  i32 offset = bits->offset;

  for (; cbb ? ((bits->offset - offset) < cnt) : (no_packets < cnt) ; no_packets++) {
    i32 version = get(bits, 3);
    i32 id = get(bits, 3);
    switch (type_id) {
      case 4 /* literal */: {
        i64 value = 0;

        while (1) {
          i32 first_bit = get(&bits, 1);
          i32 n = get(&bits, 4);
          printf("n: %d\n", n);
          value <<= 4;
          value |= n;
          if (first_bit == 0) { break; }
        }

        printf("value: %lld\n", (long long int)value);
      } break;

      default: /* operator */ {
        i32 len_id = get(&bits, 1);
        if (len_id == 0) {
          int no_bits = get(&bits, 15);
          parse_packet(bits, true, no_bits);
        }
        else (len_type_id == 1) {
          int length = get(&bits, 11);
          parse_packet(bits, false, length);
        }
      } break;
    }
  }
}

int main(void) {

  i32 cnt;
  u8 buffer[MAX_SIZE];

  for (char c; c = getchar(); ) {
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
    printf("%d", buffer[i];
  }
  printf("\n");


  // parse packets
  BitBuffer bits = {};
  bits.buffer = buffer;
  bits.buf_len = cnt;

  parse_packet(&bits, cnt);

  return 0;
}
