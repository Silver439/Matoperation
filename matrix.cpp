#include"matrix.h"
using namespace std;
int r=0;
double endtime3;
float* matrix_1d(int n)
{
	float* p1;
	p1 = new float[n];
	return p1;
}
float** matrix_2d(int m,int n)
{
	float** p2;
	p2 = new float* [m];
	for (int i = 0; i < m; i++)
	{
		p2[i] = matrix_1d(n);
	}
	return p2;
}
void setmatrix(int m, int n, matrix& g)
{
	g.row = m;
	g.col = n;
	g.data = matrix_2d(m, n);
}
void transport(matrix& g)
{
	matrix tmp;
	setmatrix(g.row, g.col, tmp);
	for (int i = 0; i < g.row; i++)
	{
		for (int j = 0; j < g.col; j++)
		{
			tmp.data[i][j] = g.data[i][j];
		}
	}
	for (int i = 0; i < g.row; i++)
	{
		for (int j = 0; j < g.col; j++)
		{
			g.data[i][j] = tmp.data[j][i];
		}
	}
}
void input(matrix &u)
{
	cout << "Please input the lenth of row and column:";
	cin >> u.row >> u.col;
	cout << "Please input the elements of matrix:\n";
	u.data = matrix_2d(u.row, u.col);
	for (int i = 0; i < u.row; i++)
	{
		for (int j = 0; j < u.col; j++)
		{
			cin >> u.data[i][j];
		}
	}
	cout << "input sucessfully!";
}
void testdata1(matrix& p)
{
	default_random_engine e;
	uniform_real_distribution<float> u(-100,100);
	for (int i = 0; i < 1000; i++)
	{
		for (int j = 0; j < 1000; j++)
		{
			p.data[i][j] = u(e);
		}
	}
}
void testdata2(matrix& p)
{
	default_random_engine e(time(0));
	uniform_real_distribution<float> u(-100, 100);
	for (int i = 0; i < 1000; i++)
	{
		for (int j = 0; j < 1000; j++)
		{
			p.data[i][j] = u(e);
		}
	}
}
void product(matrix& m, matrix& n,matrix &g)
{
	for (int i = 0; i < g.row; i++)
	{
		for (int j = 0; j < g.col; j++)
		{
			g.data[i][j] = 0;
			for (int l = 0; l < m.col; l++)
			{
				g.data[i][j] += m.data[i][l] * n.data[l][j];
			}
		}
	}
}
void product2(matrix& m, matrix& n, matrix& g)
{
	register float v0, v1, v2, v3;
	register int mrow, mcol, ncol,i,j,k;
	mrow = m.row;
	mcol = m.col;
	ncol = n.col;
	for (i = 0; i < mrow; i++)
	{
		for (k = 0; k < mcol; k+=4)
		{
			v0 = m.data[i][k];
			v1 = m.data[i][k + 1];
			v2 = m.data[i][k + 2];
			v3 = m.data[i][k + 3];
			for (j = 0; j < ncol; j+=4) {
				g.data[i][j] += v0 * n.data[k][j] + v1 * n.data[k + 1][j] + v2 * n.data[k + 2][j] + v3 * n.data[k + 3][j];
				g.data[i][j+1] += v0 * n.data[k][j+1] + v1 * n.data[k + 1][j+1] + v2 * n.data[k + 2][j+1] + v3 * n.data[k + 3][j+1];
				g.data[i][j+2] += v0 * n.data[k][j+2] + v1 * n.data[k + 1][j+2] + v2 * n.data[k + 2][j+2] + v3 * n.data[k + 3][j+2];
				g.data[i][j+3] += v0 * n.data[k][j+3] + v1 * n.data[k + 1][j+3] + v2 * n.data[k + 2][j+3] + v3 * n.data[k + 3][j+3];
			}
		}
	}
}
void block1(matrix& m, matrix& n, matrix& g)
{
	for (int i = 0; i < g.row; i++)
	{
		for(int j = 0; j < g.col; j++)
		{
			g.data[i][j] = 0;
			for (int l = 0; l < m.col; l += 8)
			{
				g.data[i][j] +=
					m.data[i][l] * n.data[l][j] +
					m.data[i][l + 1] * n.data[l + 1][j] +
					m.data[i][l + 2] * n.data[l + 2][j] +
					m.data[i][l + 3] * n.data[l + 3][j] +
					m.data[i][l + 4] * n.data[l + 4][j] +
					m.data[i][l + 5] * n.data[l + 5][j] +
					m.data[i][l + 6] * n.data[l + 6][j] +
					m.data[i][l + 7] * n.data[l + 7][j];
			}
		}
	}
}
void block2(matrix& m, matrix& n, matrix& g)
{
	for (int i = 0; i < g.row - 4; i += 4)
	{
		for (int j = 0; j < g.col - 4; j += 4)
		{
			g.data[i][j] = 0;
			g.data[i + 1][j] = 0;
			g.data[i + 2][j] = 0;
			g.data[i + 3][j] = 0;
			g.data[i][j + 1] = 0;
			g.data[i + 1][j + 1] = 0;
			g.data[i + 2][j + 1] = 0;
			g.data[i + 3][j + 1] = 0;
			g.data[i][j + 2] = 0;
			g.data[i + 1][j + 2] = 0;
			g.data[i + 2][j + 2] = 0;
			g.data[i + 3][j + 2] = 0;
			g.data[i][j + 3] = 0;
			g.data[i + 1][j + 3] = 0;
			g.data[i + 2][j + 3] = 0;
			g.data[i + 3][j + 3] = 0;
			for (int l = 0; l < m.col; l++)
			{
				g.data[i][j] += m.data[i][l] * n.data[l][j];
				g.data[i + 1][j] += m.data[i + 1][l] * n.data[l][j];
				g.data[i + 2][j] += m.data[i + 2][l] * n.data[l][j];
				g.data[i + 3][j] += m.data[i + 3][l] * n.data[l][j];
				g.data[i][j + 1] += m.data[i][l] * n.data[l][j + 1];
				g.data[i + 1][j + 1] += m.data[i + 1][l] * n.data[l][j + 1];
				g.data[i + 2][j + 1] += m.data[i + 2][l] * n.data[l][j + 1];
				g.data[i + 3][j + 1] += m.data[i + 3][l] * n.data[l][j + 1];
				g.data[i][j + 2] += m.data[i][l] * n.data[l][j + 2];
				g.data[i + 1][j + 2] += m.data[i + 1][l] * n.data[l][j + 2];
				g.data[i + 2][j + 2] += m.data[i + 2][l] * n.data[l][j + 2];
				g.data[i + 3][j + 2] += m.data[i + 3][l] * n.data[l][j + 2];
				g.data[i][j + 3] += m.data[i][l] * n.data[l][j + 3];
				g.data[i + 1][j + 3] += m.data[i + 1][l] * n.data[l][j + 3];
				g.data[i + 2][j + 3] += m.data[i + 2][l] * n.data[l][j + 3];
				g.data[i + 3][j + 3] += m.data[i + 3][l] * n.data[l][j + 3];
			}
		}
	}
}
void senior_block(matrix& m, float** n, matrix& g, int row, int col)
{
	register float t0(0), t1(0), t2(0), t3(0), t4(0), t5(0), t6(0), t7(0),
		t8(0), t9(0), t10(0), t11(0), t12(0), t13(0), t14(0), t15(0);
	float *a0(m.data[row]), * a1(m.data[row+1]), * a2(m.data[row+2]), * a3(m.data[row+3]),
		* b0(n[0]), * b1(n[1]), * b2(n[2]), * b3(n[3]), * end = a0 + m.col;
	do {
		t0 += *(a0) * *(b0);
		t1 += *(a1) * *(b0);
		t2 += *(a2) * *(b0);
		t3 += *(a3) * *(b0++);
		t4 += *(a0) * *(b1);
		t5 += *(a1) * *(b1);
		t6 += *(a2) * *(b1);
		t7 += *(a3) * *(b1++);
		t8 += *(a0) * *(b2);
		t9 += *(a1) * *(b2);
		t10 += *(a2) * *(b2);
		t11 += *(a3) * *(b2++);
		t12 += *(a0++) * *(b3);
		t13 += *(a1++) * *(b3);
		t14 += *(a2++) * *(b3);
		t15 += *(a3++) * *(b3++);
	} while (a0 != end);
	g.data[row][col] = t0;
	g.data[row + 1][col] = t1;
	g.data[row + 2][col] = t2;
	g.data[row + 3][col] = t3;
	g.data[row][col + 1] = t4;
	g.data[row+1][col + 1] = t5;
	g.data[row+2][col + 1] = t6;
	g.data[row+3][col + 1] = t7;
	g.data[row][col + 2] = t8;
	g.data[row+1][col + 2] = t9;
	g.data[row+2][col + 2] = t10;
	g.data[row+3][col + 2] = t11;
	g.data[row][col + 3] = t12;
	g.data[row+1][col + 3] = t13;
	g.data[row+2][col + 3] = t14;
	g.data[row+3][col + 3] = t15;
}
void senior_block2(matrix& m, matrix& n, matrix& g, int i, int j)
{
	register float t0(0), t1(0), t2(0), t3(0), t4(0), t5(0), t6(0), t7(0),
		t8(0), t9(0), t10(0), t11(0), t12(0), t13(0), t14(0), t15(0);
	float* a0(m.data[i]), * a1(m.data[i + 1]), * a2(m.data[i + 2]), * a3(m.data[i + 3]),
		* b0(n.data[j]), * b1(n.data[j + 1]), * b2(n.data[j + 2]), * b3(n.data[j + 3]), * end = a0 + m.col;
	do {
		t0 += *(a0) * *(b0);
		t1 += *(a1) * *(b0);
		t2 += *(a2) * *(b0);
		t3 += *(a3) * *(b0++);
		t4 += *(a0) * *(b1);
		t5 += *(a1) * *(b1);
		t6 += *(a2) * *(b1);
		t7 += *(a3) * *(b1++);
		t8 += *(a0) * *(b2);
		t9 += *(a1) * *(b2);
		t10 += *(a2) * *(b2);
		t11 += *(a3) * *(b2++);
		t12 += *(a0++) * *(b3);
		t13 += *(a1++) * *(b3);
		t14 += *(a2++) * *(b3);
		t15 += *(a3++) * *(b3++);
	} while (a0 != end);
	g.data[i][j] = t0;
	g.data[i + 1][j] = t1;
	g.data[i + 2][j] = t2;
	g.data[i + 3][j] = t3;
	g.data[i][j + 1] = t4;
	g.data[i + 1][j + 1] = t5;
	g.data[i + 2][j + 1] = t6;
	g.data[i + 3][j + 1] = t7;
	g.data[i][j + 2] = t8;
	g.data[i + 1][j + 2] = t9;
	g.data[i + 2][j + 2] = t10;
	g.data[i + 3][j + 2] = t11;
	g.data[i][j + 3] = t12;
	g.data[i + 1][j + 3] = t13;
	g.data[i + 2][j + 3] = t14;
	g.data[i + 3][j + 3] = t15;
}
void sse_senior_block(matrix& m, matrix& n, matrix& g, int i, int j)
{
			float* a0(m.data[i]), * a1(m.data[i + 1]), * a2(m.data[i + 2]), * a3(m.data[i + 3]),
				* b0(n.data[j]), * b1(n.data[j+1]), * b2(n.data[j+2]), * b3(n.data[j+3]), * end = a0 + m.col;
			__m128 m0, m1, m2, m3, n0, n1, n2, n3;
			__m128 t0, t1, t2, t3, t4, t5, t6, t7, t8, t9, t10, t11, t12, t13, t14, t15;
			t0 = t1 = t2 = t3 = t4 = t5 = t6 = t7 = t8 = t9 = t10 = t11 = t12 = t13 = t14 = t15 = _mm_set1_ps(0);
			do {
				m0 = _mm_load_ps(a0); m1 = _mm_load_ps(a1); m2 = _mm_load_ps(a2); m3 = _mm_load_ps(a3);
				n0 = _mm_set1_ps(*(b0)++); n1 = _mm_set1_ps(*(b1)++); n2 = _mm_set1_ps(*(b2)++); n3 = _mm_set1_ps(*(b3)++);
				t0 = _mm_add_ps(t0, _mm_mul_ps(m0, n0));
				t1 =_mm_add_ps(t1, _mm_mul_ps(m1, n0));
				t2 = _mm_add_ps(t2, _mm_mul_ps(m2, n0));
				t3 = _mm_add_ps(t3, _mm_mul_ps(m3, n0));
				t4 = _mm_add_ps(t4, _mm_mul_ps(m0, n1));
				t5 = _mm_add_ps(t5, _mm_mul_ps(m1, n1));
				t6 = _mm_add_ps(t6, _mm_mul_ps(m2, n1));
				t7 = _mm_add_ps(t7, _mm_mul_ps(m3, n1));
				t8 = _mm_add_ps(t8, _mm_mul_ps(m0, n2));
				t9 = _mm_add_ps(t9, _mm_mul_ps(m1, n2));
				t10 = _mm_add_ps(t10, _mm_mul_ps(m2, n2));
				t11= _mm_add_ps(t11, _mm_mul_ps(m3, n2));
				t12 = _mm_add_ps(t12, _mm_mul_ps(m0, n3));
				t13 = _mm_add_ps(t13, _mm_mul_ps(m1, n3));
				t14 = _mm_add_ps(t14, _mm_mul_ps(m2, n3));
				t15 = _mm_add_ps(t15, _mm_mul_ps(m3, n3));
				a0 += 1;
				a1 += 1;
				a2 += 1;
				a3 += 1;
			} while (a0 != end);
			_mm_store_ps(&g.data[i][j], t0);
			_mm_store_ps(&g.data[i+1][j], t1);
			_mm_store_ps(&g.data[i+2][j], t2);
			_mm_store_ps(&g.data[i+3][j], t3);
			_mm_store_ps(&g.data[i][j+1], t4);
			_mm_store_ps(&g.data[i+1][j+1], t5);
			_mm_store_ps(&g.data[i+2][j+1], t6);
			_mm_store_ps(&g.data[i+3][j+1], t7);
			_mm_store_ps(&g.data[i][j+2], t8);
			_mm_store_ps(&g.data[i+1][j+2], t9);
			_mm_store_ps(&g.data[i+2][j+2], t10);
			_mm_store_ps(&g.data[i+3][j+2], t11);
			_mm_store_ps(&g.data[i][j+3], t12);
			_mm_store_ps(&g.data[i+1][j+3], t13);
			_mm_store_ps(&g.data[i+2][j+3], t14);
			_mm_store_ps(&g.data[i+3][j+3], t15);
}
matrix result(matrix& A, matrix& B)
{
	int A_row = A.row;
	int B_col = B.col;
	int B_row = B.row;
	matrix tp;
	tp.data = matrix_2d(A_row, B_col);
	float** tr = matrix_2d(4, B_row);
	int i = 0;
	int j = 0;
	do {
		j = 0;
		do {
			tr[0][j] = B.data[j][i];
			tr[1][j] = B.data[j][i+1];
			tr[2][j] = B.data[j][i+2];
			tr[3][j] = B.data[j][i+3];
		} while ((++j) < B.row);
		j = 0;
		do {
			senior_block(A, tr, tp, j, i);
			j += 4;
		} while (j < B_row);
		i += 4;
	} while (i< B_col);
	return tp;
}

void test()
{
	clock_t start, End;
	matrix m, n, g;
	setmatrix(10000, 10000, m);
	setmatrix(10000, 10000, n);
	setmatrix(10000, 10000, g);
	testdata1(m);
	testdata2(n);
	/*start = clock();
	product(m, n, g);
	End = clock();
	double endtime1 = (double)(End - start) / CLOCKS_PER_SEC;
	cout << "normal: " << g.data[900][900] << endl;
	cout << "total time:" << endtime1 * 1000 << "ms" << endl;
	for (int i = 0; i < 1000; i++)
	{
		for (int j = 0; j < 1000; j++)
		{
			g.data[i][j] = 0;
		}
	}*/
	start = clock();
	product2(m, n, g);
	End = clock();
	double endtime2 = (double)(End - start) / CLOCKS_PER_SEC;
	cout << "senior_normal " << g.data[900][900] << endl;
	cout << "Total time:" << endtime2 * 1000 << "ms" << endl;
	/*start = clock();
	block1(m, n, g);
	End = clock();
	double endtime8 = (double)(End - start) / CLOCKS_PER_SEC;
	cout << "block1: " << g.data[900][900] << endl;
	cout << "Total time:" << endtime8 * 1000 << "ms" << endl;
	start = clock();
	block2(m, n, g);
	End = clock();
	double endtime3 = (double)(End - start) / CLOCKS_PER_SEC;
	cout << "block2: " << g.data[900][900] << endl;
	cout << "Total time:" << endtime3 * 1000 << "ms" << endl;
	start = clock();
	cout << "senior_block1 " << result(m, n).data[900][900] << endl;
	End = clock();
	double endtime4 = (double)(End - start) / CLOCKS_PER_SEC;
	cout << "Total time:" << endtime4 * 1000 << "ms" << endl;
	transport(n);
	start = clock();
	for (int i = 0; i < 1000; i += 4)
	{
		for (int j = 0; j < 1000; j += 4)
		{
			senior_block2(m, n, g, i, j);
		}
	}
	End = clock();
	double endtime5 = (double)(End - start) / CLOCKS_PER_SEC;
	cout << "senior_block2: " << g.data[900][900] << endl;
	cout << "Total time:" << endtime5 * 1000 << "ms" << endl;
	start = clock();
	for (int i = 0; i < 1000; i += 4)
	{
		for (int j = 0; j < 1000; j += 4)
		{
			sse_senior_block(m, n, g, i, j);
		}
	}
	End = clock();
	double endtime6 = (double)(End - start) / CLOCKS_PER_SEC;
	cout << "sse_senior_block2: " << g.data[900][900] << endl;
	cout << "Total time:" << endtime6 * 1000 << "ms" << endl;*/
	// enum CBLAS_ORDER Order = CblasRowMajor;
	// enum CBLAS_TRANSPOSE TransA = CblasNoTrans;
	// enum CBLAS_TRANSPOSE TransB = CblasNoTrans;
	// int M = 10000;//A的行数，C的行数
	// int N = 10000;//B的列数，C的列数
	// int K = 10000;//A的列数，B的行数
	// float alpha = 1;
	// float beta = 0;
	// int lda = K;//A的列
	// int ldb = N;//B的列
	// int ldc = N;//C的列
	// float * A=new float[100000000];
	// float * B=new float[100000000];
	// for (int i = 0; i < 10000; i++)
	// {
	// 	for (int j = 0; j < 10000; j++)
	// 	{
	// 		A[r++] = m.data[i][j];
	// 	}
	// }
	// r = 0;
	// for (int i = 0; i < 10000; i++)
	// {
	// 	for (int j = 0; j < 10000; j++)
	// 	{
	// 		B[r++] = n.data[i][j];
	// 	}
	// }
	// float* C=new float[100000000];
	// start = clock();
	// cblas_sgemm(Order, TransA, TransB, M, N, K, alpha, A, lda, B, ldb, beta, C, ldc);
	// End = clock();
    // endtime3 = (double)(End - start) / CLOCKS_PER_SEC;
	// cout << "openblas: " << C[0] << endl;
	// cout << "Total time:" << endtime3 * 1000 << "ms" << endl;
}

