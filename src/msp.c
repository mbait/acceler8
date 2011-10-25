/* Description: solution of the maximum subarray problem for two
 * dimensions. The solution employs Jay Kadane's algorithm which
 * has O(n^3) complexity. Optimzation techniques - OpenMP.
 *
 * Author: Alexander Solovets <asolovets<at>gmail.com>
 */
#include <stdio.h>
#include <string.h>

#define SQR(t) ((ans[t].r1 - ans[t].r0 + 1) * (ans[t].c1 - ans[t].c0 + 1))

#define MAX_N	20000
#define MAX_T	1000


struct test_s {
	size_t nr, nc;
	int seed;
	int a, b, m;
};

struct ans_s {
	long long sum;
	int r0, c0, r1, c1;
};


int a[MAX_N][MAX_N];
int b[MAX_N];

struct test_s test[MAX_T];
struct ans_s ans[MAX_T];


inline int rand(int seed, int i)
{
	return (test[i].a * seed + test[i].b) % test[i].m;
}

inline void fill(int t)
{
	int i, j;
	int mean;
	int seed = test[t].seed;
	long long sum = 0;

	for (i = 0; i < test[t].nr; ++i) {
		for (j = 0; j < test[t].nc; ++j) {
			seed = rand(seed, t);
			a[i][j] = seed;
			sum += seed;
		}
	}

	mean = ((long double)sum / (test[t].nr  * test[t].nc)) + 0.5;

#pragma omp parallel for shared(j)
	for (i = 0; i < test[t].nr; ++i) {
		a[i][0] -= mean;

		for (j = 1; j < test[t].nc; ++j)
			a[i][j] += a[i][j - 1] - mean;
	}
}

void solve(int t)
{
	int i, j, k;
	int min_i, max_l, max_r;
	long long min, max, sum, cur;
	struct ans_s tans;

	fill(t);

#pragma omp parallel for private(b, k, i, min_i, max_l, max_r, min, max, sum, cur)
//#pragma omp parallel
	//{
//#pragma omp parallel for firstprivate(k) lastprivate(k) reduction(-: b)
		for (j = 0; j < test[t].nc; ++j) {
			for (k = j; k < test[t].nc; ++k) {
				for (i = 0; i < test[t].nr; ++i) {
					b[i] = a[i][k];

					if (j > 0)
						b[i] -= a[i][j - 1];
				}

				max = 0;
				sum = 0;
				min = 0;
				max_l = -1;
				max_r = -1;
				min_i = -1;

				for (i = 0; i < test[t].nr; ++i) {
					sum += b[i];
					cur = sum - min;

					if (cur >= max) {
						max = cur;
						max_l = min_i + 1;
						max_r = i;
					}

					if (sum < min) {
						min = sum;
						min_i = i;
					}
				}

#pragma omp critical (ans)
				if (max >= ans[t].sum) {
					ans[t].sum = max;
					ans[t].r0 = max_l;
					ans[t].c0 = j;
					ans[t].r1 = max_r;
					ans[t].c1 = k;
				}
			}
		}
//	}
}

int main(int argc, char **argv)
{
	int t, tn;
	FILE *f;

	if (argc < 3)
		return 1;

	f = fopen(argv[1], "r");
	fscanf(f, "%d", &tn);

	for (t = 0; t < tn; ++t) {
		fscanf(f, "%d%d", &test[t].nr, &test[t].nc);
		fscanf(f, "%d%d", &test[t].seed, &test[t].a);
		fscanf(f, "%d%d", &test[t].b, &test[t].m);
	}

	fclose(f);

	for (t = 0; t < tn; ++t)
		solve(t);

	f = fopen(argv[2], "w");

	for (t = 0; t < tn; ++t)
		fprintf(f, "Case #%d: %d %d %d %d %lld %d\n", t + 1,
				ans[t].r0, ans[t].c0, ans[t].r1, ans[t].c1, ans[t].sum, SQR(t));

	fclose(f);

	return 0;
}
