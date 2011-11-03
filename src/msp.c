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
	short int r0, c0, r1, c1;
};

#ifdef _OPENMP
size_t num_threads;
#endif

int a[MAX_N + 1][MAX_N];

struct test_s test[MAX_T];
struct ans_s ans[MAX_T];
struct ans_s ttans[THREAD_NUM];

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
	int i;
	int nr = test[t].nr;
	int nc = test[t].nc;

#ifdef _OPENMP
	for (i = 0; i < THREAD_NUM; ++i)
		ttans[i].sum = -1;
#endif

	#pragma omp parallel if(test[t].nr > 1)
	{
		int j, k;
		size_t tid;
		/*
		int min_i, max_l, max_r;
		long long min, max, sum, cur;
		*/

#ifdef _OPENMP
		//size_t tid = omp_get_thread_num();
		//struct ans_s tans = { -1 };
#else
		//static size_t tid=0;
#endif

		#pragma omp for nowait schedule(dynamic, 20)
		for (i = 1; i <= nr; ++i) {
			for (j = i; j <= nr; ++j) {
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
#ifdef _OPENMP
				/*
				if (max >= tans.sum) {
					tans.sum = max;
					tans.r0 = i - 1;
					tans.c0 = max_l;
					tans.r1 = j - 1;
					tans.c1 = max_r;
				}
				*/
				tid = omp_get_thread_num();

				if (max >= ttans[tid].sum) {
					ttans[tid].sum = max;
					ttans[tid].r0 = i - 1;
					ttans[tid].c0 = max_l;
					ttans[tid].r1 = j - 1;
					ttans[tid].c1 = max_r;
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
		/*
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
		*/
#endif
	}

#ifdef _OPENMP
	nr = 0;

	for (i = 1; i < THREAD_NUM; ++i) {
		if (ttans[i].sum > ttans[nr].sum)
			nr = i;
	}

	ans[t].sum = ttans[nr].sum;
	ans[t].r0 = ttans[nr].r0;
	ans[t].c0 = ttans[nr].c0;
	ans[t].r1 = ttans[nr].r1;
	ans[t].c1 = ttans[nr].c1;
#endif
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
