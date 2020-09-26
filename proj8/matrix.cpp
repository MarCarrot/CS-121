#include <iostream>
#include <vector>
#include <sstream>
#include <string>
#include <fstream>
#include <cmath>
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
	vector <vector<float> > vec2D; // vector of vectors of type ints
	int row_nums;
	int col_nums;
};


Matrix::Matrix(int x, int y) {
	// below, x is row number (outside vector)
	// y is column number (inside vector) with initial values of zero
	vec2D = vector <vector<float> > (x, vector<float>(y, 0));
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
	float sum;
	Matrix C(A.get_rows(), B.get_cols());

	if (!(A.get_rows() == B.get_cols() && A.get_cols() == B.get_rows())) {
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


Matrix transpose(Matrix A) {
	Matrix C(A.get_cols(), A.get_rows()); // # of rows is now # of cols, vice versa
	for (int i = 0; i < A.get_rows(); i++) {
		for (int j = 0; j < A.get_cols(); j++) {
			C.set(j, i, A.get(i, j));
		}
	}
	return C;
}


vector<string> split(string line, char d = ' ') {
	vector<string> makeVector;
    istringstream SS(line); string token;
    while(getline(SS,token, d)) {
    	istringstream SS2(token);
    	string token2;
    	while(SS2 >> token2) makeVector.push_back(token2);
    }
    return makeVector;
}


class Iris_Data // assumption is that iris.dat is being read
{ // code is not universal; it will not work for any non-iris data
	public: 
		float data1;
		float data2;
		float data3;
		float data4;
};


void correlation_matrix(Matrix covariance) {
	Matrix correlation(covariance.get_rows(), covariance.get_cols());
	for (int i = 0; i < covariance.get_rows(); ++i)
	{
		for (int j = 0; j < covariance.get_cols(); ++j)
		{
			float denominator = sqrt( pow(covariance.get(i, i),2) * pow(covariance.get(j, j),2) );
			correlation.set(i, j, covariance.get(i, j) / denominator);
		}
	}
	cout << "Correlation:" << endl;
	correlation.print();
}


void covariance_matrix(Matrix centeredMatrix) {
	Matrix centeredMatrixTransposed = transpose(centeredMatrix);
	Matrix covariance = multiplication(centeredMatrixTransposed, centeredMatrix);
	cout << "Covariance:" << endl;
	covariance.print();
	correlation_matrix(covariance);
}


void irisToMatrix(vector<Iris_Data> get_iris, int iris_rows) {
	Matrix irisMatrix(iris_rows, 4);
	Matrix irisMatrixCentered(iris_rows, 4);
	vector<float> sums(4);
	for (int i = 0; i < iris_rows; ++i)
	{
		irisMatrix.set(i, 0, get_iris[i].data1);
		sums[0] += get_iris[i].data1;
		irisMatrix.set(i, 1, get_iris[i].data2);
		sums[1] += get_iris[i].data2;
		irisMatrix.set(i, 2, get_iris[i].data3);
		sums[2] += get_iris[i].data3;
		irisMatrix.set(i, 3, get_iris[i].data4);
		sums[3] += get_iris[i].data4;
	}
	for (int i = 0; i < iris_rows; ++i)
	{
		irisMatrixCentered.set(i, 0, get_iris[i].data1 - sums[0]/iris_rows);
		irisMatrixCentered.set(i, 1, get_iris[i].data2 - sums[1]/iris_rows);
		irisMatrixCentered.set(i, 2, get_iris[i].data3 - sums[2]/iris_rows);
		irisMatrixCentered.set(i, 3, get_iris[i].data4 - sums[3]/iris_rows);
	}
	covariance_matrix(irisMatrixCentered);
}


void readfile(char* fname) {
	ifstream filein(fname);
	vector<Iris_Data> get_iris;
	int iris_rows = 0;
	if (!filein.good()) {
		cout << "File not found. Syntax: ./matrix (file name)" << endl;
		exit(0);
	}
	for(string line; getline(filein, line); ) {
		Iris_Data tempInstance;
		vector<string> split_by_string = split(line, ',');
		if (split_by_string[4] == "Iris-setosa") {
			tempInstance.data1 = stof(split_by_string[0]);
			tempInstance.data2 = stof(split_by_string[1]);
			tempInstance.data3 = stof(split_by_string[2]);
			tempInstance.data4 = stof(split_by_string[3]);
			get_iris.push_back(tempInstance);
			iris_rows++;
		}
	}
	irisToMatrix(get_iris, iris_rows);
}



void rotations(float x, float y, float angle) {
	Matrix point2D(1, 2);
	point2D.set(0, 0, x);
	point2D.set(0, 1, y);
	angle = angle * M_PI/180;
	Matrix rot2D(2, 2);
	rot2D.set(0, 0, cos(angle));
	rot2D.set(0, 1, -sin(angle));
	rot2D.set(1, 0, sin(angle));
	rot2D.set(1, 1, -cos(angle));
	cout << "\nX = " << x << " Y = " << y << " Degrees (rad) = " << angle << endl;
	cout << "Rotation" << endl;
	rot2D.print();
}


void test_cases() {
	Matrix my_instance_1(3, 2);
	my_instance_1.set(0, 0, 2.3);
	my_instance_1.set(0, 1, 4);
	my_instance_1.set(1, 0, 7.7);
	my_instance_1.set(1, 1, 8);
	my_instance_1.set(2, 0, 12.5);
	my_instance_1.set(2, 1, 13.1);
	my_instance_1.print();

	Matrix my_instance_3 = transpose(my_instance_1);
	my_instance_3.print();

	Matrix my_instance_2(2, 2);
	my_instance_2.set(0, 0, 2);
	my_instance_2.set(0, 1, 4);
	my_instance_2.set(1, 0, 7);
	my_instance_2.set(1, 1, 8);
	my_instance_2.print();

	Matrix product = multiplication(my_instance_1, my_instance_2);
	product.print();
}


int main(int argc,char** argv) {
	rotations(2.5, 4.330, 30);
	readfile(argv[1]);
}