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

size_t nr, nc;
int seed, a, b, m;

inline void gen(void)
{
	int i, j;
	int mean, rem;

	long long sum = 0;
	size_t m_size = nr * nc;

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
    mean = (int) (sum / (long long) m_size); /* updated line */
    rem = (int) (sum % (long long) m_size); /* updated line */
    mean += (rem * 2 > (signed) m_size) ? (1) : (0); /* updated line */
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

	gen();
	ans.sum = -1;

	#pragma omp parallel if(nr > 1)
	{
		int j, k;
#ifdef _OPENMP
		struct ans_s tans;

		tans.sum = -1;
#endif

		#pragma omp for nowait schedule(dynamic, 10)
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
				/*
				tid = omp_get_thread_num();

				if (max > ttans[tid].sum) {
					ttans[tid].sum = max;
					ttans[tid].r0 = i - 1;
					ttans[tid].c0 = max_l;
					ttans[tid].r1 = j - 1;
					ttans[tid].c1 = max_r;
				}
				*/
				if (max > tans.sum) {
					tans.sum = max;
					tans.r0 = i - 1;
					tans.c0 = max_l;
					tans.r1 = j - 1;
					tans.c1 = max_r;
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

#ifdef _OPENMP
#pragma omp critical(ans)
		{
			if (tans.sum > ans.sum) {
				ans.sum = tans.sum;
				ans.r0 = tans.r0;
				ans.c0 = tans.c0;
				ans.r1 = tans.r1;
				ans.c1 = tans.c1;
			}
		}
#endif
	}
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
