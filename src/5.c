#include <stdio.h>

int main()
{
	FILE *in_file = fopen("in", "r");
	long long upper, lower;
	long long limits[200][2]; // 200, lower, upper
	int n = 0;
	char buffer[40];
	while (fgets(buffer, 40, in_file)) {
		if (buffer[0] == '\n') {
			break;
		}
		sscanf(buffer, "%lli-%lli", &lower, &upper);
		limits[n][0] = lower;
		limits[n][1] = upper;
		n++;
	}

	long long x;
	int res = 0;
	long long res2 = 0;
	while (fscanf(in_file, "%lli\n", &x) != EOF) {
		for (int i = 0; i < n; i++) {
			if (x >= limits[i][0] && x <= limits[i][1]) {
				res++;
				break;
			}
		}
	}
	printf("pt1: %i\n", res);
	printf("pt2: %lli\n", res2);
}
