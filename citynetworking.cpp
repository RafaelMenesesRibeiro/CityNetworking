// Institution: Instituto Superior Tecnico
// Course: Analysis and synthesis of algorithms
// Academic year: 2016-2017 - 2nd Semester
// 2nd Project - Connecting cities with roads and airways with MST
// Author: Francisco Barros, #85069
// Author: Rafael Ribeiro, #84758

// Compilation command: g++ -o3 -ansi -Wall -o citynetworking citynetworking.cpp
// Execution command: ./citynetworking

#include <stdio.h>
#include <iostream>
#include <vector>
#include <algorithm>
#include <utility>

/*------------------------------------------------------------------------------
			CONSTANTS
------------------------------------------------------------------------------*/
using namespace std;
using std::pair;


/*------------------------------------------------------------------------------
			GLOBAL VARIABLES
------------------------------------------------------------------------------*/
// Abstract city to which all _cities with _airports connect to.
int skyCity;
int skyCityCost = 0;
// Counts how many edges MST obtains during execution.
int airwayMstEdgeCount = 0;
// Output variables.
int airwayNetworkCost = 0;
int airwayNetworkRoads = 0;
int airwayNetworkAirports = 0;

int roadMstEdgeCount = 0;
int roadNetworkCost = 0;
int roadNetworkRoads = 0;

bool airportsWereUsed = false;
bool validRoadMst = false;
bool validAirwayMst = false;

/*------------------------------------------------------------------------------
			STRUCTS
------------------------------------------------------------------------------*/
// Connection represents a road or airway between city int:a to city int:b.
typedef pair<int, int> Connection;
// Edge is the pair of elements whose elements are a connection and the respective int:cost.
typedef pair<Connection, int> Edge;

/*------------------------------------------------------------------------------
			AUXILIAR FUNCTIONS
------------------------------------------------------------------------------*/

/**
* Auxiliar boolean function used on edgeWeightComparator for vector::sort;
* Returns true if and only if edge is not an airway to skyCity and anotherEdge is.
*/
bool airwayComparator(Edge edge1, Edge edge2) {
	// Gets the second city in the Connection pair of edge1 and edge2
	int a = edge1.first.second;
	int b = edge2.first.second;
	if (a != skyCity && b == skyCity) {
		return true;
	}	else {
			return false;
	}
}

/**
* Comparator used for sorting the vector of concrete edges with vector::sort
* Returns true (sort) if:
* Edge is lighter then anotherEdge
* Edges have the same weight but Edge isnt an airway and anotherEdge is.
* Returns false (dont sort) if:
* Edge is heavier than anotherEdge;
* Edges have same weight and both are airwayRank;
* Edges have same weight, Edge is an airway but anotherEdge isnt;
*/
bool edgeWeightComparator(const Edge& edge, const Edge& anotherEdge) {
	if (edge.second < anotherEdge.second) {
		return true;
	}	else if (edge.second == anotherEdge.second) {
			if (airwayComparator(edge, anotherEdge)) {
				return true;
			}
 		}
	return false;
}

/**
* Prints the MST data that only uses roads.
* The MST needs to be calculated first. The printed MST only uses roads.
*/
void roadOutputMst() {
	cout << roadNetworkCost << endl;
	cout << "0" << " " << roadNetworkRoads << endl;
}

/**
* Prints the MST data that only uses roads.
* The MST needs to be calculated first. The printed MST uses both roads and airways.
*/
void airwayOutputMst() {
	cout << airwayNetworkCost << endl;
	cout << airwayNetworkAirports << " " << airwayNetworkRoads << endl;
}

/*------------------------------------------------------------------------------
			CLASSES
------------------------------------------------------------------------------*/
class Graph {
	private:
		int _cities;
		int _airports;
		int _roads;
		int *roadRank;
		int *roadPredecessor;
		int *airwayRank;
		int *airwayPredecessor;
		// Vector holds all concrete Edges<Connection, cost> in this graph.
		vector<Edge> airwaysVector;
		vector<Edge> roadsVector;

	public:
		// Generic class methods.
		/** Construcor takes no arguments all values are setted */
		Graph() {
			this->_cities = 0;
			this->_airports = 0;
			this->_roads = 0;
		}

		~Graph() {
			delete[] this->roadRank;
			delete[] this->roadPredecessor;
			delete[] this->airwayRank;
			delete[] this->airwayPredecessor;
		}

		// Setters.
		void setCities(int n) {
			_cities = n;
		}

		void setAirports(int n) {
			_airports = n;
		}

		void setRoads(int n) {
			_roads = n;
		}

		// Edge methods.
		/** Creates a new edge, <connection, cost>. */
		Edge newEdge(int a, int b, int c) {
			Connection con = make_pair(a, b);
			Edge e = make_pair(con, c);
			return e;
		}

		/** Adds a new edge to this graph's roadsVector. */
		void roadAddEdge (Edge e) {
			roadsVector.push_back(e);
		}

		/** Adds a new edge to this graph's airwaysVector. */
		void airwayAddEdge(Edge e) {
			airwaysVector.push_back(e);
		}

		/** Prints to standard output the information of the given edge. */
		void printEdge(Edge e) {
			Connection connection = e.first;
			int connectionCost = e.second;
			int cityA = connection.first;
			int cityB = connection.second;
			cout << "city a: " << cityA << ", city b: " << cityB << ", cost: " << connectionCost << endl;
		}

		/** Sorts the concrete edges in this graphs roadsVector. */
		void roadsSort() {
			sort(roadsVector.begin(), roadsVector.end(), edgeWeightComparator);
		}

		/** Sorts the concrete edges in this graphs airwaysVector. */
		void airwaysSort() {
			sort(airwaysVector.begin(), airwaysVector.end(), edgeWeightComparator);
		}

		/**
		* Finds the set to which given vertex(city) belongs to the _roads only version of the MST.
		*/
		int roadsFindSet(int u) {
			if (u != roadPredecessor[u]) {
				int i = roadPredecessor[u];
				roadPredecessor[u] = roadsFindSet(i);
			}
			return roadPredecessor[u];
		}

		/**
		* Finds the set to which given vertex(city) belongs to on the generic version of the MST.
		*/
		int airwaysFindSet(int u) {
			if (u != airwayPredecessor[u]) {
				int i = airwayPredecessor[u];
				airwayPredecessor[u] = airwaysFindSet(i);
			}
			return airwayPredecessor[u];
		}

		/** Initializes one set each with one vertex equal to _roads int value. */
		void roadsMakeSet() {
			roadPredecessor = new int[_roads];
			roadRank = new int[_roads];
			for (int i = 0; i <= _roads; i++) {
				roadRank[i] = 0;
				roadPredecessor[i] = i;
			}
		}

		/** Initializes one set each with one vertex equal to skyCity int value. */
		void airwaysMakeSet() {
			airwayPredecessor = new int[skyCity];
			airwayRank = new int[skyCity];
			for (int i = 0; i <= skyCity; i++) {
				airwayRank[i] = 0;
				airwayPredecessor[i] = i;
			}
		}

		/** If u and v don't belong to the same set, unite them into the growing MST */
		void roadsUniteSet(int u, int v) {
			u = roadsFindSet(u);
			v = roadsFindSet(v);
			if (roadRank[u] > roadRank[v]) {
				roadPredecessor[v] = u;
			} else {
					roadPredecessor[u] = v;
			}
			if (roadRank[u] == roadRank[v]) {
				(roadRank[v])++;
			}
			roadMstEdgeCount++;
		}

		/** If u and v don't belong to the same set, unite them into the growing MST */
		void airwaysUniteSet(int u, int v) {
			u = airwaysFindSet(u);
			v = airwaysFindSet(v);
			if (airwayRank[u] > airwayRank[v]) {
				airwayPredecessor[v] = u;
			} else {
					airwayPredecessor[u] = v;
			}
			if (airwayRank[u] == airwayRank[v]) {
				(airwayRank[v])++;
			}
			airwayMstEdgeCount++;
		}

		/**
		* Calculates the MST for this graph, generating the desired network using
		* only _roads.
		*/
		void roadsKruskalMST() {
			int u, v, setU, setV;
			roadsMakeSet();
			roadsSort();
			vector<Edge>::const_iterator it;
			for (it = roadsVector.begin(); it != roadsVector.end(); it++) {
				u = (*it).first.first;
				v = (*it).first.second;
				setU = roadsFindSet(u);
				setV = roadsFindSet(v);
				if (setU != setV) {
					roadNetworkRoads++;
					roadNetworkCost += (*it).second;
					roadsUniteSet(setU, setV);
				}
			}
		}

		/**
		* Calculates the MST for this graph, generating the desired network using
		* both _roads and airwayRank.
		*/
		void airwaysKruskalMST() {
			int u, v, setU, setV;
			airwaysMakeSet();
			airwaysSort();
			vector<Edge>::const_iterator it;
			for (it = airwaysVector.begin(); it != airwaysVector.end(); it++) {
				u = (*it).first.first;
				v = (*it).first.second;
				setU = airwaysFindSet(u);
				setV = airwaysFindSet(v);
				if (setU != setV) {
					if (v == skyCity) {
						if (_airports > 1) {
							airwayNetworkAirports++;
							airportsWereUsed = true;
							airwayNetworkCost += (*it).second;
							airwaysUniteSet(setU, setV);
							if (skyCityCost == 0) {
								skyCityCost = (*it).second;
							}
						}
					}
					else {
						airwayNetworkRoads++;
						airwayNetworkCost += (*it).second;
						airwaysUniteSet(setU, setV);
					}
				}
			}

			if (airwayNetworkAirports == 1) {
				airwayNetworkAirports = 0;
				airwayNetworkCost -= skyCityCost;
				airportsWereUsed = false;
			}
		}
	};

/*------------------------------------------------------------------------------
			Application
------------------------------------------------------------------------------*/

int main() {
	/**
	* Input variables.
	* int:k, is the number of cities to connect on the network.
	* int:ma, is the maximum number of airports the network allows.
	* int:mr, is the maximum number of roads the network allows.
	* int:a, int:b are both identifiers representing two different cities in the network.
	* int:c represents the cost of building an airport on city a or a road between a and b.
	*/
	int a, b, c, k, ma, mr;
	// int:i is an int iterator declaration
	int i;
	// Edge:e is a variable which is used to pass edges to be added to a graph.
	Edge e;
	// Instanciate new Graph object
	Graph graph;

	// Gets the number of cities (graph vertices) meant to be connected.
	cin >> k;
	skyCity = k + 1;

	// Gets the max number of _airports (_airports).
	cin >> ma;
	graph.setAirports(ma);

	// Creates all the airwayRank, connecting all _cities with _airports to skyCity.
	for (i = 0; i < ma; i++) {
		cin >> a >> c;
		e = graph.newEdge(a, skyCity, c);
		graph.airwayAddEdge(e);
	}

	// Gets the max number of roadsVector (_roads).
	cin >> mr;
	graph.setRoads(mr);

	// Creates all the roadsVector connecting city a to city b, if a and b aren't the same
	for (i = 0; i < mr; i++) {
		cin >> a >> b >> c;
		if (a != b) {
			e = graph.newEdge(a, b, c);
			graph.roadAddEdge(e);
			graph.airwayAddEdge(e);
		}
	}

	// Runs Kruskal's algorithms to find the MST.
	graph.roadsKruskalMST();
	graph.airwaysKruskalMST();

	// Check sufficiency for the generated road MST.
	if (roadMstEdgeCount == k - 1) {
		validRoadMst = true;
	}

	// Check sufficiency for the generated airway MST.
	if ((airportsWereUsed) && (airwayMstEdgeCount == k)) {
		validAirwayMst = true;
	} else if ((!airportsWereUsed) && (airwayMstEdgeCount == k - 1)) {
		validAirwayMst = true;
	}

	// Print the proper output.
	if (!validAirwayMst && !validRoadMst) {
		cout << "Insuficiente" << endl;
	} else if (!validAirwayMst && validRoadMst) {
			roadOutputMst();
	} else if (validAirwayMst && !validRoadMst) {
			airwayOutputMst();
	}	else if (validAirwayMst && validRoadMst) {
			if (airwayNetworkCost < roadNetworkCost) {
				airwayOutputMst();
			} else if (roadNetworkCost <= airwayNetworkCost) {
					roadOutputMst();
			}
	}

	return 0;
}
