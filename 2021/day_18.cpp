/*
 * Advent of code
 *
 * File      day_18.cpp
 * Author    Fuzes Marcell
 * Date      2021.12.18
 */
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include <vector>

#include <aoc.h>

#define ASSERT(expr) do { if (!(expr)) { printf("Assertion %s:%d -> %s\n", __FILE__, __LINE__, #expr); __builtin_debugtrap(); } }while(0);

enum {
  LEFT = 0,
  RIGHT = 1,
};

static const char *dir_str[] = {
  "LEFT",
  "RIGHT",
};

struct Pair {
  // NULL indicates root node
  Pair *parent;
  // if not NULL it is set otherwise
  // the corresponding value applies
  Pair *pairs[2];
  int values[2];
};

static void parse(char **at, Pair *pair, Pair *parent) {
#define NEXT (*(*at)++)
  // We assume the first opening bracket was parsed
  // already
  // [[[[[9,8],1],2],3],4]
  ASSERT(**at != '[');
  pair->parent = parent;
  int i = 0;
  for (char c = NEXT; c != ']'; c = NEXT) {
    switch (c) {
      case '[': {
        pair->pairs[i] = (Pair *)calloc(1, sizeof(Pair));
        parse(at, pair->pairs[i++], pair);
      } break;
      case '0':
      case '1':
      case '2':
      case '3':
      case '4':
      case '5':
      case '6':
      case '7':
      case '8':
      case '9': {
        pair->values[i++] = c - '0';
        if (i == 1) { // expect a comma
          NEXT;
        }
      } break;
    }
  }
#undef NEXT
}

static void print_pairs(Pair *pair, int depth) {
  printf("[");
  for (int i = 0; i < 2; i++) {
    Pair *p = pair->pairs[i];
    if (p) {
      print_pairs(p, depth + 1);
    }
    else {
      printf("%d", pair->values[i]);
    }
    printf("%s", i == 0 ? "," : "");
  }

  printf("]");
  if (depth == 0) {
    printf("\n");
  }
}

static Pair *find_most_lr(Pair *start_node, int direction) {
  for (Pair *node = start_node; node; node = node->pairs[direction]) {
    if (node->pairs[direction] == nullptr) {
      return node;
    }
  }

  // unreachable in theory
  // as there must always be a literal value
  // we can find!
  ASSERT(false);
  return nullptr;
}

static void pair_increase(Pair *self, int direction, int inc) {
  // we need the previous node so we know the direction we came "from"
  Pair *prev = nullptr;
  for (Pair *node = self->parent; node; node = node->parent) {
    Pair *right = node->pairs[direction];
    if (right == nullptr) {
      node->values[direction] += inc;
      return;
    }

    if (node->parent == nullptr) { // reached root node
      Pair *root = node;
      if (root->pairs[direction] == prev) {
        return;
      }

      // if the root value in the direction 
      // was NULL we would have found the value of it
      ASSERT(root->pairs[direction]);
      int opposite = direction == LEFT ? RIGHT : LEFT;
      Pair *most_lr = find_most_lr(root->pairs[direction], opposite);
      most_lr->values[opposite] += inc;
      return;
    }
    prev = node;
  }

  return;
}

static void explode(Pair *pair) {
  // Exploding pairs must have both values be literal
  ASSERT(pair->pairs[LEFT] == nullptr && pair->pairs[RIGHT] == nullptr);

  Pair *parent = pair->parent;

  pair_increase(pair, LEFT, pair->values[LEFT]); 
  pair_increase(pair, RIGHT, pair->values[RIGHT]);

  for (int i = 0; i < 2; i++) {
    if (parent->pairs[i] == pair) {
      parent->values[i] = 0;
      free(parent->pairs[i]);
      parent->pairs[i] = nullptr;
      break;
    }
  }
}

static void split(Pair *pair, int direction) {
  ASSERT(pair->pairs[direction] == nullptr);

  float n = (float)pair->values[direction] / 2.0f;
  int a = (int)floorf(n);
  int b = (int)ceilf(n);

  pair->values[direction] = 0;
  pair->pairs[direction] = (Pair *)calloc(1, sizeof(Pair)); 
  pair->pairs[direction]->values[LEFT] = a;
  pair->pairs[direction]->values[RIGHT] = b;
  pair->pairs[direction]->parent = pair;
}

static Pair *g_root;

static bool exec_explode(Pair *pair, int depth) {
  if (depth == 4) {
    explode(pair);
    printf("Explode: ");
    print_pairs(g_root, 0);
    return true;
  }

  for (int i = 0; i < 2; i++) {
    Pair *p = pair->pairs[i];
    if (p) {
      if (exec_explode(p, depth + 1)) {
        return true;
      }
    }
  }

  return false;
}

static bool exec_split(Pair *pair) {
  
  for (int i = 0; i < 2; i++) {
    Pair *p = pair->pairs[i];
    if (p) {
      if (exec_split(p)) {
        return true;
      }
    }
    else {
      if (pair->values[i] >= 10) {
        split(pair, i);
        printf("Split: ");
        print_pairs(g_root, 0);
        return true;
      }
    }
  }
  
  return false;
}

static void exec_action(Pair *root) {
  for (;;) {
  }
  while (exec_explode(root, 0)) {}
  while (exec_split
}

static Pair *add_pairs(Pair *left, Pair *right) {
  Pair *parent = (Pair *)calloc(1, sizeof(Pair));
  parent->pairs[LEFT] = left;
  parent->pairs[RIGHT] = right;
  left->parent = parent;
  right->parent = parent;

  return parent;
}

static bool pair_from_line(Pair **pair) {
  *pair = (Pair *)calloc(1, sizeof(Pair));
  char b[0xff];
  bool result = fgets(b, sizeof(b), stdin);
  char *buf = b;
  buf++;
  parse(&buf, *pair, nullptr);

  return result;
}

int main(int argc, char **args) {

  Pair *left = nullptr;
  bool parsed = pair_from_line(&left);
  ASSERT(parsed);
  Pair *right = nullptr;
  while (pair_from_line(&right)) {
    ASSERT(left && right);
    Pair *root = add_pairs(left, right);
    print_pairs(root, 0);
    while (exec_action(root, 0)) {}
    print_pairs(root, 0);
    left = root;
  }


  // [[[[3,0],[5,3]],[4,4]], [5, 5]]
  
  return 0;
}
