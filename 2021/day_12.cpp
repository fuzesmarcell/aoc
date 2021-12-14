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

#include <unordered_set>

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
	node_t* children[10];
};

typedef struct {
	int cap;
	int len;
	node_t* nodes[128];
} search_stack_t;

static int node_count;
static node_t nodes[128];

static node_t* get_or_create(const char* id) {
	for (int i = 0; i < node_count; i++) {
		node_t* node = &nodes[i];
		if (strcmp(node->id, id) == 0) {
			return node;
		}
	}

	assert(node_count + 1 < COUNT_OF(nodes));
	node_t* node = &nodes[node_count++];
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

static bool is_small(const node_t* node) {
	char c = node->id[0];
	return (c >= 'a') && (c <= 'z');
}

typedef struct {
	int count;
	char paths[256][6];
} path_storage_t;

static int all_path_count;
static path_storage_t all_paths[0xfffff];

std::unordered_set<std::string> paths;

static int64_t search_paths(node_t* node, search_stack_t* visited, search_stack_t* path, node_t* special_node) {

#define PUSH(stack, n) assert(stack->len + 1 < stack->cap); stack->nodes[stack->len++] = n;
#define POP(stack) assert(stack->len - 1 >= 0); stack->nodes[stack->len - 1] = NULL; stack->len--;

	int64_t result = 0;

	if (node->type == END) {

		assert(all_path_count + 1 < COUNT_OF(all_paths));
		path_storage_t *storage = &all_paths[all_path_count++];

		char *buffer = (char *)calloc(1024, sizeof(char));

		for (int i = 0; i < path->len; i++) {
			strcat(buffer, path->nodes[i]->id);
		}
		strcat(buffer, "end");

		paths.insert(buffer);

		return 1;
	}

	PUSH(path, node);

	if (node->type == SMALL) {
		node->visit_count++;
		assert(node->visit_count <= 2);
	}

	for (int i = 0; i < node->child_count; i++) {
		node_t* child = node->children[i];
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

	node_t* start = NULL;
	char buffer[128];
	while (fgets(buffer, sizeof(buffer), stdin)) {
		char id_a[100];
		char id_b[100];
		int no_args_read = sscanf(buffer, "%99[^-]-%99[^\n]\n", id_a, id_b);
		assert(no_args_read == 2);

		node_t* a = get_or_create(id_a);
		node_t* b = get_or_create(id_b);

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

	search_stack_t stack = { };
	stack.cap = 128;

	search_stack_t path = { };
	path.cap = 128;

	for (int i = 0; i < COUNT_OF(nodes); i++) {
		node_t* node = &nodes[i];
		if (node->type == SMALL) {
			search_paths(start, &stack, &path, node);
		}
	}

	printf("%lld\n", paths.size());

	return 0;
}