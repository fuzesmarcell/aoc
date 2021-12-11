/*
 * Advent of code common code
 *
 *
 * File      day_09.c
 * Author    Fuzes Marcell
 * Date      2021.12.09
 */

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <aoc.h>

typedef struct {
  bool visited;
  bool low_point;
  int value;
} node_t;

bool is_digit(char c) {
  return (c >= '0') && (c <= '9');
}

bool in_bounds(int w, int h, int x, int y) {
  return (x >= 0 && x < w && y >= 0 && y < h);
}

int flood_fill(node_t *grid, int w, int h, int x, int y, int n /* the value of the previous node */) {
  if (!in_bounds(w, h, x, y)) {
    return 0;
  }
  
  node_t *node = &grid[x + y * w];
  if (node->visited) {
    return 0;
  }
  
  if (node->value == 9) {
    return 0;
  }
  
  if (n > node->value) {
    return 0;
  }
  
  node->visited = true;
  
  int result = 0;
  result += flood_fill(grid, w, h, x + 1, y, node->value);
  result += flood_fill(grid, w, h, x - 1, y, node->value);
  result += flood_fill(grid, w, h, x, y + 1, node->value);
  result += flood_fill(grid, w, h, x, y - 1, node->value);
  
  return result + 1;
}

void print_grid(const node_t *grid, int w, int h) {
  for (int y = 0; y < h; y++) {
    for (int x = 0; x < w; x++) {
      node_t *node = &grid[x + y * w];
      if (node->low_point) {
        printf(KBLU "%d" RESET, node->value);
      } else if (node->visited) {
        printf(KRED "%d" RESET, node->value);
      } else {
        printf("%d", node->value);
      }
    }
    
    printf("\n");
  }
}

int main(void) {
  FILE *file = fopen("input.txt", "r");
  if (file == NULL) {
    fprintf(stderr, "Could not open file!\n");
    return -1;
  }
  
  int32_t w = 0, h = 0;
  char buffer[1024];
  while (fgets(buffer, sizeof(buffer), file)) {
    if (!w) {
      w = (uint32_t)strlen(buffer) - 1;
    }
    
    h++;
  }
  
  node_t *grid = calloc(w * h, sizeof(node_t));
  if (grid == NULL) {
    return -1;
  }
  
  fseek(file, 0, SEEK_SET);
  uint32_t cnt = 0;
  while (fgets(buffer, sizeof(buffer), file)) {
    for (char *c = buffer; *c; c++) {
      if (is_digit(*c)) {
        grid[cnt++].value = (uint8_t)((*c) - '0');
      }
    }
  }
  
  uint64_t result_01 = 0;
  uint64_t max_basins[3] = {0, 0, 0};
  for (int y = 0; y < h; y++) {
    for (int x = 0; x < w; x++) {
      node_t *node = &grid[x + y * w];
      int n = node->value;
      bool is_low_point = true;
      int dirs[][2] = {
        {x, y + 1},
        {x, y - 1},
        {x + 1, y},
        {x - 1, y}};
      for (int dir_idx = 0; dir_idx < COUNT_OF(dirs); dir_idx++) {
        int xx = dirs[dir_idx][0];
        int yy = dirs[dir_idx][1];
        if (xx >= 0 && xx < w && yy >= 0 && yy < h) {
          if (grid[xx + yy * w].value <= n) {
            is_low_point = false;
            break;
          }
        }
      }
      
      if (is_low_point) {
        result_01 += n + 1;
        
        node->low_point = true;
        node->visited = true;
        
        int basin_size = 0;
        basin_size++;
        basin_size += flood_fill(grid, w, h, x + 1, y, n);
        basin_size += flood_fill(grid, w, h, x - 1, y, n);
        basin_size += flood_fill(grid, w, h, x, y + 1, n);
        basin_size += flood_fill(grid, w, h, x, y - 1, n);
        
#if 0                
        print_grid(grid, w, h);
        printf("\n");
#endif
        // sort max
        for (int i = 0; i < COUNT_OF(max_basins) - 1; i++) {
          for (int k = 0; k < COUNT_OF(max_basins) - i - 1; k++) {
            if (max_basins[k] > max_basins[k + 1]) {
              uint64_t tmp = max_basins[k];
              max_basins[k] = max_basins[k + 1];
              max_basins[k + 1] = tmp;
            }
          }
        }
        
        for (int i = 0; i < COUNT_OF(max_basins); i++) {
          if (basin_size > max_basins[i]) {
            max_basins[i] = basin_size;
            break;
          }
        }
      }
    }
  }
  
  // print_grid(grid, w, h);
  
  printf("Part 1: %llu\n", (long long unsigned int)result_01);
  printf("Part 1: %llu\n", (long long unsigned int)(max_basins[0] * max_basins[1] * max_basins[2]));
  
  return 0;
}
