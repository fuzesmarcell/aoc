#include <stdio.h>
#include <stdbool.h>
#include <assert.h>
#include <string.h>

typedef struct {
	char c[3];
} NodeID;

typedef struct {
	NodeID id;
	NodeID children[2];
	size_t hit;
} Node;

static bool node_id_equal(NodeID* a, NodeID* b);
static bool node_id_equal_str(NodeID* a, const char* str);
static Node* find_node(const char* str);
static Node* find_node_from_id(const NodeID* id);

#define MAX_NODES 1000
static Node nodes[MAX_NODES];
static int num_nodes;

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
	}

	Node* begin = find_node("AAA");
	Node* end = find_node("ZZZ");

	assert(begin && end);

	int move_idx = 0;
	int move_len = (int)strlen(directions);
	Node* current = begin;
	size_t steps = 0;
	while (current != end) {
		char move = directions[move_idx];
		if (move == 'L')
			current = find_node_from_id(&current->children[0]);
		else
			current = find_node_from_id(&current->children[1]);

		++current->hit;

		++steps;
		move_idx = (move_idx + 1) % move_len;
	}

	printf("Steps %zu\n", steps);

	return 0;
}

bool node_id_equal(NodeID* a, NodeID* b) {
	return
		a->c[0] == b->c[0] &&
		a->c[1] == b->c[1] &&
		a->c[2] == b->c[2];
}

bool node_id_equal_str(NodeID* a, const char* str) {
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
