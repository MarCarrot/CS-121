elevator.exe : main.o StoreItem.o
	g++ -std=c++11 main.o StoreItem.o -o elevator.exe

StoreItem.o : StoreItem.cpp classes.h
	g++ -std=c++11 -Wall -c StoreItem.cpp 

main.o : main.cpp classes.h
	g++ -std=c++11 -Wall -c main.cpp 

clean :
	rm *.o elevator.exe