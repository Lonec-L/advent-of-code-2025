#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int max(int a, int b)
{
	if (a > b) {
		return a;
	} else {
		return b;
	}
}

struct edge {
	struct edge *next;
	int to;
};

int name_to_int(const char *s)
{
	assert(strlen(s) == 3 && "invalid input!");
	return (s[0] - 'a') * 26 * 26 + (s[1] - 'a') * 26 + (s[2] - 'a');
}

int count_paths(struct edge *adj[20000], int a, int goal)
{
	if (a == goal) {
		return 1;
	}
	struct edge *curr = adj[a];
	int res = 0;
	while (curr != NULL) {
		res += count_paths(adj, curr->to, goal);
		curr = curr->next;
	}
	return res;
}

int main()
{
	struct edge *adj[20000] = {0};

	FILE *in_file = fopen("in", "r");

	char buffer[150];

	while (fgets(buffer, sizeof buffer, in_file)) {
		char *ptr = buffer;
		char *token;
		token = strsep(&ptr, " ");
		assert(strlen(token) == 4 && token[3] == ':');
		token[3] = 0;
		int from = name_to_int(token);
		while ((token = strsep(&ptr, " ")) != NULL) {
			if (strlen(token) == 4 && token[3] == '\n') {
				token[3] = 0;
			}
			struct edge *e = malloc(sizeof(struct edge));
			int to = name_to_int(token);
			e->to = to;
			e->next = NULL;
			if (adj[from] != NULL) {
				e->next = adj[from];
			}
			adj[from] = e;
		}
	}

	int start = name_to_int("you");
	int goal = name_to_int("out");
	printf("pt1: %i\n", count_paths(adj, start, goal));
}
