#include <iostream>
#include <cstdlib>
#include <vector>
#include <cmath>
#include <string.h>
#include <bitset>

using namespace std;


void readVector(vector<string> C) {
	for (int y = 0; y < C.size(); y++) {
		cout << C[y] << " ";
	}
	cout << endl;
}


void iterate(vector<string> C) {
	const int terms = pow(2, C.size());
	for (int i = 0; i < terms; i++) {
		string binaryVal = bitset<8>(i).to_string();
		for (int y = 0; y < binaryVal.size(); y++) {
			if (binaryVal[y] == '1') {
				cout <<(C[7-y]) << " ";
			}
		}
		cout << endl;

	}
}


int main(int argc,char** argv) {
	vector<string> userVector;
	for (int i = 1; i < argc; i++) {
		userVector.push_back(argv[i]);
	}
	readVector(userVector);
	iterate(userVector);


	return 0;
}