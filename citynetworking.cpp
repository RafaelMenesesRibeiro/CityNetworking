//ASA Project 2
//2016/2017 - 2st Semestre
//Instituto Superior Tecnico
//U.C. Análise e Síntese de Algoritmos
//Francisco Barros, nº 85069
//Rafael Ribeiro, nº 84758

//Compile with "g++ -o3 -ansi -Wall -o citynetworking citynetworking.cpp"

#include <stdio.h>
#include <iostream>
#include <vector>
#include <vector>
#include <string>
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
int skyCity; //Abstract city to which all cities with airports connect to.

int airwayMstEdgeCount = 0; //Counts how many edges MST obtains during execution.
//Output variables.
int airwayNetworkCost = 0; 			//Total weight of the MST (Minimum Spanning Tree).
int airwayNetworkRoads = 0; 		//Number of roadsVector in the MST.
int airwayNetworkAirports = 0; 		//Number of airports in the MST.

int roadNetworkCost = 0; 			//Total weight of the MST (Minimum Spanning Tree).
int roadMstEdgeCount = 0;
int roadNetworkRoads = 0; 			//Number of roadsVector in the MST.

bool validRoadMst = false;
bool validAirwayMst = false;

/*------------------------------------------------------------------------------
			
			STRUCTS

------------------------------------------------------------------------------*/
//Connection represents a road or airway between city int:a to city int:b.
typedef pair<int, int> Connection;
//Edge is the pair of elements whose elements are a connection and the respective int:cost.
typedef pair<Connection, int> Edge;


/*------------------------------------------------------------------------------

			AUXILIAR FUNCTIONS

------------------------------------------------------------------------------*/
//Auxiliar function used on edgeWeightComparator for sorting.
//Prioritizes roads over airways.
//Returns true only if edge1 is not an airway to skyCity and edge2 is.
bool airwayComparator(Edge edge1, Edge edge2) {
	int a = edge1.first.second; //Gets the second city of the first connection.
	int b = edge2.first.second; //Gets the second city of the second connection.
	//If the second connection is to skyCity.
	if (a != skyCity && b == skyCity) { return true; }
	else { return false; }
}


//Comparator used for sorting the vector of concrete edges with vector::sort
//Prioritizes roads over airways.
//Returns true (sort) if:
	// - edge1 has a smaller weight is lighter.
	// - both have the same weight but edge1 isn't an airway and edge2 is.
bool edgeWeightComparator(const Edge& edge1, const Edge& edge2) {
	//If edge1 has a smaller weight.
	if (edge1.second < edge2.second) { return true;	}
  	//If both edges have the same weight.
  	else if (edge1.second == edge2.second) {
  		//If edge1 isn't an airway but edge2 is.
		if (airwayComparator(edge1, edge2)) { return true; }
 	}
	return false;
}

//Outputs the result.
void outputAirwayMst() {
	cout << airwayNetworkCost << endl;
	cout << airwayNetworkAirports << " " << airwayNetworkRoads << endl;
}
//Outputs the result.
void outputRoadsMst() {
	cout << roadNetworkCost << endl;
	cout << "0" << " " << roadNetworkRoads << endl;
}


/*------------------------------------------------------------------------------

			CLASSES

------------------------------------------------------------------------------*/
class Graph {
	private:
		int graphVertices; //Number of cities of the network.
		int graphMaxAirports; //Maximum number of airports to be built.
		int graphMaxRoads; //Maximum number of roads to be built.
		bool airportsWereUsed;
		int *predecessor, *predecessorRoads, *rank, *rankRoads;
		vector<Edge> airwayVector; //Vector that holds all Edges<Connection, cost>.
		vector<Edge> roadsVector; //Vector that holds only road edges.

	public:
		Graph(int vertices) {
			this->graphVertices = vertices;
			this->graphMaxAirports = 0;
			this->graphMaxRoads = 0;
			skyCity = vertices + 1;
			airportsWereUsed = false;
		}
		~Graph() {
			delete[] this->predecessor;
			delete[] this->predecessorRoads;
			delete[] this->rank;
			delete[] this->rankRoads;
		}

		//Getters.
		bool getWereAirportsBuilt() { return airportsWereUsed; }
		//Setters.
		void setMaxAirports(int i) { graphMaxAirports = i; }
		void setMaxRoads(int i) { graphMaxRoads = i; }

		//Creates a new Edge<connection, cost>.
		Edge newEdge(int a, int b, int c) {
			Connection con = make_pair(a, b);
			Edge e = make_pair(con, c);
			return e;
		}

		//Adds an edge to the network.
		void addEdge(Edge e) { airwayVector.push_back(e); }
		//Adds a road to the road vector.
		void addRoad (Edge e) { roadsVector.push_back(e); }

		//Sorts all the edges in the network in nondecrescing order by edge weight.
		void airwaysSort() { sort(airwayVector.begin(), airwayVector.end(), edgeWeightComparator); }
		//Sorts the road edges in the road vector in nondecrescing order by edge weight.
		void roadsSort() { sort(roadsVector.begin(), roadsVector.end(), edgeWeightComparator); }


		//Recursively finds the root of the tree the given node belongs to.
		int roadsFindSet(int u) {
			//If the vertex isn't the root of the tree.
			if (u != predecessorRoads[u]) { predecessorRoads[u] = roadsFindSet(predecessorRoads[u]); }
			return predecessorRoads[u];
		}

		//Recursively finds the root of the tree the given node belongs to.
		int airwaysFindSet(int u) {
			if (u != predecessor[u]) { predecessor[u] = airwaysFindSet(predecessor[u]); }
			return predecessor[u];
		}

		//Makes a set for each road.
		void roadsMakeSet() {
			predecessorRoads = new int[graphMaxRoads];
			rankRoads = new int[graphMaxRoads];
			for (int i = 0; i <= graphMaxRoads; i++) {
				rankRoads[i] = 0;
				predecessorRoads[i] = i;
			}
		}

		//Makes a set for each vertex.
		void airwaysMakeSet() {
			predecessor = new int[skyCity];
			rank = new int[skyCity];
			for (int i = 0; i <= skyCity; i++) {
				rank[i] = 0;
				predecessor[i] = i;
			}
		}

		//If the vertices don't belong to the same set, unites them.
		void roadsUniteSet(int u, int v) {
			u = roadsFindSet(u);
			v = roadsFindSet(v);
			if (rankRoads[u] > rankRoads[v]) { predecessorRoads[v] = u; }
			else { predecessorRoads[u] = v; }
			if(rankRoads[u] == rankRoads[v]) { (rankRoads[v])++; }
			roadMstEdgeCount++;
		}

		//If the vertices don't belong to the same set, unites them.
		void airwaysUniteSet(int u, int v) {
			u = airwaysFindSet(u);
			v = airwaysFindSet(v);
			if (rank[u] > rank[v]) { predecessor[v] = u; }
			else { predecessor[u] = v; }
			if (rank[u] == rank[v]) { (rank[v])++; }
			airwayMstEdgeCount++;
		}

		//Calculates the MST (Minimum Spanning Tree) using only the road edges.
		void roadsKruskalMST() {
			int u, v, setU, setV;
			roadsMakeSet(); //Makes a set out of each vertex.
			roadsSort(); //Sorts the road edges by nondecrescing weight.
			vector<Edge>::const_iterator it;
			for (it = roadsVector.begin(); it != roadsVector.end(); it++) {
				u = (*it).first.first;
				v = (*it).first.second;
				setU = roadsFindSet(u);
				setV = roadsFindSet(v);
				if (setU != setV) { //If the nodes weren't already in the same tree.
					roadNetworkRoads++; //Increments the roads to build counter.
					roadNetworkCost += (*it).second; //Increments the cost of the network.
					roadsUniteSet(setU, setV); //Unites the vertices.
				}
			}
		}

		//Calculates the MST (Minimum Spanning Tree) using road edges and airways.
		void airwaysKruskalMST() {
			int u, v, setU, setV, skyCityCost = 0;
			airwaysMakeSet(); //Makes a set out of each vertex.
			airwaysSort(); //Sorts all the edges by nondecrescing weight.
			vector<Edge>::const_iterator it;
			for (it = airwayVector.begin(); it != airwayVector.end(); it++) {
				u = (*it).first.first;
				v = (*it).first.second;
				setU = airwaysFindSet(u);
				setV = airwaysFindSet(v);
				if (setU != setV) { //If the nodes weren't already in the same tree.
					if (v == skyCity) { //If the edge is an airway.
						if (graphMaxAirports > 1) { //If there is more than on airway.
							airwayNetworkAirports++; //Increments the airports to build counter.
							airportsWereUsed = true; //Flags that airports were built.
							airwayNetworkCost += (*it).second; //Increments the cost of the network.
							airwaysUniteSet(setU, setV); //Unites the vertices.
							skyCityCost = (*it).second;
						}
					}
					else { //If the edge is a road edge.
						airwayNetworkRoads++; //Increments the roads to build counter.
						airwayNetworkCost += (*it).second; //Increments the cost of the network.
						airwaysUniteSet(setU, setV); //Unites the vertices.
					}
				}
			}

			if (airwayNetworkAirports == 1) { //If only one airport is to be built.
				airwayNetworkAirports = 0; //Removes the airport from the network.
				airwayNetworkCost -= skyCityCost; //Decrements the cost of the network.
				airportsWereUsed = false; //Flags that airports weren't built.
			}
		}
	};

/*------------------------------------------------------------------------------
			
			Application

------------------------------------------------------------------------------*/
int main() {
	//Input variables.
		//int:aux			used to fetch input numbers.
		//int:a, int:b		represent the cities of the connection
		//int:c				represents the cost of the connection.
	int aux, a, b, c, i, vertices;
	Edge e; //Edge (auxiliar).

	
	cin >> vertices; //Gets the number of cities (graph vertices) meant to be connected.
	Graph graph(vertices); //Constructs the graph.

	cin >> aux; //Gets the max number of airports (graphMaxAirports).
	graph.setMaxAirports(aux);
	for (i = 0; i < aux; i++) { //Creates all the airways, connecting all cities with airports to skyCity.
		cin >> a >> c;
		e = graph.newEdge(a, skyCity, c);
		graph.addEdge(e);
	}

	cin >> aux; //Gets the max number of roads (graphMaxRoads).
	graph.setMaxRoads(aux);
	for (i = 0; i < aux; i++) { //Creates all the roadsVector connecting city a to city b, if a and b aren't the same.
		cin >> a >> b >> c;
		if (a != b) {
			e = graph.newEdge(a, b, c);
			graph.addRoad(e);
			graph.addEdge(e);
		}
	}

	graph.roadsKruskalMST(); //Runs Kruskal's algorithm to find the MST, only using road edges.
	graph.airwaysKruskalMST(); //Runs Kruskal's algorithm to find the MST, road edges and airways.

	//Checks if the road MST is sufficient.
	if (roadMstEdgeCount == vertices - 1) { validRoadMst = true; }

	//Checks if the road and airways MST is sufficient.
	bool airWereUsed = graph.getWereAirportsBuilt();
	if ((airWereUsed) && (airwayMstEdgeCount == vertices)) { validAirwayMst = true; }
	else if ((!airWereUsed) && (airwayMstEdgeCount == vertices - 1)) { validAirwayMst = true; }

	//If the input given can't be modeled in to a connected graph.
	if (!validAirwayMst && !validRoadMst) { cout << "Insuficiente" << endl; }
	else if (!validAirwayMst && validRoadMst) { outputRoadsMst(); }
	else if (validAirwayMst && !validRoadMst) { outputAirwayMst(); }
	else if (validAirwayMst && validRoadMst) { //If both MSTs are suficient
			//If the MST with only road edges is cheaper or the same price to build. Prioritizes less airports.
			if (roadNetworkCost <= airwayNetworkCost) { outputRoadsMst(); }
			//If the MST with only road edges is more expensive than the MST with both road edges and airways.
			else if (roadNetworkCost < airwayNetworkCost) { outputAirwayMst(); }
	}
	return 0;
}
