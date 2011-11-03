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


#define SQR(a) ((a.r1 - a.r0 + 1) * (a.c1 - a.c0 + 1))

#define MAX_N	20000
#define MAX_T	1000

#define THREAD_NUM	100


struct ans_s {
	int sum;
	short int r0, c0, r1, c1;
};


int mat[MAX_N + 1][MAX_N];

struct ans_s ans;
struct ans_s ttans[THREAD_NUM];

size_t nr, nc;
int seed, a, b, m;
int sum;

inline void gen(void)
{
	int i, j;
	int mean, rem;
	int m_size = nr * nc;

	sum = 0;

	for (i = 1; i <= nr; ++i) {
		for (j = 0; j < nc; ++j) {
			seed = (seed * a + b) % m;
			mat[i][j] = mat[i - 1][j] + seed;
		}
	}

	for (j = 0; j < nc; ++j)
		sum += mat[nr][j];

	/* This chunk is taken from the official jury code. */
	///////////////////////////////////////////////////////////////////////////
	mean = sum / m_size;
	rem = sum - mean * m_size;
	mean += (rem * 2 > m_size) ? (1) : (0);
	mean -= (rem * 2 < -m_size) ? (1) : (0);
	///////////////////////////////////////////////////////////////////////////

	for (i = 1; i <= nr; ++i) {
		for (j = 0; j < nc; ++j)
			mat[i][j] -= mean * i;
	}

#ifdef DEBUG
	// Print accumulated matrix.
	for (i = 1; i <= nr; ++i) {
		for (j = 0; j < nc; ++j)
			fprintf(stderr, "%3d", mat[i][j]);

		fputc('\n', stderr);
	}
#endif
}

void solve(void)
{
	int i;

#ifdef _OPENMP
	for (i = 0; i < THREAD_NUM; ++i)
		ttans[i].sum = -1;
#else
	ans.sum = -1;
#endif

	gen();

	#pragma omp parallel if(nr > 1)
	{
		int j, k;
		size_t tid;

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
					sum += mat[j][k] - mat[i - 1][k];
					cur = sum - min;

					if (cur > max) {
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
				tid = omp_get_thread_num();

				if (max > ttans[tid].sum) {
					ttans[tid].sum = max;
					ttans[tid].r0 = i - 1;
					ttans[tid].c0 = max_l;
					ttans[tid].r1 = j - 1;
					ttans[tid].c1 = max_r;
				}
#else
				if (max > ans.sum) {
					ans.sum = max;
					ans.r0 = i - 1;
					ans.c0 = max_l;
					ans.r1 = j - 1;
					ans.c1 = max_r;
				}
#endif
			}
		}
	}

#ifdef _OPENMP
	nr = 0;

	for (i = 1; i < THREAD_NUM; ++i) {
		if (ttans[i].sum > ttans[nr].sum)
			nr = i;
	}

	ans.sum = ttans[nr].sum;
	ans.r0 = ttans[nr].r0;
	ans.c0 = ttans[nr].c0;
	ans.r1 = ttans[nr].r1;
	ans.c1 = ttans[nr].c1;
#endif
}

int main(int argc, char **argv)
{
	int t, tn;
	FILE *fin;
	FILE *fout;

	if (argc < 3)
		return 1;

	fin = fopen(argv[1], "r");
	fout = fopen(argv[2], "w");
	fscanf(fin, "%d", &tn);

	for (t = 0; t < tn; ++t) {
		fscanf(fin, "%d%d", &nr, &nc);
		fscanf(fin, "%d%d%d%d", &seed, &a, &b, &m);
		solve();
		fprintf(fout, "Case #%d: %d %d %d %d %d %d\n",
				t + 1, ans.r0, ans.c0, ans.r1, ans.c1, ans.sum, SQR(ans));
	}

	fclose(fin);
	fclose(fout);

	return 0;
}
