/* Description: solution of the maximum subarray problem for two
 * dimensions. The solution employs Jay Kadane's algorithm which
 * has O(n) complexity. Parallelization technique - OpenMP.
 *
 * Author: Alexander Solovets <asolovets<at>gmail.com>
 */
#include <stdio.h>

#ifdef _OPENMP
#include <omp.h>
#endif


#define SQR(t) ((ans[t].r1 - ans[t].r0 + 1) * (ans[t].c1 - ans[t].c0 + 1))

#define MAX_N	20000
#define MAX_T	1000

#define THREAD_NUM	100


struct test_s {
	size_t nr, nc;
	int seed;
	int a, b, m;
};

struct ans_s {
	long long sum;
	int r0, c0, r1, c1;
};


int a[MAX_N + 1][MAX_N];
int b[THREAD_NUM][MAX_N];

struct test_s test[MAX_T];
struct ans_s ans[MAX_T];

inline int rand(int seed, int i)
{
	return (test[i].a * seed + test[i].b) % test[i].m;
}

inline void gen(int t)
{
	int i, j;
	int mean;
	int seed = test[t].seed;
	long long sum = 0;

	for (i = 1; i <= test[t].nr; ++i) {
		for (j = 0; j < test[t].nc; ++j) {
			seed = rand(seed, t);
			a[i][j] = seed;
			sum += seed;
		}
	}

	mean = ((long double)sum / (test[t].nr  * test[t].nc)) + 0.5;

	for (i = 1; i <= test[t].nr; ++i) {
		#pragma omp parallel for
		for (j = 0; j < test[t].nc; ++j)
			a[i][j] += a[i - 1][j] - mean;
	}

#ifdef DEBUG
	// Print accumulated matrix.
	for (i = 1; i <= test[t].nr; ++i) {
		for (j = 0; j < test[t].nc; ++j)
			fprintf(stderr, "%3d", a[i][j]);

		fputc('\n', stderr);
	}
#endif
}

void solve(int t)
{
	int i, j, k;
	int min_i, max_l, max_r;
	long long min, max, sum, cur;

	#pragma omp parallel
	{
#ifdef _OPENMP
		size_t tid = omp_get_thread_num();
		struct ans_s tans = { -1 };
#else
		static size_t tid=0;
#endif

		#pragma omp for private(j, k, min_i, max_l, max_r, min, max, sum, cur)
		for (i = 1; i <= test[t].nr; ++i) {
			for (j = i; j <= test[t].nr; ++j) {
				max = 0;
				sum = 0;
				min = 0;
				max_l = -1;
				max_r = -1;
				min_i = -1;

				for (k = 0; k < test[t].nc; ++k)
					b[tid][k] = a[j][k] - a[i - 1][k];

				for (k = 0; k < test[t].nc; ++k) {
					sum += b[tid][k];
					cur = sum - min;

					if (cur >= max) {
						max = cur;
						max_l = min_i + 1;
						max_r = k;
					}

					if (sum < min) {
						min = sum;
						min_i = k;
					}
				}
#ifdef _OPENMP
				if (max >= tans.sum) {
					tans.sum = max;
					tans.r0 = i - 1;
					tans.c0 = max_l;
					tans.r1 = j - 1;
					tans.c1 = max_r;
				}
#else
				if (max >= ans[t].sum) {
					ans[t].sum = max;
					ans[t].r0 = i - 1;
					ans[t].c0 = max_l;
					ans[t].r1 = j - 1;
					ans[t].c1 = max_r;
				}
#endif
			}
		}
#ifdef _OPENMP
		#pragma omp critical(ans)
		{
			if (tans.sum >= ans[t].sum) {
				ans[t].sum = tans.sum;
				ans[t].r0 = tans.r0;
				ans[t].c0 = tans.c0;
				ans[t].r1 = tans.r1;
				ans[t].c1 = tans.c1;
			}
		}
#endif
	}
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

	for (t = 0; t < tn; ++t) {
		gen(t);
		solve(t);
	}

	f = fopen(argv[2], "w");

	for (t = 0; t < tn; ++t)
		fprintf(f, "Case #%d: %d %d %d %d %lld %d\n", t + 1,
				ans[t].r0, ans[t].c0, ans[t].r1, ans[t].c1, ans[t].sum, SQR(t));

	fclose(f);

	return 0;
}
