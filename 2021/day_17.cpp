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

int main(int argc, char **args) {

  if (argc < 3) {
    fprintf(stderr, "Not enough arguments\n");
    return -1;
  }

  int vx = atoi(args[1]);
  int vy = atoi(args[2]);

  int sx = 0, sy = 0;
  int px = 0, py = 0;
  int minx = 20;
  int maxx = 30;
  int miny = -10;
  int maxy = -5;
  
  std::vector<int> x_values;
  for (int x = maxx; x >= minx; x--) {
    for (int i = 0; ; i++) {
      int n = seq(i);
      if (n == x) {
        x_values.push_back(i);
      }
      else if (n > x) {
        break;
      }
    }
  }
  
  int max_height = -1000000;

  int steps = 0;
  while (1) {
    steps++;
    printf("\033[0;0H\033[2J");

    
    px += vx;
    vx = MAX(0, vx - 1);

    py += vy;
    vy -= 1;

    if (py > max_height) {
      max_height = py;
    }

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

    printf("Possible x values: [");
    for (int x : x_values) {
      printf("%d,", x);
    }
    printf("]\n");

    if (((px >= minx) && (px <= maxx)) && ((py <= maxy) && (py >= miny))) {
      printf("TARGET REACHED\n");
      break;
    } 
    else  if (px > maxx) {
      break;
    }
    if (py < miny) {
      break;
    }

    usleep(50000);
  }

  printf("Steps: %d, x = %d, y = %d\n", steps, px, py);
  printf("Max height: %d\n", max_height);

  return 0;
}
