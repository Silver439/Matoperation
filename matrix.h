#ifndef MATRIX_H_
#define MATRIX_H_
#include <iostream>
#include<random>
#include<ctime>
//#include<cblas.h>
#include <immintrin.h>
using namespace std;
struct matrix
{
	int row;
	int col;
	float** data;
};
float* matrix_1d(int n);

float** matrix_2d(int m, int n);

void setmatrix(int m, int n,matrix &g);

void transport(matrix &g);

void input(matrix &u);

void testdata1(matrix& p);

void testdata2(matrix& p);

void product(matrix& m, matrix& n, matrix& g);

void product2(matrix& m, matrix& n, matrix& g);

void block1(matrix& m, matrix& n, matrix& g);

void block2(matrix& m, matrix& n, matrix& g);

void senior_block(matrix& m, float** n, matrix& g, int row, int col);

void senior_block2(matrix& m, matrix& n, matrix& g, int i, int j);

void sse_senior_block(matrix& m, matrix& n, matrix& g, int i, int j);

matrix result(matrix &A,matrix& B);

void test();
 #endif