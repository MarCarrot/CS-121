#include <iostream>
#include <cstdlib>
#include <iomanip>
#include <cmath>
#include <cmath>
using namespace std;


float iterate(float num, float initial) {
	float result = (initial + num/initial)/2;
	int counter = 1;
	while (abs(result - initial) > 1e-12) {
		cout << "Iterative estimate " << counter << " " << result << endl;		
		initial = result;
		result = (initial + num/initial)/2;
		counter++;
		if (counter >= 1000) {
			cout << "Sequence may be diverging. Terminating calculation." << endl;
			break;
		}
	}
	cout << "Iterative estimate " << counter << " " << result << endl;		
	return result;
}


float recursive(float num, float y, int counter) {
	float x = (y + num/y)/2; 
	counter++;
	cout << "Recursive estimate " << counter << " " << x << endl;	
	if (abs(x - y) > 1e-12 && counter < 1000) {
		x = recursive(num, x, counter);
	} else if (counter >= 1000) {
		cout << "Sequence may be diverging. Terminating calculation." << endl;
	}
	return x;
}


void verify(float N) {
	cout << "[built-in] sqrt(" << N << ") = " << sqrt(N) << endl;
	cout << "[verifying] " << sqrt(N) << "^2 = " << pow(sqrt(N), 2) << endl;

}


int main(int argc,char** argv) {
	cout << setprecision(12);
	float N = atof(argv[1]);
	float guess = atof(argv[2]);

	cout << "\n[iterative] sqrt(" << N << ") = " << iterate(N, guess) << "\n\n";
	cout << "\n[recursive] sqrt(" << N << ") = " << recursive(N, guess, 0) << "\n\n";
	verify(N);
	return 0;
}