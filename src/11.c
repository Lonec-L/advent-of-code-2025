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

long long count_paths(struct edge *adj[20000], long long memo[20000][2][2], int a, int goal,
		      int fft, int dac)
{
	if (memo != NULL && memo[a][fft][dac] != -1) {
		return memo[a][fft][dac];
	}
	if (a == goal) {
		if (fft == 1 && dac == 1) {
			return 1;
		}
		return 0;
	}
	int ndac = dac;
	int nfft = fft;
	if (a == name_to_int("fft")) {
		nfft = 1;
	}
	if (a == name_to_int("dac")) {
		ndac = 1;
	}

	struct edge *curr = adj[a];
	long long res = 0;
	while (curr != NULL) {
		res += count_paths(adj, memo, curr->to, goal, nfft, ndac);
		curr = curr->next;
	}
	if (memo != NULL) {
		assert(memo[a][fft][dac] == -1 || memo[a][fft][dac] == res);
		memo[a][fft][dac] = res;
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
	long long memo[20000][2][2];
	for (int i = 0; i < 20000; i++) {
		for (int j = 0; j < 2; j++) {
			for (int k = 0; k < 2; k++) {
				memo[i][j][k] = -1;
			}
		}
	}
	printf("pt1: %lli\n", count_paths(adj, NULL, start, goal, 1, 1));

	// pt2
	start = name_to_int("svr");
	for (int i = 0; i < 20000; i++) {
		for (int j = 0; j < 2; j++) {
			for (int k = 0; k < 2; k++) {
				memo[i][j][k] = -1;
			}
		}
	}
	printf("pt2: %lli\n", count_paths(adj, memo, start, goal, 0, 0));
}
