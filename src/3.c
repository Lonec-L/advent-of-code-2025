#include <stdio.h>
#include <stdlib.h>
#include <string.h>

long long max(long long a, long long b)
{
	if (a > b) {
		return a;
	} else {
		return b;
	}
}

long long solve(char s[], int n)
{
	char a[n + 1];
	int N = strlen(s);
	for (int i = 0; i <= n; i++) {
		a[i] = 0;
	}
	for (int i = 0; i < N; i++) {
		int assigned = 0;
		for (int j = max(0, i + n - N); j < n; j++) {
			if (assigned) {
				a[j] = '0';
			} else if (s[i] > a[j]) {
				a[j] = s[i];
				assigned = 1;
			}
		}
	}
	return atoll(a);
}

int main()
{
	FILE *in_file = fopen("in", "r");
	long long res = 0;
	long long res2 = 0;
	// pt1
	char s[110];
	while (fscanf(in_file, "%s\n", s) != EOF) {
		res += solve(s, 2);
		res2 += solve(s, 12);
	}
	printf("pt1: %lli\n", res);
	printf("pt2: %lli\n", res2);
}
