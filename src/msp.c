/* Description: solution of the maximum subarray problem for two
 * dimensions. The solution employs Jay Kadane's algorithm which
 * has O(m*n^2) complexity. Parallelization technique - OpenMP.
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

/* Don't know why, but 'long long' works faster then 'int'
 * even on 32-bit platform. Can only suggest alignment issues. */
struct ans_s {
	long long sum;
	int r0, c0, r1, c1;
};

#ifdef _OPENMP
size_t num_threads;
#endif

int a[MAX_N + 1][MAX_N];

struct test_s test[MAX_T];
struct ans_s ans[MAX_T];

inline int rand(int seed, int i)
{
	return (test[i].a * seed + test[i].b) % test[i].m;
}

inline void gen(int t)
{
	int i, j;
	int sum = 0;
	int mean, rem;
	int seed = test[t].seed;
	int m_size = test[t].nr * test[t].nc;

	for (i = 1; i <= test[t].nr; ++i) {
		for (j = 0; j < test[t].nc; ++j) {
			seed = rand(seed, t);
			a[i][j] = seed;
			sum += seed;
		}
	}

	/* This chunk is taken from the official jury code. */
	///////////////////////////////////////////////////////////////////////////
	mean = sum / m_size;
	rem = sum - mean * m_size;
	mean += (rem * 2 > m_size) ? (1) : (0);
	mean -= (rem * 2 < -m_size) ? (1) : (0);
	///////////////////////////////////////////////////////////////////////////

	for (i = 1; i <= test[t].nr; ++i) {
		//#pragma omp parallel for
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
	size_t nr = test[t].nr;
	size_t nc = test[t].nc;

	#pragma omp parallel if(test[t].nr > 1)
	{
		int i, j;

		#pragma omp for nowait
		for (i = 1; i <= nr; ++i) {
			for (j = i; j <= nr; ++j) {
				#pragma omp task shared(nc)
				{
					int k;
					int cur;
					int max = 0;
					int sum = 0;
					int min = 0;
					int max_l = -1;
					int max_r = -1;
					int min_i = -1;

					for (k = 0; k < nc; ++k) {
						sum += a[j][k] - a[i - 1][k];
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
					#pragma omp critical(ans)
					{
						if (ans[t].sum <= max) {
							ans[t].sum = max;
							ans[t].r0 = i - 1;
							ans[t].c0 = max_l;
							ans[t].r1 = j - 1;
							ans[t].c1 = max_r;
						}
					}
				}
			}
		}
	}
}

int main(int argc, char **argv)
{
	int t, tn;
	FILE *f;

	if (argc < 3)
		return 1;

#ifdef _OPENMP
	num_threads = omp_get_num_procs();
#endif

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
