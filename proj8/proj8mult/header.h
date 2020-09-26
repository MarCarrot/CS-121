#ifndef HEADER_H
#define HEADER_H

#include <iostream>
#include <vector>

using namespace std;

class Matrix
{
public:
	Matrix(int x, int y);
	// inline functions
	float get_rows() {return row_nums;}
	float get_cols() {return col_nums;}
	// member functions
	float get(int row, int col);
	void set(int row, int col, float val);
	void print();
private:
	vector <vector<int> > vec2D; // vector of vectors of type ints
	int row_nums;
	int col_nums;
};

Matrix multiplication(Matrix A, Matrix B);
#endif
