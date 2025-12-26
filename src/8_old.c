#include <float.h>
#include <limits.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

struct point {
	int used;
	int id;
	int x, y, z;
};

double dist(struct point p1, struct point p2)
{
	return sqrt((double)(p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y) +
		    (p1.z - p2.z) * (p1.z - p2.z));
}

int main()
{
	FILE *in_file = fopen("in", "r");

	struct point p1, p2;
	p1.used = 1;
	static struct point data[1000][1000];
	memset(data, 0, sizeof(data));

	int n = 0;
	while (fscanf(in_file, "%i,%i,%i", &p1.x, &p1.y, &p1.z) != EOF) {
		p1.id = n;
		data[n++][0] = p1;
	}

	static int used[1000][1000];
	memset(used, 0, sizeof(used));

	int T = 1000;
	while (--T) {
		long long lowest = LLONG_MAX;
		int lowest_index[2];
		int lowest_ids[2];
		for (int i = 0; i < n; i++) {
			int j = 0;
			p1 = data[i][j];
			while (p1.used == 1 && j < n - 1) {
				for (int k = 0; k < n; k++) {
					int l = 0;
					p2 = data[k][l];

					while (p2.used == 1 && l < n - 1) {
						if (p1.id != p2.id && dist(p1, p2) < lowest &&
						    used[p1.id][p2.id] == 0) {
							lowest = dist(p1, p2);
							lowest_ids[0] = p1.id;
							lowest_ids[1] = p2.id;
							memcpy(lowest_index, (int[]){i, k},
							       sizeof(lowest_index));
						}
						p2 = data[k][++l];
					}
				}
				p1 = data[i][++j];
			}
		}
		if (lowest < DBL_MAX) {
			used[lowest_ids[0]][lowest_ids[1]] = 1;
			if (lowest_index[0] == lowest_index[1]) {
				continue;
			}
			int i = 0;
			while (data[lowest_index[0]][i].used > 0) {
				i++;
			}
			int j = 0;
			while (data[lowest_index[1]][j].used > 0) {
				data[lowest_index[0]][i++] = data[lowest_index[1]][j];
				data[lowest_index[1]][j].used = 0;
				j++;
			}
		} else {
			printf("Lowest not found...\n");
		}
		printf("%i\n", T);
	}
	int largest[3] = {-1, -1, -1};
	for (int i = 0; i < n; i++) {
		int n = 0;
		while (data[i][n].used > 0) {
			n++;
		}
		printf("N: %i\n", n);
		for (int j = 0; j < 3; j++) {
			if (n > largest[j]) {
				for (int k = 2; k > j; k--) {
					largest[k] = largest[k - 1];
				}
				largest[j] = n;
				break;
			}
		}
	}
	printf("pt1: %i\n", largest[0] * largest[1] * largest[2]);
}
