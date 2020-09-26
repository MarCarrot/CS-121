#include <iostream>
#include <cstdlib>
#include <fstream>
#include <vector>
#include <string>
#include <cmath>

using namespace std;


vector<int> stats(vector<int> li) {
	int sum = 0;
	int n = li.size();
	float sqrtsum = 0;
	int min = li[0];
	int max = li[0];

	for(int i = 0; i < n; i++) { // finds sum, min, max, eventually mean
		sum += li[i];
		if (li[i] < min) min = li[i];
		if (li[i] > max) max = li[i];
	}

	float mean = (float)sum/n;
	cout << "Number of values is " << n << endl;
	cout << "Greatest value is " << max << endl;
	cout << "Least value is " << min << endl;
	cout << "The range is " << max - min << endl;
	cout << "The average is " << mean << endl;

	for(int i = 0; i < n; i++) { // finds stdev
		sqrtsum += pow(abs(li[i] - mean), 2);
	}
	cout << "The standard deviance is " << sqrt(sqrtsum/n) << endl;

	vector<int> mm;
	mm.push_back(max);
	mm.push_back(min);
	return mm;
}


void histo(vector<int> minmax, vector<int> li, int bins_amt, int sc) {
	float bin = (minmax[0] - minmax[1]) / (float)bins_amt;
	// initializes parallel vectors, storing value range and freq.
	vector<int> hist_range;
	vector<int> hist_vals(bins_amt+1);

	for (int i = 0; i <= bins_amt+1; i++) { // creates the bin ranges
		hist_range.push_back(minmax[1] + i*bin);
		cout << minmax[1] + i*bin << "  -  " << endl;
	}

	for (int i = 0; i < li.size(); i++) { // iterates each game's points
		for (int y = 0; y <= bins_amt; y++) { // iterates the bin ranges
			if(li[i] >= hist_range[y] && li[i] <= hist_range[y+1]-1) {
				hist_vals[y] += 1; // counts how many times points are within a certain range
			}
		}
	}

	for (int i = 0; i <= bins_amt; i++) {
		cout << hist_range[i] << " - " << hist_range[i+1]-1 << " ";
		for (int y = 0; y < hist_vals[i]; y++) if((y+1)%sc==0) cout << "*";
		cout << endl;
	}
}


int main(int argc,char** argv)
{
	ifstream input;
	int user_bins;
	int scale;

	// all the logic to interperate user input
	if (argc == 1) { // 1 arg, default file is mj_points, the amount michael jordan scored in every game. default bins = 20
		input.open("mj_points.txt"); 
		user_bins = 20;
		scale = 2;
	} else if (argc == 2) { // 2 args, user input file, default bins = 20
		input.open(argv[1]);
		user_bins = 20;
		scale = 2;
	} else if (argc == 3) { // 3 args or more, user input file, user input bins
		input.open(argv[1]);
		user_bins = atoi(argv[2]);
		scale = 2;
	} else if (argc == 4) {
		input.open(argv[1]);
		user_bins = atoi(argv[2]);
		scale = atoi(argv[3]);
	} else {
		cout << "Syntax error. Usage: ./histo <file name> <bin amount> <scale>" << endl;
		exit(0);
	}

	vector<int> points;
	string word;

while (input >> word) { // stores data in vector
  	int b = atoi(word.c_str());
  	points.push_back(b);
  }


vector<int> range = stats(points); // prints min,max,mean,stdev, returns min,max.

histo(range, points, user_bins, scale); // creates histogram
input.close();
return 0;
}
