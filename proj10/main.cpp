#include <iostream>
#include <vector>
#include <algorithm>
#include <stdlib.h>
#include <sstream>
#include <fstream>
#include <utility>
#include <string>
#include <tuple>
using namespace std;

#include "classes.h"


int main(int argc, char **argv) {
	if (argc != 3) {
		if (argc > 3) {
			cout << "Too many arguments.";
		} else {
			cout << "Too few arguments.";
		}
		cout << " Requires 2.\nSyntax './elevator.exe <elevator count> <file name>" << endl;
		exit(0);
	}

	if (atoi(argv[1]) <= 0) {
		cout << "There must be at least one elevator." << endl;
		exit(0);
	}

	pair<int, int> floors_iters = get_floors_and_iters(argv[2]);
	Building building0(floors_iters.second, floors_iters.first, atoi(argv[1]), argv[2]);
	building0.readin_traffic(argv[2]);
	building0.run();
	building0.summary();
   	return 0;
}