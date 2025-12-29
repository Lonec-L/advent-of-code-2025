#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

long long rect_size(int points[1000][2], int a, int b)
{
	return ((long long)abs(points[a][0] - points[b][0]) + 1) *
	       ((long long)abs(points[a][1] - points[b][1]) + 1);
}

int max(int a, int b)
{
	if (a > b) {
		return a;
	} else {
		return b;
	}
}

int min(int a, int b)
{
	if (a < b) {
		return a;
	} else {
		return b;
	}
}

int main()
{
	FILE *in_file = fopen("in", "r");
	int points[1000][2];
	int n = 0;
	while (fscanf(in_file, "%i,%i\n", &points[n][0], &points[n][1]) != EOF) {
		n++;
	}

	// pt1
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

	// visualization data
	int img[1000][1000];
	memset(img, 0, sizeof(img));

	int points_to_check[11000][2];
	memset(points_to_check, 0, sizeof(points_to_check));
	int points_to_check_count = 0;

	img[points[0][0] / 100][points[0][1] / 100] = 1;
	for (int i = 1; i < n; i++) {
		img[points[i][0] / 100][points[i][1] / 100] = 1;
		for (int j = 0; j < 10; j++) {
			img[(points[i - 1][0] / 100) +
			    (((points[i][0] - points[i - 1][0]) * j) / 1000)]
			   [(points[i - 1][1] / 100) +
			    (((points[i][1] - points[i - 1][1]) * j) / 1000)] = 2;
			points_to_check[points_to_check_count][0] =
				(points[i - 1][0]) + (((points[i][0] - points[i - 1][0]) * j) / 10);
			points_to_check[points_to_check_count][1] =
				(points[i - 1][1]) + (((points[i][1] - points[i - 1][1]) * j) / 10);
			points_to_check_count++;
		}
	}

	// pt2
	long long res2 = 0;
	int p1, p2;
	for (int i = 0; i < n; i++) {
		for (int j = i + 1; j < n; j++) {
			long long size = rect_size(points, i, j);
			int good = 1;
			for (int k = 0; k < points_to_check_count; k++) {
				// check if inside
				if (points_to_check[k][0] > min(points[i][0], points[j][0]) &&
				    points_to_check[k][0] < max(points[i][0], points[j][0]) &&
				    points_to_check[k][1] > min(points[i][1], points[j][1]) &&
				    points_to_check[k][1] < max(points[i][1], points[j][1])) {
					good = 0;
					break;
				}
			}
			if (size > res2 && good == 1) {
				res2 = size;
				p1 = i;
				p2 = j;
			}
		}
	}

	printf("pt2: %lli\n", res2);

	// data for accepted answer
	for (int i = min(points[p1][0], points[p2][0]) / 100;
	     i < max(points[p1][0], points[p2][0]) / 100; i++) {
		for (int j = min(points[p1][1], points[p2][1]) / 100;
		     j < max(points[p1][1], points[p2][1]) / 100; j++) {
			img[i][j] = 3;
		}
	}

	// write to image file
	FILE *f = fopen("visualizations/day9.ppm", "wb");
	fprintf(f, "P6\n");
	fprintf(f, "%d %d\n", 1000, 1000);
	fprintf(f, "255\n");

	for (int i = 0; i < 1000; i++) {
		for (int j = 0; j < 1000; j++) {
			if (img[i][j] == 0) {
				// nothing, print black
				fputc(0x00, f);
				fputc(0x00, f);
				fputc(0x00, f);
			} else if (img[i][j] == 1) {
				// red
				fputc(0xFF, f);
				fputc(0x00, f);
				fputc(0x00, f);
			} else if (img[i][j] == 2) {
				// green
				fputc(0x00, f);
				fputc(0xFF, f);
				fputc(0x00, f);
			} else if (img[i][j] == 3) {
				// purple - solution
				fputc(0xFF, f);
				fputc(0x00, f);
				fputc(0xFF, f);
			}
		}
	}
	fclose(f);
}
