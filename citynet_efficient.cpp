// Institution: Instituto Superior Tecnico
// Course: Analysis and synthesis of algorithms
// Academic year: 2016-2017 - 2nd Semester
// 2nd Project - Connecting cities with roads and airways with MST
// Author: Francisco Barros
// Author: Rafael Ribeiro

// Compilation command: g++ -o3 -ansi -Wall citynet_efficient.cpp

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
// Abstract city to which all _cities with networkMaxAirports connect to.
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

int v, setU, setV;

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
		int networkMaxAirports;
		int networkMaxRoads;
		int *airwayRank;
		int *airwayPredecessor;
		vector<Edge> airwaysVector; //Holds all concrete Edges<Connection, cost>.

	public:
		Graph() {
			this->networkMaxAirports = 0;
			this->networkMaxRoads = 0;
		}
		~Graph() {
			delete[] this->airwayRank;
			delete[] this->airwayPredecessor;
		}

		void setAirports(int n) { networkMaxAirports = n; }
		void setRoads(int n) { networkMaxRoads = n; }

		//Creates a new edge, <connection, cost>.
		Edge edgeNew(int a, int b, int c) {
			Connection con = make_pair(a, b);
			Edge e = make_pair(con, c);
			return e;
		}
		//Adds a new edge to this graph's airwaysVector.
		void edgeAdd(Edge e) { airwaysVector.push_back(e); }

		void edgesSort() { sort(airwaysVector.begin(), airwaysVector.end(), edgeWeightComparator); }

		void setsMake() {
			airwayPredecessor = new int[skyCity];
			airwayRank = new int[skyCity];
			for (int i = 0; i <= skyCity; i++) {
				airwayRank[i] = 0;
				airwayPredecessor[i] = i;
			}
		}
		void setsClean() {
			for (int i = 0; i <= skyCity; i++) {
				airwayRank[i] = 0;
				airwayPredecessor[i] = i;
			}
		}
		int setsFind(int u) {
			if (u != airwayPredecessor[u]) {
				airwayPredecessor[u] = setsFind(airwayPredecessor[u]);
			}
			return airwayPredecessor[u];
		}

		void setsMerge(int u, int v) {
			if (airwayRank[u] > airwayRank[v]) { 
				airwayPredecessor[v] = u;
			} 
			else {
				airwayPredecessor[u] = v;
			}
			if (airwayRank[u] == airwayRank[v]) {
				(airwayRank[v])++;
			}
		}

		//Calculates the MST generating the desired network using only roads.
		void roadsKruskalMST() {
			setsMake();
			for (vector<Edge>::const_iterator it = airwaysVector.begin(); it != airwaysVector.end(); it++) {
				v = (*it).first.second;
				if (v == skyCity) { continue; }
				setU = setsFind((*it).first.first);
				setV = setsFind(v);
				if (setU == setV) { continue; }
				roadNetworkRoads++;
				roadNetworkCost += (*it).second;
				setsMerge(setU, setV);
				roadMstEdgeCount++;
			}
		}

		//Calculates the MST generating the desired network using only roads and airports.
		void airwaysKruskalMST() {
			setsClean();
			for (vector<Edge>::const_iterator it = airwaysVector.begin(); it != airwaysVector.end(); it++) {
				setU = setsFind((*it).first.first);
				v = (*it).first.second;
				setV = setsFind(v);
				if (setU == setV) { continue; }
				if (v != skyCity) { airwayNetworkRoads++; }
				else {
					airwayNetworkAirports++;
					skyCityCost = (*it).second;
				}
				airwayNetworkCost += (*it).second;
				airwayMstEdgeCount++;
				setsMerge(setU, setV);
			}
			airportsWereUsed = (networkMaxAirports > 1 && airwayNetworkAirports > 1) ? true : false;
			if (airwayNetworkAirports == 1) {
				airwayNetworkAirports = 0;
				airwayNetworkCost -= skyCityCost;
			}
		}
};

/*------------------------------------------------------------------------------
			
			Application

------------------------------------------------------------------------------*/
int main() {
	int a, b, c, k, i;
	Graph graph; //Creation of new Graph object.

	cin >> k; //Gets the number of cities (graph vertices) meant to be connected.
	skyCity = k + 1; //Declares the skyCity as the last city.

	cin >> k; //Gets the max number of networkMaxAirports (networkMaxAirports).
	graph.setAirports(k);
	for (i = 0; i < k; i++) { //Creates all the airways, connecting all cities to skyCity.
		cin >> a >> c;
		graph.edgeAdd(graph.edgeNew(a, skyCity, c));
	}

	cin >> k; //Gets the max number of roads (networkMaxRoads).
	graph.setRoads(k);
	for (i = 0; i < k; i++) { //Creates all the roads connecting city a to city b, if a and b aren't the same.
		cin >> a >> b >> c;
		if (a != b) { graph.edgeAdd(graph.edgeNew(a, b, c)); }
	}

	graph.edgesSort();
	//Runs Kruskal's algorithm twice to find the MST with less airports.
	graph.roadsKruskalMST();
	graph.airwaysKruskalMST();

	//Checks sufficiency for the generated road MST.
	if (roadMstEdgeCount == k - 1) { validRoadMst = true; }
	//Checks sufficiency for the generated airway MST.
	if ((airportsWereUsed && (airwayMstEdgeCount == k)) || (!airportsWereUsed && (airwayMstEdgeCount == k - 1))) { validAirwayMst = true; }

	//Prints the proper output.
	if (!validAirwayMst && !validRoadMst) { cout << "Insuficiente" << endl; }
	else if (!validAirwayMst && validRoadMst) { roadOutputMst(); }
	else if (validAirwayMst && validRoadMst) {
		if (airwayNetworkCost < roadNetworkCost) { airwayOutputMst(); }
		else { roadOutputMst(); }
	}
	return 0;
}
