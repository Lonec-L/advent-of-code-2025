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

// looks like a classic BFS problem. We can present the state as bitmask and then present all
// operations as bitmasks too. the just BFS from initial state, taking each operation once in each
// step and seeing where it takes us. If we reach state 0, we found the solution
int main()
{
	FILE *in_file = fopen("in", "r");
	char buffer[500];
	int n = 0, m = 0;
	int max_op = 0;
	uint16_t operations[20];

	int res1 = 0;
	while (fgets(buffer, sizeof buffer, in_file)) {
		memset(operations, 0, sizeof(operations));
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
			}
			op_count++;

			token = strsep(&ptr, " \n");
		}

		while (token != NULL && token[0] == '{') {
			token++;
			if (token[strlen(token) - 1] == '}') {
				token[strlen(token) - 1] = 0;
			}

			char *subptr = token;
			char *subtoken;
			while ((subtoken = strsep(&subptr, ",")) != NULL) {
			}

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
	}
	printf("day1: %i\n", res1);
}
