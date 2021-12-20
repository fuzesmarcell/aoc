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

static void parse(const char **at, Pair *pair, Pair *parent) {
#define NEXT (*(*at)++)
  // We assume the first opening bracket was parsed
  // already
  // [[[[[9,8],1],2],3],4]
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

static Pair *find_pair(Pair *self, int direction, int inc) {
  // we need the previous node so we know the direction we came "from"
  Pair *prev = nullptr;
  for (Pair *node = self->parent; node; node = node->parent) {
    Pair *right = node->pairs[direction];
    if (right == nullptr) {
      node->values[direction] += inc;
      return node;
    }

    if (node->parent == nullptr) { // reached root node
      Pair *root = node;
      if (root->pairs[direction] == prev) {
        return nullptr;
      }

      // if the root value in the direction 
      // was NULL we would have found the value of it
      ASSERT(root->pairs[direction]);
      int opposite = direction == LEFT ? RIGHT : LEFT;
      Pair *most_lr = find_most_lr(root->pairs[direction], opposite);
      most_lr->values[opposite] += inc;
      return most_lr;
    }
    prev = node;
  }

  return nullptr;
}

static void explode(Pair *pair) {
  // Exploding pairs must have both values be literal
  ASSERT(pair->pairs[LEFT] == nullptr && pair->pairs[RIGHT] == nullptr);

  Pair *parent = pair->parent;

  Pair *left = find_pair(pair, LEFT, pair->values[LEFT]); 
  Pair *right = find_pair(pair, RIGHT, pair->values[RIGHT]);
 
  if (left && left == parent) { // merge/explode
    parent->values[RIGHT] = 0;
    free(parent->pairs[RIGHT]);
    parent->pairs[RIGHT] = nullptr;
    ASSERT(parent->pairs[LEFT] == nullptr);
  }
  else if (right && right == parent) { // merge/explode
    parent->values[LEFT] = 0;
    free(parent->pairs[LEFT]);
    parent->pairs[LEFT] = nullptr;
    ASSERT(parent->pairs[RIGHT] == nullptr);
  }
  else {
    ASSERT(false);
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

static bool exec_action(Pair *pair, int depth) {
  if (depth == 4) {
    explode(pair);
    printf("Explode: ");
    print_pairs(g_root, 0);
    return true;
  }

  for (int i = 0; i < 2; i++) {
    Pair *p = pair->pairs[i];
    if (p) {
      if (exec_action(p, depth + 1)) {
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
  
  // This path means we took no actions at all!
  return false;
}

int main(int argc, char **args) {
  const char *text = argc == 2 ? args[1] : "[1,[2,3]]";
  text++;

  Pair root = { };
  g_root = &root;
  parse(&text, &root, nullptr);
  print_pairs(&root, 0);
  while (exec_action(&root, 0)) {}
  print_pairs(&root, 0);
  
  return 0;
}
