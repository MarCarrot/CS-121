#include <iostream>
#include <vector>
#include <algorithm>
#include <stdlib.h>
#include <sstream>
#include <fstream>
#include <tuple>
#include <string>
#include <utility>
using namespace std;

#include "classes.h"

Person::Person(int i,int Time,int s,int d) {
	id = i;
	start = Time;
	src = s;
	dest = d;
	embarkTime = 0;
	arriveTime = 0;
}

Dir Person::dir() {
	if (dest > src) {
		return UP;
	} else {
		return DOWN;
	}
}

void Person::print() {
	cout << "ID: " << id << endl;
	cout << "Start: " << start << endl;
	cout << "Source: " << src << endl;
	cout << "Destination: " << dest << endl;
	cout << "Direction: " << dir() << endl;
}

Building::Building(int numiters,int numfloors,int numcars, char* fname) {
	ncars = numcars;
	nfloors = numfloors;
	ITERS = numiters;
}


int Person::getTime() const {
	return start;
}


int Person::getID() const {
	return id;
}


int Person::getSrc() const {
	return src;
}


int Person::getDest() const {
	return dest;
}


int Person::getArrive() const {
	return arriveTime;
}


int Person::getEmbark() const {
	return embarkTime;
}


vector<int> split(string line, char d = ' ') {
	vector<int> makeVector;
    istringstream SS(line); string token;
    while(getline(SS,token, d)) {
    	istringstream SS2(token);
    	string token2;
    	while(SS2 >> token2) makeVector.push_back(atoi(token2.c_str()));
    }
    return makeVector;
}

bool sort_function(Person a, Person b) {
	return a.getTime() > b.getTime();
}


void Container::AddPerson(Person p) {
	persons.push_back(p);
}


void Container::RemovePerson(Person p) {
	vector<Person> persons2;
	for(unsigned int i = 0; i < persons.size(); i++) {
		if ( persons[i].getID() != p.getID() ) {
			persons2.push_back(persons[i]);
		}
	}
	persons = persons2;
}


bool sortbysec(const tuple<int, int, int, int>& a,  
               const tuple<int, int, int, int>& b) 
{ 
    return (get<1>(a) < get<1>(b)); 
} 


void Building::readin_traffic(char* fname) {
	ifstream filein(fname);
	if (!filein.good()) {
		cout << "File no good: \"" << fname << "\""<< endl;
		exit(0);
	}
	vector<tuple<int, int, int, int> > tupleVector;
	bool readFirstLine = false;
	for(string line; getline( filein, line ); ) {
		if (readFirstLine) {
			vector<int> splitLine = split(line);
			tupleVector.push_back( make_tuple(splitLine[2], splitLine[4], splitLine[6], splitLine[8]) );
		} else {
			readFirstLine = true;
		}
	}
	sort(tupleVector.begin(), tupleVector.end(), sortbysec);
	for (auto iterate : tupleVector) {
		Person personInstance(  get<0>(iterate) ,  get<1>(iterate) ,  get<2>(iterate) ,  get<3>(iterate) );
		persons.push_back(personInstance);
	}
	for(int i = 0; i < this->nfloors; i++) {
		Floor floorObject(i);
		floors.push_back(floorObject);
	}
	for(int i = 0; i < this->ncars; i++) {
		Car carObject(i, this->nfloors);
		cars.push_back(carObject);
	}
}


Floor::Floor(int i) {
	id = i;
	buttons.push_back(0);
	buttons.push_back(0);

}

void Container::Press(int f) {
}

void Floor::PressUp() {
	buttons[0] = 1;
}

void Floor::PressDown() {
	buttons[1] = 1;
}


vector<Person> Container::getPersons() const {
	return persons;
}


string Floor::toString() {
	string returnString;
	returnString.append("FLOOR ");

	returnString.append(to_string(this->id));
	returnString.append(", persons ");
	returnString.append(to_string(this->getPersons().size()));
	returnString.append(", up ");
	returnString.append(to_string(this->buttons[0]));
	returnString.append(", down ");
	returnString.append(to_string(this->buttons[1]));	

	return returnString;
}


string Car::toString() {
	string returnString;
	returnString.append("CAR ");
	returnString.append(to_string(this->id));
	returnString.append(", floor ");
	returnString.append(to_string(this->currentFloor));
	returnString.append(", state ");
	returnString.append(to_string(this->state));
	returnString.append(", dir ");
	returnString.append(to_string(this->dir));
	returnString.append(", buttonsPressed ");
	for(unsigned int i = 0; i < this->buttons.size(); i++) {
		if (buttons[i]) {
			returnString.append(to_string(i));
			returnString.append(" ");
		}
	}
	return returnString;
}


Car::Car(int i, int f) {
	id = i;
	nfloors = f;
	capacity = 10;
	currentFloor = 0;
	dir = UP;
	state = LOADING;
	for(int i = 0; i < nfloors; i++) {
		buttons.push_back(0);
	}
}


// ContainerObject is object of class Container. ContainerObject.getPersons() returns vector of Person objects.
void Building::NewArrivals(int iter) {
	for(auto iterator : persons) {
		if (iterator.getTime() == iter) {
			floors[ iterator.getSrc() ].Press( iterator.getSrc() );
			floors[ iterator.getSrc() ].AddPerson( iterator );
			if (iterator.getDest() > iterator.getSrc()) { // going up
				floors[ iterator.getSrc() ].PressUp();
			} else {
				floors[ iterator.getSrc() ].PressDown();
			}
		}
	}
}


pair<int, int> get_floors_and_iters(char* fname) {
	ifstream filein(fname);
	if (!filein.good()) {
		cout << "File no good: \"" << fname << "\""<< endl;
		exit(0);
	}
	pair<int, int> returnPair;
	for(string line; getline( filein, line ); ) {
		vector<int> vectorForm = split(line);
		returnPair = make_pair(vectorForm[1], vectorForm[3]);
		break;
	}
	return returnPair;
}


void Car::printSymbolic() {
	cout << " CAR" << id << "[" << to_string(this->getPersons().size()) << "]";
	if (this->state == MOVING) {
		if (dir == UP) {
			cout << "^";
		} else if (dir == DOWN) {
			cout << "v";
		}
	} else if (this->state == LOADING) {
		cout << "*";
		this->state = MOVING;
	}
}


void Person::setEmbarkTime(int x) {
	embarkTime = x;
}

void Person::setArriveTime(int x) {
	arriveTime = x;
}



void Car::embark(Floor& floor,int iter) {
	if(this->state == LOADING) {
		for(auto iterate : floor.getPersons()) {
			if (this->dir == iterate.dir()) {
				if (iterate.getSrc() == currentFloor) {
					floor.RemovePerson(iterate);
				}
			}
		}
	} else if (this->state == MOVING) {
		if(this->dir == UP) {
			this->currentFloor += 1;
		} else {
			this->currentFloor -= 1;
		}
	}
}


int Floor::getFloorID() const {
	return id;
}

int Car::getCarID() const {
	return id;
}


void Floor::ClearUp() {
	buttons[0] = 0;
}


void Floor::ClearDown() {
	buttons[1] = 0;
}


void Car::update(vector<Floor>& floors, int iter, vector<Person>& allpersons) {
	for (unsigned int i = 0; i < this->buttons.size(); ++i)
	if (this->dir == UP && this->currentFloor == floors[floors.size()-1].getFloorID()) {
		this->dir = DOWN;
	}
	if (this->dir == DOWN && this->currentFloor == floors[0].getFloorID()) {
		this->dir = UP;
	}

	bool peopleAdded = false;
	for(auto iterate : floors[this->currentFloor].getPersons()) { 
		if (iterate.dir() == this->dir) 
		{ // picks up people 
			for( unsigned int i = 0; i < allpersons.size(); i++) {
				if (allpersons[i].getID() == iterate.getID()) {
					allpersons[i].setEmbarkTime(iter);
				}
			}		
			peopleAdded = true;
			this->state = LOADING;
			this->AddPerson(iterate);
			this->buttons[iterate.getDest()] = 1;
			if (this->dir == UP) {
				floors[this->currentFloor].ClearUp();
			} else {
				floors[this->currentFloor].ClearDown();
			}
		}
	}
	for(auto iterate : this->getPersons()) { //iterates through every person in elevator car
		if (this->currentFloor == iterate.getDest()) {
			this->buttons[currentFloor] = 0;
			this->state = LOADING;
			peopleAdded = true;
			cout << "Person " << iterate.getID() << " is getting ready to disembark from car " << this->id << " to floor " << iterate.getDest() << " time " << iter << endl;
			for( unsigned int i = 0; i < allpersons.size(); i++) {
				if (allpersons[i].getID() == iterate.getID()) {
					allpersons[i].setArriveTime(iter);
				}
			}
			this->RemovePerson(iterate);
		}
	}
	if (!peopleAdded) this->state = MOVING;


	embark(floors[currentFloor], iter);
}


void Car::disembark(Floor floor,int iter,vector<Person>& allpersons) {

}

void Building::summary() {
	float sumWaitTime = 0;
	int totalPeople = 0;
	bool readExceptions = false;
	for(auto iterator : persons) {
		if (readExceptions == false) {
			for (auto iterator2 : this->cars) {
				for (auto iterator3 : iterator2.getPersons()) {
					cout << "in car " << iterator2.getCarID() << ": Person ID " << iterator3.getID()
					<< " src " << iterator.getSrc() << " dest " << iterator.getDest() 
					<< " embarked " << iterator.getEmbark() << " arrived -1" << endl;
				}
			}
			for (auto iterator2 : this->floors) {
				for (auto iterator3 : iterator2.getPersons()) {
					cout << "on floor " << iterator2.getFloorID() << ": Person ID " << iterator3.getID()
					<< " src " << iterator.getSrc() << " dest " << iterator.getDest() 
					<< " embarked -1 arrived -1  WAITING ON FLOOR " << iterator2.getFloorID() << endl;
				}
			}
			readExceptions = true;
		} else if (iterator.getArrive() > iterator.getEmbark()) {
			cout << "Person ID " << iterator.getID() << " src " << iterator.getSrc() << " dest " << iterator.getDest() 
			     << " start " << iterator.getTime() << " embarked " << iterator.getEmbark() << " arrived " << iterator.getArrive() 
			     << " traveltime " << iterator.getArrive() - iterator.getTime() << endl;
		     sumWaitTime += iterator.getArrive() - iterator.getTime();
		     totalPeople += 1;
			}
	}
	cout << "Average wait time: " << sumWaitTime / totalPeople << endl;
}