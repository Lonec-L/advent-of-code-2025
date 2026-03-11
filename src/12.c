#include <stdio.h>

int main()
{
	FILE *in_file = fopen("in", "r");

	char buffer[40];
	int x, y;
	int c[6];
	int res1 = 0;
	while (fgets(buffer, 40, in_file)) {
		if (buffer[2] != 'x') {
			continue;
		}
		sscanf(buffer, "%ix%i: %i %i %i %i %i %i\n", &x, &y, &c[0], &c[1], &c[2], &c[3],
		       &c[4], &c[5]);
		int tmp = 0;
		for (int i = 0; i < 6; i++) {
			tmp += c[i];
		}

		if (((x / 3) * (y / 3)) >= tmp) {
			res1 += 1;
		}
	}
	printf("pt1: %i\n", res1);
}
