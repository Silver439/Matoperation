#ifndef _MATOPERATION_H

#define _MATOPERATION_H

#include<iostream>
#include<vector>
using namespace std;
class matrix {
private:
	int row=0;
	int col=0;
	float** data;
	void check1();
	bool check2();
	void init();
public:
	matrix(const matrix& B);
	matrix(int m, int n);
	matrix(int m, int n,float** element);

	matrix& operator=(const matrix& B);
	matrix operator+(const matrix& B);
	matrix operator-(const matrix& B);
	matrix operator*(const matrix& B);
	matrix operator*(const float& b);

	~matrix();

	friend matrix operator*(const float a, const matrix& B);
	friend ostream& operator<<(ostream& os,const matrix& B);
};
#endif // !_MATOPERATION_H

