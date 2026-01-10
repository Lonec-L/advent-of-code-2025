#include <assert.h>
#include <limits.h>
#include <stdint.h>
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

struct light_state_node {
	struct light_state_node *next;
	uint16_t state;
	int steps;
};

struct light_queue {
	struct light_state_node *first;
	struct light_state_node *last;
};

int pop_light_queue(struct light_queue *q, uint16_t *state, int *steps)
{
	if (q->first == NULL) {
		return -1;
	} else {
		*state = q->first->state;
		*steps = q->first->steps;
		struct light_state_node *tmp = q->first;
		q->first = q->first->next;
		free(tmp);
		if (q->first == NULL) {
			q->last = NULL;
		}
	}
	return 0;
}

int push_light_queue(struct light_queue *q, uint16_t state, int steps)
{
	struct light_state_node *node = malloc(sizeof(struct light_state_node));
	node->state = state;
	node->steps = steps;
	node->next = NULL;
	if (q->first == NULL && q->last == NULL) {
		q->first = node;
		q->last = node;
	} else if (q->last->next == NULL) {
		q->last->next = node;
		q->last = node;
	}
	return 0;
}

void visualize(const int equiations[10][15])
{
	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < 14; j++) {
			printf("%i ", equiations[i][j]);
		}
		printf("| %i\n", equiations[i][14]);
	}
}

void gauss_jordan(int equations[10][15])
{
	int curr_row = 0;
	for (int i = 0; i < 15; i++) {
		// find pivot
		int pivot = -1;
		for (int j = curr_row; j < 10; j++) {
			if (equations[j][i] != 0) {
				pivot = j;
				break;
			}
		}
		if (pivot == -1) {
			continue;
		}
		// swap rows
		if (pivot != curr_row) {
			int tmp;
			for (int j = 0; j < 15; j++) {
				tmp = equations[pivot][j];
				equations[pivot][j] = equations[curr_row][j];
				equations[curr_row][j] = tmp;
			}
		}

		// subtract current row from all others that are below
		for (int j = curr_row + 1; j < 10; j++) {
			int factor = (equations[curr_row][i] / equations[j][i]);
			for (int k = 0; k < 15; k++) {
				equations[j][k] -= equations[curr_row][k] * factor;
			}
		}
		curr_row++;
	}

	// jordan part
	for (int i = 1; i < 10; i++) {
		// find the pivot
		int pivot = -1;
		for (int j = 0; j < 14; j++) {
			if (equations[i][j] != 0) {
				if (equations[i][j] != 1) {
					break;
				}
				pivot = j;
				break;
			}
		}
		if (pivot == -1) {
			continue;
		}

		for (int j = 0; j < i; j++) {
			for (int k = 0; k < 15; k++) {
				equations[j][k] -= equations[i][k] *
						   (equations[j][pivot] / equations[i][pivot]);
			}
		}
	}
}

int solve_pt2(int equations[10][15])
{
	// while we have variables to solve, find the column with most ones
	int res = 0;
	while (1) {
		int m_ones = 0;
		int m_index = -1;
		for (int i = 0; i < 14; i++) {
			int tmp = 0;
			for (int j = 0; j < 10; j++) {
				if (equations[j][i] != 0) {
					tmp++;
				}
			}
			if (tmp > m_ones) {
				m_index = i;
				m_ones = tmp;
			}
		}
		if (m_ones == 0) {
			break;
		}
		printf("m_index: %i\nm_ones: %i\n", m_index, m_ones);

		// find factor
		int factor = INT_MAX;
		for (int i = 0; i < 10; i++) {
			if (equations[i][m_index] != 0) {
				int tmp = equations[i][14] / equations[i][m_index];
				factor = (abs(factor) < abs(tmp)) ? factor : tmp;
			}
		}
		printf("factor: %i\n", factor);

		for (int i = 0; i < 10; i++) {
			if (factor != INT_MAX) {
				equations[i][14] -= factor * equations[i][m_index];
			}
			equations[i][m_index] = 0;
		}
		res += factor;
		visualize(equations);
		getchar();
	}
	return res;
}

// pt1 looks like a classic BFS problem. We can present the state as bitmask and then present
// all operations as bitmasks too. the just BFS from initial state, taking each operation
// once in each step and seeing where it takes us. If we reach state 0, we found the
// solution
int main()
{
	FILE *in_file = fopen("in", "r");
	char buffer[500];
	int n = 0, m = 0;
	int max_op = 0;
	uint16_t operations[20];

	int res1 = 0;
	int res2 = 0;
	int max_op_count = 0;
	int max_joltage_req = 0;

	// 10 equiations with max 14 unknowns and a result in right-most column
	int equations[10][15] = {0};
	while (fgets(buffer, sizeof buffer, in_file)) {
		memset(operations, 0, sizeof(operations));
		memset(equations, 0, sizeof(equations));
		uint16_t state = 0;
		char *ptr = buffer;
		char *token;

		while ((token = strsep(&ptr, " \n")) != NULL && token[0] == '[') {
			token++;
			if (token[strlen(token) - 1] == ']') {
				token[strlen(token) - 1] = 0;
			}
			for (int i = 0; i < strlen(token); i++) {
				if (token[i] == '#') {
					state |= (1 << i);
				}
			}
		}

		int op_count = 0;
		while (token != NULL && token[0] == '(') {
			token++;
			if (token[strlen(token) - 1] == ')') {
				token[strlen(token) - 1] = 0;
			}

			char *subptr = token;
			char *subtoken;
			while ((subtoken = strsep(&subptr, ",")) != NULL) {
				operations[op_count] |= (1 << atoi(subtoken));
				if (atoi(subtoken) < 14) {
					equations[atoi(subtoken)][op_count] = 1;
				}
			}
			op_count++;
			token = strsep(&ptr, " \n");
		}
		max_op_count = max(max_op_count, op_count);

		while (token != NULL && token[0] == '{') {
			token++;
			if (token[strlen(token) - 1] == '}') {
				token[strlen(token) - 1] = 0;
			}

			char *subptr = token;
			char *subtoken;
			int jol_req = 0;
			while ((subtoken = strsep(&subptr, ",")) != NULL) {
				equations[jol_req][14] = atoi(subtoken);
				jol_req++;
			}
			max_joltage_req = max(max_joltage_req, jol_req);

			token = strsep(&ptr, " \n");
		}

		// we have a state and operations. Lets do this
		struct light_queue q;
		q.first = NULL;
		q.last = NULL;
		int steps = 0;
		push_light_queue(&q, state, 0);
		int solved = 0;
		while (q.first != NULL) {
			if (pop_light_queue(&q, &state, &steps) != 0) {
				perror("failed to pop!\n");
			}
			for (int i = 0; i < op_count; i++) {
				push_light_queue(&q, (state ^ operations[i]), steps + 1);
				if ((state ^ operations[i]) == 0) {
					solved = 1;
					res1 += steps + 1;
					break;
				}
			}
			if (solved == 1) {
				break;
			}
		}

		while (q.first != NULL) {
			if (pop_light_queue(&q, &state, &steps) != 0) {
				perror("failed to pop!\n");
			}
		}

		// pt2
		printf("pt2\n");
		visualize(equations);
		// TODO: this is not yet correct!
		gauss_jordan(equations);
		printf("\n");
		visualize(equations);
		// when we have variables in reduced low echolen form, we want to maximise the free
		// variables with the most impact, to reduce the number of presses to minimum
		res2 += solve_pt2(equations);
		printf("\n");
		visualize(equations);
		printf("\n");
	}
	printf("pt1: %i\n", res1);
	printf("pt2: %i\n", res2);

	printf("max_jol: %i\nmax_op: %i\n", max_joltage_req, max_op_count);
}
