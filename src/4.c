#include <assert.h>
#include <stdio.h>
#include <string.h>

int main()
{
	FILE *in_file = fopen("in", "r");

	int res = 0;
	int res2 = 0;
	char data[150][150];
	int count[150][150];
	char buffer[150];
	memset(data, 0, sizeof(data));
	memset(count, 0, sizeof(count));
	int m = 0, n = 0;

	while (fscanf(in_file, "%s\n", buffer) != EOF) {
		if (n == 0) {
			n = strlen(buffer);
		} else {
			assert(n == strlen(buffer));
		}
		memcpy(data[m++], buffer, n);
	}

	// preproccess
	for (int i = 0; i < m; i++) {
		for (int j = 0; j < n; j++) {
			if (data[i][j] == '@') {
				for (int dx = -1; dx <= 1; dx++) {
					for (int dy = -1; dy <= 1; dy++) {
						int x = i + dx;
						int y = j + dy;
						if (x < 0 || y < 0 || x >= m || y >= m ||
						    (dx == 0 && dy == 0)) {
							continue;
						}
						count[x][y]++;
					}
				}
			}
		}
	}

	// pt1
	for (int i = 0; i < m; i++) {
		for (int j = 0; j < n; j++) {
			if (data[i][j] == '@' && count[i][j] < 4) {
				res++;
			}
		}
	}

	// pt2
	int removed = 0;
	do {
		removed = 0;
		for (int i = 0; i < m; i++) {
			for (int j = 0; j < n; j++) {
				if (data[i][j] == '@' && count[i][j] < 4) {
					res2++;
					data[i][j] = '.';
					removed++;
					for (int dx = -1; dx <= 1; dx++) {
						for (int dy = -1; dy <= 1; dy++) {
							int x = i + dx;
							int y = j + dy;
							if (x < 0 || y < 0 || x >= m || y >= m ||
							    (dx == 0 && dy == 0)) {
								continue;
							}
							count[x][y]--;
						}
					}
				}
			}
		}
	} while (removed > 0);

	printf("pt1: %i\n", res);
	printf("pt1: %i\n", res2);
}
