#include <assert.h>
#include <stdio.h>
#include <string.h>

int main()
{
	FILE *in_file = fopen("in", "r");
	char buffer[150];
	long long beams[150];
	memset(beams, 0, sizeof(beams));
	int n = 0;
	int res = 0;
	long long res2 = 0;
	while (fgets(buffer, 150, in_file)) {
		if (n == 0) {
			n = strlen(buffer);
			for (int i = 0; i < n; i++) {
				if (buffer[i] == 'S') {
					beams[i] = 1;
				}
			}
			continue;
		}
		assert(n == strlen(buffer));

		for (int i = 0; i < n; i++) {
			if (buffer[i] == '^' && beams[i] > 0) {
				if (i >= 1) {
					beams[i - 1] += beams[i];
				}
				if (i < n - 1) {
					beams[i + 1] += beams[i];
				}
				res++;
				beams[i] = 0;
			}
		}
	}
	printf("pt1: %i\n", res);

	// pt2
	for (int i = 0; i < n; i++) {
		res2 += beams[i];
	}
	printf("pt2: %lli\n", res2);
}
