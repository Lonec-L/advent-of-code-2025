#include <stdio.h>
#include <string.h>

int is_valid(char s[])
{
	int N = strlen(s);
	if (N % 2 != 0) {
		return 1;
	}
	int is_bad = 1;
	for (int i = 0; i < N / 2; i++) {
		if (s[i] != s[i + N / 2]) {
			is_bad = 0;
			break;
		}
	}
	if (is_bad) {
		return 0;
	}
	return 1;
}

int is_valid_pt2(char s[])
{
	int N = strlen(s);
	// for substring lengths from 1 to N/2
	for (int n = 1; n <= N / 2; n++) {
		if (N % n != 0) {
			continue;
		}
		int is_bad = 1;
		// iterate through first substring of length n
		for (int i = 0; i < n; i++) {
			// check if it repeats through the whole string
			for (int j = n; j < N; j += n) {
				if (s[i] != s[j + i]) {
					is_bad = 0;
					break;
				}
			}
			if (!is_bad) {
				break;
			}
		}
		if (is_bad) {
			return 0;
		}
	}
	return 1;
}

int main()
{
	FILE *in_file = fopen("in", "r");
	long long res = 0;
	long long res2 = 0;
	long long lower, upper;
	// pt1
	char str[20];
	while (fscanf(in_file, "%lli-%lli,", &lower, &upper) != EOF) {
		for (long long i = lower; i <= upper; i++) {
			sprintf(str, "%lli", i);
			if (!is_valid(str)) {
				res += i;
			}
			if (!is_valid_pt2(str)) {
				res2 += i;
			}
		}
	}
	printf("pt1: %lli\n", res);
	printf("pt2: %lli\n", res2);
}
