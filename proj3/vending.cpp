#include <iostream>
#include <string>
#include <cstdlib>
#include <fstream>
#include <iomanip>
#include <vector>
#include <cmath>

using namespace std;


vector<string> split(string parse_line) {
	char delimiter = ' ';
	parse_line += delimiter;
	vector<string> splitVector;
	int occurance = 0;
	for(int i = 0; i < parse_line.size(); i++) {
		if (parse_line.at(i) == delimiter) {
			occurance += 1;
		}
	}
	for (int i = 0; i < occurance; i++) {
		string section = parse_line.substr(0, parse_line.find(delimiter)); // section of string from start until it reaches delimeter
		if (section.empty()) { // if its white space...
			parse_line.erase(0, 1); // delete and reloop
		} else {
			splitVector.push_back(section);
			parse_line.erase(0, section.size()+1); // erase from index 0 to delimiter
		}
	}
	return splitVector;
}


void printVend(vector<string> name, vector<int> quan, vector<float> cost) {
	int wide = 12;
	cout << setw(5) << "\nQuarters: " << quan[0] << endl;
	cout << setw(5) << "Dimes: " << quan[1] << endl;
	cout << setw(5) << "Nickels: " << quan[2] <<  "\n\n" << endl;

	cout << setw(wide) << "Item" << setw(wide) << "Quantity" << setw(wide) << "Cost" << endl;
	for (int i = 3; i < name.size(); i++) {
    	cout << setw(wide) << name[i];
    	cout << setw(wide) << quan[i];
    	cout << setw(wide) << cost[i] << endl;
    }
    cout << endl;
}


int change(float & money_left, int amount, int C) {
	int giveCoins = money_left / C;
	if (giveCoins > amount) return 0;
	money_left = (money_left+1) - (giveCoins * C);
	return giveCoins;
}


float giveChange(float & money,  vector<int> & quan) {
    cout << "Change: " << money << endl; 
    money *= 100;
    int quarters = 0; int dimes = 0; int nickels = 0;

    if (quan[0] > 0) {
    	quarters = change(money, quan[0], 25);
    	quan[0] -= 1;
    }
    if (quan[1] > 0) {
    	dimes = change(money, quan[1], 10);
    	quan[1] -= 1;
    }
    if (quan[2] > 0) {
    	nickels = change(money, quan[2], 5);
    	quan[2] -= 1;    	
    }

    if (quarters != 0) cout << "   Dispensing " << quarters << " quarters." << endl;
    if (dimes != 0) cout << "   Dispensing " << dimes << " dimes." << endl;
    if (nickels != 0) cout << "   Dispensing " << nickels << " nickels." << endl;
    money = 0;
}


int userInput(float & bal, string prompt, vector<string> name, vector<int> & quan, vector<float> cost) {
	vector<string> input_parse = split(prompt);
	bool foundItem = false;
	for (int i = 0; i < input_parse.size(); i++) {
		if (input_parse[i] == "quarter") {
			bal += 0.25;
			quan[0] += 1;
		} else 	if (input_parse[i] == "dime") {
			bal += 0.10;
			quan[1] += 1;
		} else if (input_parse[i] == "nickel") {
			bal += 0.05;
			quan[2] += 1;
		} else if(input_parse[i] == "print") {
			printVend(name, quan, cost);
		} else if (input_parse[i] == "change") {
			giveChange(bal, quan);
			return 0;
		} else if (input_parse[i] == "press") {
			for (int j = 0; j < name.size(); j++) {
				if (input_parse[i+1] == name[j]) {
					if (quan[j] == 0) {
						cout << "This item is out of stock." << endl;
						return 0;
					}
					if (bal >= cost[j]) {
						cout << "You have purchased a " << name[j] << "." << endl;
						bal -= cost[j];
						quan[j] -= 1;
						giveChange(bal, quan);
					} else {
						cout << "You don't have enough money to purchase this." << endl;
					}
					foundItem = true;
				}
			}
			if (!foundItem) cout << "Item not found. Enter a valid item." << endl;
			return 0;
		} else if (input_parse[i] == "exit" || input_parse[i] == "quit") {
			cout << "Exiting ..." << endl;
			exit(0);
		} else {
			cout << "Syntax Error";
			exit(0);
		}
	}
}


int main(int argc,char** argv) {
	bool inUse = true;
	vector<string> vendName;
	vector<int> vendQuan;
	vector<float> vendCost;
	ifstream filein("vending1.txt");
	string delimiter = " ";

	for (string line; getline(filein, line); ) { // gets each line
		vector<string> parsed = split(line); // returns vector of every element parsed
		vendName.push_back(parsed[0]);
		vendQuan.push_back(atoi(parsed[1].c_str()));
		vendCost.push_back(atof(parsed[2].c_str()));
    }

    float balance = 0;
    cout << fixed << setprecision(2);
    while (inUse) {
    	char choice[100];
    	cout << "What would you like to do? ";
    	cin.getline(choice, sizeof(choice));
    	userInput(balance, choice, vendName, vendQuan, vendCost);
    	cout << "Balance in machine: " << balance << endl;
	}
	return 0;
}