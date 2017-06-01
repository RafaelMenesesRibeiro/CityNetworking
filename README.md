# CityNetworking
[University Project]

[2nd Year - 2nd Semester]

Objective: Calculate the minimum cost of building R roads and A airports to connect all the N cities in a country.  

Language: C++  
Efficiency: O[(R+A) * log(N)]  
Method: Kruskal's algorithm  

- The cities are represented by an integer between 1 and N.
- Cities with airports are connected with any city with airports. Cities without airports need roads to be connected.
- A road connects city A to B and B to A.
- In case there is more than one minimum cost solution, the one with the least airports is chosen.


The input consists of:
- one line with the number N of cities to connect;
- one line with the maximum number A of allowed airports to build (0 <= A <= N);
- a sequence of A lines with 2 integers 'a' and 'b':
	- 'a' - city in which the airport can be built;
	- 'b' - cost of building the given airport;
- one line with the maximum number R of allowed roads to build;
- a sequence of R line with 3 integers:
	- 'a' - city where the road starts/ends;
	- 'b' - city where the road starts/ends;
	- 'c' - cost of building the given road;

The output is one of 3 possible:
- "Insuficiente" (Insufficient): the input is not sufficient to connect all the cities;
- Two lines in case the input is sufficient:
	- a line with the total cost (minimum possible);
	- a line with 'a' and 'b' integers. 'a' is the number of airports to build and 'b' is the number of roads to build;
