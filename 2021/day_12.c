/*
 * Advent of code
 *
 *
 * File      day_12.c
 * Author    Fuzes Marcell
 * Date      2021.12.12
 */

#include <assert.h>
#include <stdio.h>
#include <string.h>

#include <aoc.h>

typedef enum {
	LARGE,
	SMALL,
	START,
	END,
} type_t;

typedef struct node_t node_t;
struct node_t {
	int visit_count;
	
	type_t type;
	char id[6];

	int child_count;
	node_t *children[10];
};

typedef struct {
	int cap;
	int len;
	node_t *nodes[128];
} search_stack_t;

static int node_count;
static node_t nodes[128];

static node_t *get_or_create(const char *id) {
	for (int i = 0; i < node_count; i++) {
		node_t *node = &nodes[i];
		if (strcmp(node->id, id) == 0) {
			return node;
		}
	}

	assert(node_count + 1 < COUNT_OF(nodes));
	node_t *node = &nodes[node_count++];
	if (strcmp(id, "start") == 0) {
		node->type = START;
	}
	else if (strcmp(id, "end") == 0) {
		node->type = END;
	}
	else if (id[0] >= 'a' && id[0] <= 'z') {
		node->type = SMALL;
	}
	else {
		node->type = LARGE;
	}

	strcpy(node->id, id);
	
	return node;
}

static bool is_small(const node_t *node) {
	char c = node->id[0];
	return (c >= 'a') && (c <= 'z');
}

typedef struct {
	int count;
	char paths[256][6];
} path_storage_t;

static int all_path_count;
static path_storage_t all_paths[0xfffff];

static int64_t search_paths(node_t *node, search_stack_t *visited, search_stack_t *path, node_t *special_node) {

#define PUSH(stack, n) assert(stack->len + 1 < stack->cap); stack->nodes[stack->len++] = n;
#define POP(stack) assert(stack->len - 1 >= 0); stack->nodes[stack->len - 1] = NULL; stack->len--;

	int64_t result = 0;

	if (node->type == END) {

		assert(all_path_count + 1 < COUNT_OF(all_paths));
		path_storage_t *storage = &all_paths[all_path_count++];

		
		for (int i = 0; i < path->len; i++) {
			// printf("%s,", path->nodes[i]->id);
			strcpy(storage->paths[storage->count++], path->nodes[i]->id);
		}

		strcpy(storage->paths[storage->count++], "end");

		// printf("end\n");

		return 1;
	}

	PUSH(path, node);

	if (node->type == SMALL) {
		node->visit_count++;
		assert(node->visit_count <= 2);
	}

	for (int i = 0; i < node->child_count; i++) {
		node_t *child = node->children[i];
		if (child->type == START) {
			continue;
		}

		if (special_node == child && child->visit_count > 1) {
			continue;
		}
		else if (special_node != child && child->visit_count > 0) {
			assert(child->type == SMALL);
			continue;
		}

		bool can_visit = true;
		if (node->type == SMALL && child->type == SMALL) {
			can_visit = false;
			for (int k = 0; k < child->child_count; k++) {
				if (child->children[k]->type == LARGE ||
					child->children[k]->type == END) {
					can_visit = true;
					break;
				}
			}

			if (!can_visit && special_node == node) {
				can_visit = special_node->visit_count == 1;
			}
		}

		if (can_visit) {
			result += search_paths(child, visited, path, special_node);
		}
	}

	if (node->type == SMALL) {
		node->visit_count--;
		assert(node->visit_count >= 0);
	}

	POP(path);

#undef PUSH
#undef POP

	return result;
}

int main(void) {

	node_t *start = NULL;
	char buffer[128];
	while (fgets(buffer, sizeof(buffer), stdin)) {
		char id_a[100];
		char id_b[100];
		int no_args_read = sscanf(buffer, "%99[^-]-%99[^\n]\n", id_a, id_b);
		assert(no_args_read == 2);

		node_t *a = get_or_create(id_a);
		node_t *b = get_or_create(id_b);

		if (a->type == START) {
			start = a;
		}
		else if (b->type == START) {
			start = b;
		}

		assert(a->child_count + 1 < COUNT_OF(a->children));
		a->children[a->child_count++] = b;

		assert(b->child_count + 1 < COUNT_OF(b->children));
		b->children[b->child_count++] = a;
	}

	assert(start != NULL);

	search_stack_t stack = {
		.cap = 128,
		.len = 0,
	};

	search_stack_t path = {
		.cap = 128,
		.len = 0,
	};

	for (int i = 0; i < COUNT_OF(nodes); i++) {
		node_t *node = &nodes[i];
		if (node->type == SMALL) {
			search_paths(start, &stack, &path, node);
		}
	}

	for (int i = 0; i < all_path_count; i++) {
		if (i % 500 == 0) {
			printf("\r%.2f (%d / %d)", ((float)i / (float)all_path_count) * 100.f, i, all_path_count);
			fflush(stdout);
		}
		
		path_storage_t *a = &all_paths[i];
		for (int k = 0; k < all_path_count; k++) {
			if (i == k) { continue; }
			path_storage_t *b = &all_paths[k];
			if (a->count != b->count) {
				continue;
			}

			size_t size_to_check = a->count * sizeof(6);
			if (memcmp(a, b, sizeof(path_storage_t)) == 0) {
				b->count = 0;
			}
		}
	}

	int64_t result = 0;
	for (int i = 0; i < all_path_count; i++) {
		if (all_paths[i].count > 0) {
			result++;
		}
	}

	printf("\nResult: %lld", (long long int)result);

	return 0;
}