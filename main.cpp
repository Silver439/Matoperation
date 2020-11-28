#include"Matoperation.h";
using namespace std;

int main()
{
	float** p1;
	float** p2;
	float** p3;
	p1 = new float* [2];
	p2 = new float* [2];
	p3 = new float* [3];
	for (int i = 0; i < 2; i++) {
		p1[i] = new float[3];
	}
	for (int i = 0; i < 2; i++) {
		p2[i] = new float[3];
	}
	for (int i = 0; i < 3; i++) {
		p3[i] = new float[4];
	}
	for(int i=0;i<2;i++)
	{
		for (int j = 0; j < 3; j++) {
			p1[i][j] = 1;
		}
	}
	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < 3; j++) {
			p2[i][j] = 1;
		}
	}
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 4; j++) {
			p3[i][j] = 1;
		}
	}

	matrix A(2, 3, p1);
	matrix B(2, 3, p2);
	matrix C(3, 4, p3);

	float a, b;
	a = 2.1f;
	b = 3.2f;

	cout << "a*B-A " << endl;
	cout << a * B - A << endl;
	cout << endl;
	cout << "(A*b)*C " << endl;
	cout << (A * b)* C << endl;
	cout << endl;
	cout << "(A+B)*C: " << endl;
	cout << (A + B) * C << endl;
	cout << endl;

	p1 = NULL;
	p2 = NULL;
	p3 = NULL;
	delete[] p1;
	delete[] p2;
	delete[] p3;

	return 0;
}
