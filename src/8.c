#include <float.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

long long square(long long x)
{
	return x * x;
}

double dist(int points[1000][3], int a, int b)
{
	double res = 0;
	for (int i = 0; i < 3; i++) {
		res += (double)square(points[a][i] - points[b][i]);
	}
	return sqrt(res);
}

int dfs(int edge_matrix[1000][1000], int colors[1000], int x, int c)
{
	if (colors[x] != 0) {
		return 0;
	}
	colors[x] = c;
	int ret = 1; // each needs to count itself
	for (int i = 0; i < 1000; i++) {
		if (edge_matrix[x][i] > 0 && colors[i] == 0) {
			ret += dfs(edge_matrix, colors, i, c);
		}
	}
	return ret;
}

long long insert_smallest(int points[1000][3], int edge_matrix[1000][1000], int n)
{
	double smallest = DBL_MAX;
	int a, b;
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			double d = dist(points, i, j);
			if (d < smallest && i != j && edge_matrix[i][j] == 0) {
				smallest = d;
				a = i;
				b = j;
			}
		}
	}
	if (smallest < DBL_MAX) {
		edge_matrix[a][b] = 1;
		edge_matrix[b][a] = 1;
		return (long long)points[a][0] * (long long)points[b][0];
	} else {
		printf("Smallest not found");
		return 0;
	}
}

int main()
{
	FILE *in_file = fopen("in", "r");

	int points[1000][3];
	int n = 0;
	while (fscanf(in_file, "%i,%i,%i\n", &points[n][0], &points[n][1], &points[n][2]) != EOF) {
		n++;
	}
	printf("N: %i\n", n);

	int edge_matrix[1000][1000];
	memset(edge_matrix, 0, sizeof(edge_matrix));
	int T = 0;
	while (T++ < 1000) {
		insert_smallest(points, edge_matrix, n);
	}

	int colors[1000];
	memset(colors, 0, sizeof(colors));
	int c = 1;
	long long largest[3] = {0, 0, 0};
	for (int i = 0; i < n; i++) {
		if (colors[i] == 0) {
			int size = dfs(edge_matrix, colors, i, c++);
			for (int j = 0; j < 3; j++) {
				if (size > largest[j]) {
					for (int k = 2; k > j; k--) {
						largest[k] = largest[k - 1];
					}
					largest[j] = size;
					break;
				}
			}
		}
	}
	printf("pt1: %lli\n", largest[0] * largest[1] * largest[2]);
	int last = 0;
	while (T++) {
		memset(colors, 0, sizeof(colors));
		int c = 1;
		long long last = insert_smallest(points, edge_matrix, n);
		int size = dfs(edge_matrix, colors, 0, c++);
		if (size == n) {
			printf("pt2: %lli\n", last);
			break;
		}
	}
}
