#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main()
{
	FILE *in_file = fopen("in", "r");
	char c;
	int x;
	int m = 0;
	int n = 0;
	char buffer[5000];
	// pt1
	{
		fgets(buffer, 5000, in_file);
		char *number_str = strtok(buffer, " ");
		buffer[strcspn(buffer, "\n")] = 0;
		do {
			n++;
		} while ((number_str = strtok(NULL, " \n")) != NULL);

		int data[n][20];
		int ret;
		fseek(in_file, 0, SEEK_SET);
		for (int i = 0; i < 20; i++) {
			for (int j = 0; j < n; j++) {
				ret = fscanf(in_file, "%i ", &x);
				if (ret == 0 || ret == EOF) {
					break;
				}
				data[j][i] = x;
			}
			if (ret == 0 || ret == EOF) {
				m = i;
				break;
			}
		}

		char operations[n];
		for (int i = 0; i < n; i++) {
			int ret = fscanf(in_file, "%c ", &operations[i]);
			if (ret == 0 || ret == EOF) {
				break;
			}
		}
		long long res = 0;

		long long tmp = 0;
		for (int i = 0; i < n; i++) {
			if (operations[i] == '*') {
				tmp = 1;
			} else {
				tmp = 0;
			}
			for (int j = 0; j < m; j++) {
				if (operations[i] == '*') {
					tmp *= data[i][j];
				} else {
					tmp += data[i][j];
				}
			}
			res += tmp;
		}
		printf("pt1: %lli\n", res);
	}

	// pt2
	{
		fseek(in_file, 0, SEEK_SET);
		char data[20][5000];
		n = 0;
		m = 0;
		while (fgets(buffer, 5000, in_file)) {
			if (buffer[0] != '\n') {
				strlcpy(data[n++], buffer, 5000);
				if (m == 0) {
					m = strlen(buffer);
				} else {
					assert(m == strlen(buffer));
				}
			}
		}

		char op = ' ';
		long long res2 = 0;
		long long tmp = 0;
		char number_s[20];
		for (int i = 0; i < m; i++) {
			if (data[n - 1][i] != ' ') {
				res2 += tmp;
				if (data[n - 1][i] == '\n') {
					break;
				}
				op = data[n - 1][i];
				if (op == '*') {
					tmp = 1;
				} else if (op == '+') {
					tmp = 0;
				} else {
					perror("INVALID OP!");
					exit(-1);
				}
			}
			int k = 0;
			for (int j = 0; j < n - 1; j++) {
				if (isdigit(data[j][i])) {
					number_s[k++] = data[j][i];
				}
			}
			number_s[k] = 0; // null terminate
			if (k == 0) {
				continue;
			}
			if (op == '*') {
				tmp *= atoll(number_s);
			} else {
				tmp += atoll(number_s);
			}
		}
		printf("pt2: %lli\n", res2);
	}
}
