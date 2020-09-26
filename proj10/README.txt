Syntax: ./elevator.exe <elevator count> <file name>

The program already parses for the number of iterations and number of floors based on the first line of the .txt file provided
( "floors 5 iters 100" ).

There must be at least one car. 
The elevator is not intelligent; it just oscilates up and down and picks up/drops off people accordingly, as in the example test cases.

The classes were not changed too much from the provided template. The only modications were more getters/setter functions, and the disembark member function was not utilized. 

The elevator has two states, LOADING, for passengers coming in or coming out, and MOVING, for the elevator in motion. 

The final average wait time is printed out in the program. 

C++ 11, will probably not work in older versions

Accidently removed new person arriving line when removing debug lines. I didn't want to add it back because I didn't want late submission. 
