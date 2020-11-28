#include"Matoperation.h";
using namespace std;

static vector<int> address;
static vector<int> counter;

void matrix::init()
{
	data = new float*[row];
	for (int i = 0; i < row; i++)
	{
		data[i] = new float[col] {0};
	}
	if (row * col != 0)
	{
		address.push_back((int)&(data[0][0]));
		counter.push_back(1);
	}
}

	void matrix:: check1()
	{
		bool b = 0;
		for (int i = 0; i < address.size(); i++)
		{
			if ((int)&(data[0][0]) == address[i]) {
				counter[i] += 1;
				b = 1;
				break;
			}
		}
		if (b == 0) {
			address.push_back((int)&(data[0][0]));
			counter.push_back(1);
		}
	}

	bool matrix::check2()
	{
		int i;
		for (i = 0; i < address.size(); i++)
		{
			if ((int)&(data[0][0]) == address[i]) {
				break;
			}
		}
		if (counter[i] == 1) {
			return 1;
		}
		else {
			counter[i] -= 1;
			return 0;
		}
	}

matrix::matrix(const matrix& B)
{
	row = B.row;
	col = B.col;
	data = B.data;
	check1();
}

matrix::matrix(int m, int n)
{
	row = m;
	col = n;
	init();
}

matrix::matrix(int m, int n,float** element)
{
	bool b = 0;
	for (int i = 0; i < address.size(); i++)
	{
		if ((int)&(element[0][0]) == address[i]) {
			counter[i] += 1;
			b = 1;
			break;
		}
	}
	if (b == 0) {
		address.push_back((int)&(element[0][0]));
		counter.push_back(1);
	}
	row = m;
	col = n;
	data = element;
}


matrix& matrix::operator=(const matrix& B)
{
	if (row * col != 0) {
		if (check2()) {
			cout << row << "," << col << ":" << "Done!" << endl;
			delete[] data;
		}
		else {
			cout << row << "," << col << ":" << "Wait!" << endl;
		}
	}
	row = B.row;
	col = B.col;
	data = B.data;
	check1();
	return *this;
}

matrix matrix::operator+(const matrix& B)
{
	if (row == B.row && col == B.col) {
		int m = row;
		int n = B.col;
		matrix C(m, n);
		for (int i = 0; i < m; i++)
		{
			for (int j = 0; j < n; j++)
			{
				C.data[i][j] = data[i][j] + B.data[i][j];
			}
		}
		return C;
	}
	else {
		cout << "You can't add two matrix with different size!";
		return *this;
	}
}
matrix matrix::operator-(const matrix& B)
{
	if (row == B.row && col == B.col) {
		int m = row;
		int n = B.col;
		matrix C(m, n);
		for (int i = 0; i < m; i++)
		{
			for (int j = 0; j < n; j++)
			{
				C.data[i][j] = data[i][j] - B.data[i][j];
			}
		}
		return C;
	}
	else {
		cout << "You can't minus two matrix with different size!";
		return *this;
	}
}

matrix matrix::operator*(const matrix &B)
{
	if (col == B.row) {
		matrix C(row, B.col);
		for (int i = 0; i < row; i++)
		{
			for (int k = 0; k < col; k++)
			{
				float v = data[i][k];
				for (int j = 0; j < B.col; j++)
				{
					C.data[i][j] += v * B.data[k][j];
				}
			}
		}
		return C;
	}
	else {
		cout << "Please multiply two matrix that the first matrix's col is equal to the second matrix's row.";
		return *this;
	}
}

matrix matrix::operator*(const float& b)
{
	matrix C(row, col);
	for (int i = 0; i < row; i++) {
		for (int j = 0; j < col; j++)
		{
			C.data[i][j] = b * data[i][j];
		}
	}
	return C;
}

matrix operator*(const float a, const matrix& B)
{
	matrix C(B.row, B.col);
	for (int i = 0; i < B.row; i++) {
		for (int j = 0; j < B.col; j++)
		{
			C.data[i][j] = a * B.data[i][j];
		}
	}
	return C;
}

ostream& operator<<(ostream& os,const matrix& B)
{
	for (int i = 0; i < B.row; i++)
	{
		for (int j = 0; j < B.col; j++)
		{
			os << B.data[i][j]<<" ";
			if (j == B.col - 1) os << endl;
		}
	}
	return os;
}

matrix::~matrix()
{
	if (check2()) {
		cout <<row<<","<<col<<":"<< "Done!" << endl;
		delete[] data;
	}
	else {
		cout <<row<<","<<col<<":"<< "Wait!" << endl;
	}
}
