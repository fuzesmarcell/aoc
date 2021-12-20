/*
 * Advent of code common code
 *
 *
 * File      day_17.cpp
 * Author    Fuzes Marcell
 * Date      2021.12.17
 */

#include <stdio.h>
#include <stdlib.h>

#include <vector>

#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif

#include "../aoc.h"

static int seq(int n) {
  return (n * (n + 1)) / 2;
}

static int traject_x(int x, int vx, int t) {
  if (t >= vx) {
    return x + seq(vx);
  }
  return x + seq(vx) - seq(vx - t);
}

static int traject_y(int y, int vy, int t) {
  if (t >= vy) {
    int s = MAX(0, t - vy - 1);
    printf("s %d\n", s);
    return y + seq(vy) - seq(s);
  }
  return y + seq(vy) - seq(vy - t);
}

static void draw_grid(int sx, int sy, int px, int py, int minx, int maxx, int miny, int maxy) {
  int w = 150, h = 50;
  for (int y = 0; y < h; y++) {
    for (int x = 0; x < w; x++) {
      int xx = x - (w / 2);
      int yy = (h - y) - (h / 2);
      if (sx == xx && sy == yy) {
        printf(KGRN "S" RESET);
      }
      else if (px == xx && py == yy) {
        printf(KRED "#" RESET);
      }
      else if (((xx >= minx) && (xx <= maxx)) && ((yy <= maxy) && (yy >= miny))) {
        printf("T");
      }
      else {
        printf(".");
      }
    }
    printf("\n");
  }
}

int main(int argc, char **args) {

  // target area: x=25..67, y=-260..-200
  int minx = 20;
  int maxx = 30;
  int miny = -10;
  int maxy = -5;
  
  int sx = 0, sy = 0;
  int px = 0, py = 0;
  int vx = 0, vy = 0;

  for (int i = 0; i < 30; i++) {
    printf("%d: %d\n", i, seq(i));
  }

  for (int y = 0; ;y++) {
    printf("y = %d\n", y);
    py = y;
    vy = 0;
    while (1) {
      py += vy;
      vy += -1;
      if (py < miny) {
        goto end;
      }
      else if (py <= maxy && py >= miny) {
        break;
      }
    }
  }

end:

  return 0;
}
