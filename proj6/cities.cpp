#include <iostream>
#include <sstream>
#include <vector>
#include <fstream>
#include <cstdlib>
#include <cmath>
#include <math.h>
#include <string>
#include <iomanip>
#include <algorithm>

using namespace std;

struct Cities { // structs for listing all cities & their closest/furthest city
	string name;
	float lat;
	float lon;
};

struct CityInfo { // structs for holding data of city names and respective distance from user inputted city
	string city_name;
	float distance;
};

float find_distance(float lat1, float lon1, float lat2, float lon2) {
	// uses Haversine formula to get distance between points
	// necessary to convert deg. to rad.
	const float deg_to_rad = M_PI/180;
	lat2 *= deg_to_rad;
	lat1 *= deg_to_rad;
	lon2 *= deg_to_rad;
	lon1 *= deg_to_rad;
	float d_lat = lat2 - lat1;
	float d_lon = lon2 - lon1;
	float a = pow(sin(d_lat/2),2) + cos(lat1) * cos(lat2) * pow(sin(d_lon/2),2);
    float c = 2*asin(sqrt(a));
  	float d = 3961.0 * c;
  	return d;
}


vector<Cities> get_data(char* fname) {
	ifstream filein(fname);
	if (!filein.good()) {
		cout << "File not found. Exiting..." << endl;
		exit(0);
	}
	vector<Cities> holdData; // initializes vector of type Cities
	for(string line; getline(filein, line); ) { // parses by space. assuming source file is seperated by spaces.
		Cities getInfo;
		istringstream inSS(line);
		string a;
		string b;
		string c;
		inSS >> a;
		inSS >> b;
		inSS >> c;
		getInfo.name = a;
		getInfo.lat = atof(b.c_str());
		getInfo.lon = atof(c.c_str());
		holdData.push_back(getInfo);
	}
	return holdData;
}


void findMinMax(vector<float> myVector, vector<Cities> V, int n) {
	vector<float> min_max; // index 0 = lowest value, index 1 = lowest index, index 2 = highest value, index 3 = highest index
	min_max.push_back(myVector[0]);
	min_max.push_back(0);
	min_max.push_back(0);
	min_max.push_back(0);
	for (int i = 0; i < myVector.size(); ++i) {
		if (myVector[i] < min_max[0]) {
			min_max[0] = myVector[i];
			min_max[2] = i+1;
		} else if (myVector[i] > min_max[1]) {
			min_max[1] = myVector[i];
			min_max[3] = i+1;
		}
	}
	cout << setw(14) << left << V[n].name << setw(14) << " closest = " << V[min_max[3]].name << "  (" << min_max[0]
	<< setw(14) << " miles)," << "farthest = " << V[min_max[2]].name << "  (" << min_max[1] << " miles) " << endl;	
}


void print_distances(vector<Cities> V){
	// prints out furthest/closest city for all cities
	float d_city;

	for (int i = 0; i < V.size(); i++) { // iterates through cities
		vector<float> hold_distances;
		for (int x = 0; x < V.size(); x++) { // iterates through all other cities besides itself
			if (i != x) {
				d_city = find_distance(V[x].lat, V[x].lon, V[i].lat, V[i].lon);
				hold_distances.push_back(d_city); 
			}
		}
		findMinMax(hold_distances, V, i); // runs function to find max/min of vector
	}
}


bool compareStructs(const CityInfo &a, const CityInfo &b) { // function for sort
    return a.distance < b.distance;
}


void city_info(vector<Cities> V, int amt, string city) {
	cout << "Showing " << amt << " closest cities to " << city << "..." << endl;
	bool foundCity = false; // flag to determine if city is found
	int num_cities = V.size(); 
	int city_index;
	vector<CityInfo> city_vector;

	// test cases
	for (int i = 0; i < num_cities; ++i)
	{
		if (V[i].name == city) {
			foundCity = true;
			city_index = i;
		}
	}
	if (!foundCity) {
		cout << "The city of '" << city << "' was not found. Exiting program." << endl;
		exit(0);
	}
	if (amt >= num_cities) {
		cout << "The entered amount of " << amt << " cities to be shown exceeds the total amount of cities (" << num_cities-1 << ")." << endl;
		cout << "Exiting program." << endl;
		exit(0); 
	}


	float d_city;
	for (int x = 0; x < V.size(); x++) {
		if (x != city_index) {
			d_city = find_distance(V[x].lat, V[x].lon, V[city_index].lat, V[city_index].lon);
			CityInfo CityInfoStruct;
			CityInfoStruct.city_name = V[x].name; 
			CityInfoStruct.distance = d_city;
			city_vector.push_back(CityInfoStruct);
		}
	}

	sort(city_vector.begin(), city_vector.end(), compareStructs); // sorts by magnitude of distances

	for (int y = 0; y < amt; y++) {
		cout << y+1 << ".) " << city_vector[y].city_name << " " << city_vector[y].distance << " miles." << endl;
	}

}


int main(int argc,char** argv) {
	if (argc == 2) {
		vector<Cities> CityInfo = get_data(argv[1]); // returns a vector of structs, interperating file contents
		print_distances(CityInfo);
	}
	else if (argc == 4) {
		string cityName = argv[2];
		int showAmt = atoi(argv[3]);
		vector<Cities> CityInfo = get_data(argv[1]);
		city_info(CityInfo, showAmt, cityName);		
	} else {
		cout << "Incorrect Syntax. Usage: ./cities <file name> [city name] [list amount]" << endl;
	}
	// find_distance(55, 37, 59, 30);
}