#include <iostream>
#include <vector>
#include <stdio.h>
#include "header.h"
using namespace std;


Matrix::Matrix(int x, int y) {
	// below, x is row number (outside vector)
	// y is column number (inside vector) with initial values of zero
	vec2D = vector <vector<int> > (x, vector<int>(y, 0));
	row_nums = x;
	col_nums = y;
}



float Matrix::get(int row, int col) {
	return vec2D[row][col];
}


void Matrix::set(int row, int col, float val) {
		vec2D[row][col] = val; // modifies matrix val at row x, col y
}	


void Matrix::print() {
    for(int vec = 0; vec < vec2D.size(); vec++) {
        for(int x = 0; x < vec2D[vec].size(); x++) {
            cout << vec2D[vec][x];
            if (x != vec2D[vec].size() - 1) cout << ", "; // place a comma unless its the last element
        }
            cout << endl;
    }
    cout << endl;
}

Matrix multiplication(Matrix A, Matrix B) {
	int sum;
	Matrix C(A.get_rows(), B.get_cols());

	if (A.get_rows() == B.get_cols() && A.get_cols() == B.get_rows()) {
		cout << "Performing Multiplication..." << endl;
	} else {
		cout << "Matrix dimensions are not equal, cannot perform multiplication" << endl;
	}

	for (int i = 0; i < A.get_rows(); i++) {
		for (int j = 0; j < B.get_cols(); j++) {
			sum = 0;
			for (int k = 0; k < A.get_cols(); k++) {
				sum += (A.get(i, k) * B.get(k, j));
			}
			C.set(i, j, sum);
		}
	}
	return C;
}