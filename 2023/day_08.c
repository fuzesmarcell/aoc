#include <stdio.h>
#include <stdbool.h>
#include <assert.h>
#include <string.h>
#include <inttypes.h>

typedef struct {
	char c[3];
} NodeID;

typedef struct {
	NodeID id;
	NodeID children[2];
	size_t hit;
} Node;

static bool node_id_equal(const NodeID* a, const NodeID* b);
static bool node_id_equal_str(const NodeID* a, const char* str);
static Node* find_node(const char* str);
static Node* find_node_from_id(const NodeID* id);

#define MAX_NODES 1000
static Node nodes[MAX_NODES];
static int num_nodes;

typedef struct {
	bool offset_computed;
	bool computed;
	int64_t offset;
	int64_t loop;
} NodeStats;

static NodeStats stats[MAX_NODES];
static Node* iter_nodes[MAX_NODES];
static int num_iter_nodes;

static Node* advance_node(char move, const Node* node);
static bool iter_nodes_done();

int main() {
	char directions[512];
	scanf("%s", directions);

	int nreads = 0;
	while (1) {
		Node node = { 0 };
		int nreads = scanf("%s = (%31[^,], %31[^)])", &node.id.c[0], &node.children[0].c[0], &node.children[1].c[0]);
		if (nreads == EOF)
			break;
		nodes[num_nodes++] = node;
		if (nodes[num_nodes-1].id.c[2] == 'A')
			iter_nodes[num_iter_nodes++] = &nodes[num_nodes-1];
	}

	int move_idx = 0;
	int move_len = (int)strlen(directions);
	size_t steps = 0;

	int n = 0;
	do {
		int z_count = 0;
		char move = directions[move_idx];
		bool all_computed = true;
		for (int i = 0; i < num_iter_nodes; i++) {
			iter_nodes[i] = advance_node(move, iter_nodes[i]);

			if (!stats[i].computed) {
				all_computed = false;

				if (!stats[i].offset_computed) {
					++stats[i].offset;
				}

				if (stats[i].offset_computed) {
					++stats[i].loop;
				}

				if (iter_nodes[i]->id.c[2] == 'Z') {
					if (stats[i].offset_computed) {
						stats[i].computed = true;
					}
					else {
						stats[i].offset_computed = true;
					}
				}
			}
		}

		if (all_computed) {

			int64_t max = INT64_MIN;
			for (int i = 0; i < num_iter_nodes; i++)
				if (stats[i].offset > max)
					max = stats[i].offset;

			int64_t step_counter = max;
			for (;;) {

				bool all_divisble = true;
				for (int i = 0; i < num_iter_nodes; i++) {
					if ((step_counter % stats[i].offset) != 0) {
						all_divisble = false;
						break;
					}
				}

				if (all_divisble) {
					int stophere = 5;
					return;
				}

				step_counter += max;
			}

		}

		++steps;
		move_idx = (move_idx + 1) % move_len;
	} while (!iter_nodes_done());

	printf("Steps %zu\n", steps);

	return 0;
}

bool node_id_equal(const NodeID* a, const NodeID* b) {
	return
		a->c[0] == b->c[0] &&
		a->c[1] == b->c[1] &&
		a->c[2] == b->c[2];
}

bool node_id_equal_str(const NodeID* a, const char* str) {
	return a->c[0] == str[0] && a->c[1] == str[1] && a->c[1] == str[1];
}

Node* find_node(const char* str) {
	for (int i = 0; i < num_nodes; i++)
		if (node_id_equal_str(&nodes[i].id, str))
			return &nodes[i];
	return NULL;
}

Node* find_node_from_id(const NodeID* id) {
	for (int i = 0; i < num_nodes; i++)
		if (node_id_equal(id, &nodes[i].id))
			return &nodes[i];
	return NULL;
}

Node* advance_node(char move, const Node* node) {
	assert(move == 'L' || move == 'R');
	if (move == 'L')
		return find_node_from_id(&node->children[0]);
	return find_node_from_id(&node->children[1]);
}

bool iter_nodes_done() {
	for (int i = 0; i < num_iter_nodes; i++)
		if (iter_nodes[i]->id.c[2] != 'Z')
			return false;
	return true;
}
