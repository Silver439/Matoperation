#include"matrix.h"
using namespace std;
void quickproduct(matrix& m, matrix& n, matrix& g);
void blockproduct(matrix& m, matrix& n, matrix& g);
int main()
{
	/*test();*/
	bool b1(0), b2(0);
	matrix m, n, g;
	input(m,b1);
	if (b1)
	{
		input(n, b2);
	}
	if(b2)
	{
		setmatrix(m.row, n.col, g);
		for (int i = 0; i < m.row; i++)
		{
			for (int j = 0; j < n.col; j++)
			{
				g.data[i][j] = 0;
			}
		}
		if (m.row * n.col < 10000)
		{
			quickproduct(m, n, g);
		}
		else
		{
			blockproduct(m, n, g);
		}
		for (int i = 0; i < m.row; i++)
		{
			for (int j = 0; j < n.col; j++)
			{
				cout << g.data[i][j] << " ";
				if (j == n.col-1) cout << endl;
			}
		}
	}
}
void quickproduct(matrix& m, matrix& n, matrix& g)
{
	for (int i = 0; i < m.row; i++)
	{
		for (int k = 0; k < m.col; k++)
		{
			int v = m.data[i][k];
			for (int j = 0; j < n.col; j++) {
				g.data[i][j] += v * n.data[k][j];
			}
		}
	}
}
void blockproduct(matrix& m, matrix& n, matrix& g)
{
	register float v, v0, v1, v2, v3;
	register int mrow, mcol, ncol, i, j, k;
	mrow = m.row;
	mcol = m.col;
	ncol = n.col;
	int x = mcol % 4;
	int y = ncol % 4;
	for (i = 0; i < mrow; i++)
	{
		for (k = 0; k < mcol - 3; k += 4)
		{
			v0 = m.data[i][k];
			v1 = m.data[i][k + 1];
			v2 = m.data[i][k + 2];
			v3 = m.data[i][k + 3];
			for (j = 0; j < ncol - 3; j += 4) {
				g.data[i][j] += v0 * n.data[k][j] + v1 * n.data[k + 1][j] + v2 * n.data[k + 2][j] + v3 * n.data[k + 3][j];
				g.data[i][j + 1] += v0 * n.data[k][j + 1] + v1 * n.data[k + 1][j + 1] + v2 * n.data[k + 2][j + 1] + v3 * n.data[k + 3][j + 1];
				g.data[i][j + 2] += v0 * n.data[k][j + 2] + v1 * n.data[k + 1][j + 2] + v2 * n.data[k + 2][j + 2] + v3 * n.data[k + 3][j + 2];
				g.data[i][j + 3] += v0 * n.data[k][j + 3] + v1 * n.data[k + 1][j + 3] + v2 * n.data[k + 2][j + 3] + v3 * n.data[k + 3][j + 3];
			}
			for (j = ncol - y; j < ncol; j++)
			{
				g.data[i][j] = v0 * n.data[k][j] + v1 * n.data[k + 1][j] + v2 * n.data[k + 2][j] + v3 * n.data[k + 3][j];
			}
		}
		for (k = mcol - x; k < mcol; k++)
		{
			v = m.data[i][k];
			for (j = 0; j < ncol; j++)
			{
				g.data[i][j] += v * n.data[k][j];
			}
		}
	}
}