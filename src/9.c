#include <stdio.h>
#include <stdlib.h>

long long rect_size(int points[1000][2], int a, int b)
{
	return ((long long)abs(points[a][0] - points[b][0]) + 1) *
	       ((long long)abs(points[a][1] - points[b][1]) + 1);
}

int main()
{
	FILE *in_file = fopen("in", "r");
	int points[1000][2];
	int n = 0;
	while (fscanf(in_file, "%i,%i\n", &points[n][0], &points[n][1]) != EOF) {
		n++;
	}

	long long res1 = 0;
	for (int i = 0; i < n; i++) {
		for (int j = i + 1; j < n; j++) {
			long long size = rect_size(points, i, j);
			if (size > res1) {
				res1 = size;
			}
		}
	}
	printf("pt1: %lli\n", res1);
}
