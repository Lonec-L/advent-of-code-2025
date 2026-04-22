/*
 * Inspired by:
 * https://www.reddit.com/r/adventofcode/comments/1pk87hl/2025_day_10_part_2_bifurcate_your_way_to_victory/
 */

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

int active_bits(unsigned int n)
{
	int count = 0;
	while (n > 0) {
		n &= (n - 1);
		count++;
	}
	return count;
}

// gets the next combination of buttons that gets the state to 0. Starts searching with combination
// stored in start argument and returns the next combination that gives 0 output. Returns -1 if none
// were found
int get_next_zero_state(uint16_t start, uint16_t state, uint16_t button_memo[], int op_count)
{
	for (uint16_t i = start; i < 1 << (op_count + 1); i++) {
		if ((state ^ button_memo[i]) == 0) {
			return i;
		}
	}
	return -1;
}

int solve_pt1(uint16_t state, uint16_t button_memo[], int op_count)
{
	int smallest = INT_MAX;
	int tmp = get_next_zero_state(0, state, button_memo, op_count);
	while (tmp != -1) {
		if (active_bits(tmp) < smallest) {
			smallest = active_bits(tmp);
		}
		tmp = get_next_zero_state(tmp + 1, state, button_memo, op_count);
	}
	assert(smallest != INT_MAX && "Solution to pt1 not found!");
	return smallest;
}

int solve_pt2(uint16_t button_memo[], uint16_t operations[20], int op_count,
	      uint16_t joltage_target[20], int jcount)
{
	int sum = 0;
	for (int i = 0; i < jcount; i++) {
		sum += joltage_target[i];
	}
	if (sum == 0) {
		return 0;
	}
	// 1. determine state from joltage target
	uint16_t state = 0;
	for (int i = 0; i < jcount; i++) {
		state |= (joltage_target[i] % 2) << i;
	}
	int res = INT_MAX;

	// 2. get all combinations of buttons that get all joltage values to even numbers
	int tmp = get_next_zero_state(0, state, button_memo, op_count);
	while (tmp != -1) {
		// 3. divide joltage targets by 2
		uint16_t joltages_copy[20] = {0};
		memcpy(joltages_copy, joltage_target, sizeof(joltages_copy));
		for (int i = 0; i < jcount; i++) {
			assert(joltages_copy[i] == joltage_target[i]);
		}

		for (int i = 0; i < op_count; i++) {
			// if button is pressed
			if ((tmp >> i & 1) == 1) {
				for (int j = 0; j < jcount; j++) {
					if ((operations[i] >> j & 1) == 1) {
						if (joltages_copy[j] == 0) {
							// invalid state, stop exploring this path
							goto end;
						}
						joltages_copy[j]--;
					}
				}
			}
		}

		for (int i = 0; i < jcount; i++) {
			assert(joltages_copy[i] % 2 == 0 && "Joltage in solve pt2 not even!");
			joltages_copy[i] /= 2;
		}

		// 4. recurse, multiply result by 2
		int ret = 0;
		ret = solve_pt2(button_memo, operations, op_count, joltages_copy, jcount);
		// some leaves are dead ends, haINT_MAXve to check ret is not INT_MAX
		if (ret == INT_MAX) {
			goto end;
		}
		int button_count = active_bits(tmp);
		if (res > 2 * ret + button_count) {
			res = 2 * ret + button_count;
		}

	end:
		tmp = get_next_zero_state(tmp + 1, state, button_memo, op_count);
	};

	// 5. return lowest calculated result
	return res;
}

int precompute(uint16_t button_memo[], int op_count, uint16_t operations[20])
{
	uint16_t prev = 0;
	// precompute single button presses first
	for (uint16_t i = 0; i < op_count; i++) {
		button_memo[1 << i] = operations[i];
	}
	// now we can incrementally build the whole precompute
	for (uint16_t i = 1; i < 1 << (op_count + 1); i++) {
		int n = -1;
		for (int j = 0; j <= 16; j++) {
			if ((1 & i >> j) == 1) {
				n = j;
				break;
			}
		}
		assert(n >= 0 && "No active bit found!");
		button_memo[i] = button_memo[1 << n] ^ button_memo[i ^ (1 << n)];
	}
	return 0;
}

int main()
{
	FILE *in_file = fopen("in", "r");
	char buffer[500];
	int n = 0, m = 0;
	int max_op = 0;
	uint16_t operations[20];
	uint16_t joltage_target[20];

	int res1 = 0;
	long long res2 = 0;
	while (fgets(buffer, sizeof buffer, in_file)) {
		memset(operations, 0, sizeof(operations));
		memset(joltage_target, 0, sizeof(joltage_target));
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

		int jcount = 0;
		while (token != NULL && token[0] == '{') {
			assert(jcount < 20 && "joltage requirements array too long!!!");
			token++;
			if (token[strlen(token) - 1] == '}') {
				token[strlen(token) - 1] = 0;
			}

			char *subptr = token;
			char *subtoken;
			while ((subtoken = strsep(&subptr, ",")) != NULL) {
				joltage_target[jcount++] = atoi(subtoken);
			}

			token = strsep(&ptr, " \n");
		}

		// precompute
		uint16_t button_memo[1 << (op_count + 1)];
		memset(button_memo, 0, sizeof(button_memo));
		precompute(button_memo, op_count, operations);

		// pt1
		res1 += solve_pt1(state, button_memo, op_count);

		// pt2
		res2 += solve_pt2(button_memo, operations, op_count, joltage_target, jcount);
	}
	printf("pt1: %i\n", res1);
	printf("pt2: %lli\n", res2);
}
