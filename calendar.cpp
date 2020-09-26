
// 	https://calendars.wikia.org/wiki/Calculating_the_day_of_the_week


#include <iostream>
#include <string>
#include <cstdlib>
#include <boost/algorithm/string.hpp>
using namespace std;


bool isLeap(int y) {
	if (y % 4 == 0) {
		return true;
	} else {
		return false;
	}

}


int yearNum(int y) {
	if((y >= 1700 && y <= 1799) || (y >= 2100 && y <= 2199)) {
		return 4;
	} else if(y >= 1800 && y <= 1899) {
		return 2;
	} else if(y >= 1900 && y <= 1999) {
		return 0;
	} else if(y >= 2000 && y <= 2099) {
		return 6;
	} else {
		cout << "Not in range. Valid years from 1700 to 2099. Syntax: ./calendar [month] [year]" << endl;
   		exit(0);
	}
}


int month_int(string m, bool L) {
	boost::algorithm::to_lower(m);
	if(m == "january" || m == "1") {
		if(L) {
			return 6;
		} else {
			return 0;
		}
	} else if(m == "february" || m == "2") {
		if(L) {
			return 2;
		} else {
			return 3;
		}
	} else if(m == "march" || m == "3") {
		return 3;
	} else if(m == "april" || m == "4") {
		return 6;
	} else if(m == "may" || m == "5") {
		return 1;
	} else if(m == "june" || m == "6") {
		return 4;
	} else if(m == "july" || m == "7") {
		return 6;
	} else if(m == "august" || m == "8") {
		return 2;
	} else if(m == "september" || m == "9") {
		return 5;
	} else if(m == "october" || m == "10") {
		return 0;
	} else if(m == "november" || m == "11") {
		return 3;	
	} else if(m == "december" || m == "12") {
		return 5;										
	} else {
		cout << "Not a valid month. Syntax: ./calendar [month] [year]" << endl;
		exit(0);
	}
}


int daysInMonth(string m, bool L) {
	boost::algorithm::to_lower(m);
	string month_31[4] = {"april", "june", "september", "november"};
	for(int x = 0; x < 4; x++) {
		if (m == month_31[x] || m == "4" || m == "6" || m == "9" || m == "11") {
			return 30;
		}
	}
	if (m == "february" || m == "2") {
		if (L) {
			return 29;
		} else {
			return 28;
		}
	} else {
		return 31;
	}
}


int lastTwo(int y) {
	return (y % 100);
}


void makeCal(string month, int year, int dayVal, int days) {
	boost::algorithm::to_upper(month);
	cout << "\n" << month << " " << year << endl;
	string dashLine = "-------------------------------------------";
	string holiday;
	cout << "| Sun | Mon | Tue | Wed | Thu | Fri | Sat |" << endl;
	int n = 1;
		for(int x = 0; x <= 37+dayVal; x++) {
			if (dayVal == 7) {
				cout << "|" << endl;
				cout << dashLine << endl;
				dayVal = 0;
				x--;
			} else if (x < dayVal) {
				cout << "|     ";
			} else {
				if (month == "OCTOBER" && n == 31) {
					cout << "| 31* ";
					holiday = "* - Halloween";
				} else if (month == "JULY" && n == 4) {
					cout << "| 4*  ";
					holiday = "* - Independence Day";
				} else if (month == "DECEMBER" && n == 25) {
					cout << "| 25* ";
					holiday = "* - Christmas";					
				} else if (n < 10) {
					cout << "|  " << n << "  ";
				} else if (n > days) {
					cout << "|" << endl;
					break;
				} else {
					cout << "| " << n << "  ";
				}
				dayVal += 1;
				n += 1;
			}
		}
		cout << holiday << endl;
	}


int main(int argc,char** argv) {
	if(argc < 3) {
		cout << "Syntax Error, use ./calendar [month name] [year]";
		exit(0);
	}
	int userYear = atoi(argv[2]);
	string monthName = (argv[1]);
	isLeap(userYear);
	int yearNumVar = yearNum(userYear);
	int monthNumVar = month_int(monthName, isLeap(userYear));
	int lastTwoVar = lastTwo(userYear);
	int sum = (1 + yearNumVar + monthNumVar + lastTwoVar + lastTwoVar/4) % 7;
	int numOfDays = daysInMonth(monthName, isLeap(userYear));
	makeCal(monthName, userYear, sum, numOfDays);
}