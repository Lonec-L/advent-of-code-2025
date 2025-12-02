#include <stdio.h>

int main()
{
	FILE *in_file = fopen("in", "r");
	int dial_state = 50;
	int res = 0;
	char c;
	int x;
	// pt1
	while (fscanf(in_file, "%c%i\n", &c, &x) != EOF) {
		if (c == 'L') {
			dial_state -= x;
			dial_state %= 100;
		} else if (c == 'R') {
			dial_state += x;
			dial_state %= 100;
		} else {
			perror("invalid value c");
			return -1;
		}
		if (dial_state == 0) {
			res++;
		}
	}
	printf("pt1: %i\n", res);

	// pt2
	res = 0;
	dial_state = 50;
	fseek(in_file, 0, SEEK_SET);
	while (fscanf(in_file, "%c%i\n", &c, &x) != EOF) {
		if (x > 100) {
			res += x / 100;
			x %= 100;
		}
		if (c == 'L') {
			if (x >= dial_state && dial_state != 0) {
				res++;
			}
			dial_state += 100;
			dial_state -= x;
			dial_state %= 100;
		} else if (c == 'R') {
			if (dial_state + x >= 100) {
				res++;
			}
			dial_state += x;
			dial_state %= 100;
		} else {
			perror("invalid value c");
			return -1;
		}
	}
	printf("pt2: %i\n", res);
}
