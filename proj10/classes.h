enum Dir { DOWN=0, UP=1 };


class Person
{
  int id;
  int start; // time when arrives at source floor
  int src; // source floor
  int dest; // destination floor
  int embarkTime; // boarding car
  int arriveTime; // arriving at destination flloor

public:

  Person(int i,int Time,int s,int d); // constuctor: pass-in id, time, src, dest
  bool operator==(const Person& p); // based on id, to check if 2 persons are same
  bool operator<(const Person& p); // based on start time, for sorting
  void setEmbarkTime(int x);
  void setArriveTime(int x);
  Dir dir(); // what direction are they travelling?
  void print();
  int getTime() const;
  int getID() const;
  int getSrc() const;
  int getDest() const;
  int getArrive() const;
  int getEmbark() const;

};


class Container
{
  vector<Person> persons;

public:
  void Press(int f);
  bool Pressed(int f);
  void Reset(int f);
  vector<Person> getPersons() const;
  void AddPerson(Person p);
  void RemovePerson(Person p);
  vector<bool> buttons; // true means pressed

};


class Floor : public Container
{
  int id;


public:

  Floor(int i); // pass-in id; initialize 2 buttons to false
  bool UpPressed();
  bool DownPressed();
  void PressUp();
  void PressDown();
  void ClearUp();
  void ClearDown();
  string toString();
  int getFloorID() const;

};

class Car : public Container
{
  int id; // set in constructor
  int capacity; // don't let more than this many poeple on
  enum State {MOVING=1, LOADING=0}; // whatever states you want
  State state;
  Dir dir; // what direction is car moving?
  int nfloors;

public:
  int currentFloor;
  Car(int i,int f); // initialize with id and number of floors; initilize state, dir, and buttons
  string toString();
  void printSymbolic(); // print something like "CAR0[3]^"
  // this is where all the logic goes...
  void update(vector<Floor>& floors,int iter,vector<Person>& allpersons); // *****
  // it is important to get these functions right...
  void embark(Floor& floor,int iter);
  void disembark(Floor floor,int iter,vector<Person>& allpersons);
  int getCarID() const;
};


pair<int, int> get_floors_and_iters(char* fname);


class Building
{
  int ncars;
  int nfloors;
  int ITERS;

  vector<Floor> floors;
  vector<Car> cars;
  vector<Person> persons;

public:

  void AddPerson(Person p);
  void RemovePerson(Person p);
  void readin_traffic(char* fname);
  Building(int numiters,int numfloors,int numcars,char* fname);
  void NewArrivals(int iter); // check for person whose start time has arrived
  void summary(); // print-out all persons, cars, and floors at end
  void run()
  {

    int C = ncars;
    for (int iter=0 ; iter<ITERS ; iter++)
    {
      cout << endl << "iter=" << iter << endl;
      // check for Persons whose start time is now, move to source floor
      NewArrivals(iter); 

      // print out contents of floors and cars
      for (int i=0 ; i<nfloors ; i++) 
      { 
        int f=nfloors-i-1;
        cout << floors[f].toString();
        for (int j=0 ; j<C ; j++)
          if (cars[j].currentFloor==f) cars[j].printSymbolic();
        cout << endl; 
      }
      for (int i=0 ; i<C ; i++) cout << cars[i].toString() << endl;

      // update cars...
      for (int i=0 ; i<C ; i++) cars[i].update(floors,iter,persons);
    }
  }
};