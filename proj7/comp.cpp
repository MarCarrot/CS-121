#include <iostream>
#include <string>
#include <cstdlib>
#include <vector>
#include <algorithm>
#include <sstream>
#include <iomanip>
#include <cctype>
#include <cmath>
using namespace std;

class DoMath{
	public:
		DoMath();
		void MathParse(string readEq);	
		void Print();			
		int get_power() const;
		int get_group() const;
		float get_val() const;
		void set_group(int g);		
		void set_val(float v);		
		void set_power(int p);		

	private:
		float val;
		int power;
		int group;
};


DoMath::DoMath() {
	// very handy, defaults to 1
	power = 1;
	val = 1;
}


void DoMath::MathParse(string readEq) {
	// 3x^2+1
	int untilVar = readEq.find("x");
	if (untilVar == -1) { // if no x is found
		try {
			val = stof(readEq); // its just a constant
		} catch (std::invalid_argument) {
			cerr << "Invalid syntax." << endl;
			exit(0);
		}
	power = 0;
	}
	else { // else is a variable
		int untilCar = readEq.find("^");
		if (untilCar == -1) { // if ^ is not found
			if (readEq[untilVar + 1] != '\0') { // tests if there's nothing after variable
				cout << "Syntax Error 2. " << endl;			
				exit(0); // if there's nothing, that's not allowed, i.e. x3 is invalid
			}
		} else { // if ^ is found
			string powerString = readEq.substr(untilCar+1, readEq.length()-untilCar); // takes value of everything after ^
			// stoi was giving a weird run time error, atoi works better 
			power = atoi(powerString.c_str()); 
		}
		string valString = readEq.substr(0, untilVar);
		if (valString == "-") {
			val = -1;
		}
		else if (!valString.empty()) { 
			try {
				val = stof(valString); 
			} catch (std::invalid_argument) {
				cerr << "Invalid syntax. Unexpected value in '" << valString << "x^" <<this->get_power() << "' term."<< endl;
				exit(0);
			}
		}
	}
}


void DoMath::Print() {
	cout << "Value:\t" << val << endl;
	cout << "Exponent:\t" << power << endl;
	cout << "Group:\t" << group << endl;
	cout << "-------" << endl;
}


int DoMath::get_power() const {
	return power;
}


float DoMath::get_val() const {
	return val;
}


void DoMath::set_val(float v) {
	val = v;
}


void DoMath::set_group(int g) {
	group = g;
}


void DoMath::set_power(int p) {
	power = p;
}


int DoMath::get_group() const {
	return group;
}


bool my_sort (const DoMath &a, const DoMath &b){
	// useful reference:
	// https://www.walletfox.com/course/sortvectorofcustomobjects.php
    return a.get_power() > b.get_power();
}


void replaceString(string & line, string original, string replaced) {
	// useful reference thispointer.com/find-and-replace-all-occurrences-of-a-sub-string-in-c/
	size_t pos = line.find(original); // the first time it appears
	while(pos != string::npos) { // until it reaches end of string
		// replace from occurance index till length of original with 'replaced'
		line.replace(pos, original.size(), replaced);
		// then find the next occurance, search starting from the last occurance
		pos = line.find(original, pos + replaced.size());
	}
}


void add(vector<DoMath> poly1) {
	string final_line = " > ";
	int analyze_power;
	// this vector is sorted, highest power is @ first index
	// initalizing variables for first instance of loop
	analyze_power = poly1[0].get_power(); // highest power
	int inter = 0;
	int sum = poly1[0].get_val();
	int number_counts = poly1.size();
	for (int i = 1; i < number_counts; ++i) // start @ index 1 since index 0 was already interperated
	{
		if (analyze_power == poly1[i].get_power()) { // if numbers have same degrees, add to sum
			sum += poly1[i].get_val();
		} else { // if it iterates to new degree
			analyze_power = poly1[i].get_power(); // save the value of this new degree
			if (sum != 0) {
				final_line.append(to_string(sum) + "x^" + to_string(poly1[i-1].get_power())); // append previous number to string
				final_line.append(" + ");
			}
			sum = poly1[i].get_val(); // initialize new sum
		}
	}
	final_line.append(to_string(sum) + "x^" + to_string(poly1[number_counts-1].get_power()));
	replaceString(final_line, "x^0", "");
	replaceString(final_line, "x^1", "x");
	replaceString(final_line, " 1x", " x");
	replaceString(final_line, " -1x", " -x");
	replaceString(final_line, "+ -", "- ");
	replaceString(final_line, "+ 0", "");
	cout << final_line;
}


void prod(vector<DoMath> poly1) { 
	vector<DoMath> group_1;
	vector<DoMath> group_2;
	for (int i = 0; i < poly1.size(); ++i) { // logic to seperate into groups
		if (poly1[i].get_group() == 1) {
			group_1.push_back(poly1[i]);
		} else {
			group_2.push_back(poly1[i]);
		}
	}
	if (group_2.size() == 0) { // avoids segfault with invalid inputs
		cerr << "Syntax Error: No second polynomial provided." << endl;
		exit(0);
	}

	vector<DoMath> instances;
	for (int j = 0; j < group_1.size(); j++) // take term of polynomial
	{
		for (int k = 0; k < group_2.size(); ++k) // iterate through terms of 2nd poly
		{
			DoMath newInstance;
			newInstance.set_val(group_1[j].get_val() * group_2[k].get_val()); // multiply values
			newInstance.set_power(group_1[j].get_power() + group_2[k].get_power()); // sum powers
			instances.push_back(newInstance);
		}
	}
	sort(instances.begin(), instances.end(), my_sort );
	add(instances);
}


float evaluate(vector<DoMath> poly1, float x = 1) {
	float sum = 0;
	for (int i = 0; i < poly1.size(); ++i)
	{
		sum += poly1[i].get_val() * pow(x, poly1[i].get_power());
	}
	return sum;
}


float derivative(vector<DoMath> poly1, bool eval, float x = 1) {
	string final_line = " > ";
	vector<DoMath> instances;
	for (int i = 0; i < poly1.size(); i++) {
		DoMath newInstance;
		if (poly1[i].get_power() != 0) { // no need to compute values with degree of 0
			newInstance.set_power(poly1[i].get_power()-1);
			newInstance.set_val(poly1[i].get_val() * poly1[i].get_power());
			instances.push_back(newInstance);
		}
	}
	if (eval) {
		return evaluate(instances, x);
	} else {
		add(instances);
	}
}


void roots(vector<DoMath> poly1, double initial = 5) {
	double f_eval = evaluate(poly1, initial);
	double f_eval_deriv = derivative(poly1, true, initial);
	double new_inital = initial - (f_eval / f_eval_deriv);
	while ( abs(new_inital - initial) >= 1e-12) {
		initial = new_inital;
		f_eval = evaluate(poly1, initial);
		f_eval_deriv = derivative(poly1, true, initial);
		new_inital = initial - (f_eval / f_eval_deriv);
	}
	cout << new_inital << endl;
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


void parse_string(string input, vector<string> & V) {
	if (input.at(0) != '-') input.insert(0, "+");
	replaceString(input, "-", "+-");
	V = split(input, '+');
}


string inputs_to_vector(string line, vector<DoMath> & V) {
	cout << "> " << line << endl;
	vector<string> stringVector;
	stringVector = split(line);

	for (int i = 1; i < stringVector.size(); i++) {
		string input = stringVector[i];
		vector<string> inputVector;

		parse_string(input, inputVector);
		for (int j = 0; j < inputVector.size(); j++) {
			DoMath poly;
			poly.MathParse(inputVector[j]);	
			poly.set_group(i-1);
			V.push_back(poly);
		}
	}
	sort(V.begin(), V.end(), my_sort );
	return stringVector[0];
}


void get_operation(string oper, vector<DoMath> vectorMath) {
	if (oper == "sum" || oper == "add") {
		cout << "Finding sum..." << endl;
		add(vectorMath);
	} else if (oper == "prod" || oper == "product") {
		cout << "Finding product..." << endl;
		prod(vectorMath);
	} else if (oper == "derivative" || oper == "deriv") {
		cout << "Finding derivative..." << endl;
		derivative(vectorMath, false);
	} else if (oper == "root") {
		cout << "Finding root..." << endl;
		roots(vectorMath);
	} else if (oper == "quit" || oper == "exit") {
		cout << "Exiting program..." << endl;
		exit(0);
	} else {
		cout << "Invalid command." << endl;
	}
}


int main(int argc, char** argv) {
	string line;
    cout << "\nEnter a line: ";
    while (getline(cin, line)) {
    	vector<DoMath> vectorMath;

        string operation = inputs_to_vector(line, vectorMath); // vectorMath gets filled in via reference, line is the operation to be done

        get_operation(operation, vectorMath); // executes operation
        
        cout << "\nEnter a line: ";
        
    }
}