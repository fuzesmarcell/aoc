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

int main(void) {
  u64 bits[0xff];
  char buffer[2048];

  if (!fgets(buffer, sizeof(buffer), stdin)) {
    fprintf(stderr, "Error\n");
    return -1;
  }

  printf("%s\n", buffer);
  
  char *source = buffer;
  u64 out = 0;
  int len = sizeof(u64) * 2;
  for (int i = 0; i < len; i++) {
    char c = *source++;
    if (c == '\0' || c == '\n') { 
      break;
    }

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

    out |= ((u64)n << ((len - 1) * i));

    printf("%d\n", n);
  }

  printf("%llu\n", (long long unsigned int)out);
    
  return 0;
}
