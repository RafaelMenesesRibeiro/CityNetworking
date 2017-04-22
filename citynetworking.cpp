// ASA Project 1
// 2016/2017 - 2st Semestre
// Instituto Superior Tecnico
// U.C. Análise e Síntese de Algoritmos
// Francisco Barros, nº 85069
// Rafael Ribeiro, nº 84758

//Must be compiled with "g++ -std=c++11 ..."

#include <stdio.h>
#include <iostream>
#include <list>
#include <string>
#include <algorithm>
#include <utility>

using namespace std;
using std::pair;

/*------------------------------------------------------------------------------

			CONSTANTS

------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------

			GLOBAL VARIABLES

------------------------------------------------------------------------------*/

/**
* Output variables.
* int:networkCost is the total weight of the MST - "Minimum cost of connecting all graphVertices".
* int:networkRoads is the number of networkMaxRoads that were actually used.
*	int:networkAirports is the number of networkAirports that were actually used.
*/
int networkCost = 0;
int networkRoads = 0;
int networkAirports = 0;

/*------------------------------------------------------------------------------

			STRUCTS

------------------------------------------------------------------------------*/

// Connection represents a road or airway between city int:a to city int:b.
typedef pair<int, int> Connection;
// Edge is the pair of elements whose elements are a connection and the respective int:cost.
typedef pair<Connection, int> Edge;
// Graph

/*------------------------------------------------------------------------------

			AUXILIAR FUNCTIONS

------------------------------------------------------------------------------*/

/** Comparator used for sorting the list of concrete edges with list::sort */
bool edgeWeightComparator(const Edge& edge, const Edge& anotherEdge) {
  if (edge.second < anotherEdge.second) {
    return true;
  } else if (edge.second == anotherEdge.second) {
    return false;
  } else {
    return false;
  }
}


/** Prints the output in case of impossibility of creating a proper network. */
void outputInsuficient() { cout << "Insuficiente" << endl; }

/*------------------------------------------------------------------------------

			CLASSES

------------------------------------------------------------------------------*/
class Graph {
	private:
		// Abstract city to which all graphVertices with an airport connect.
		int skyCity;
		// Number of vertices in the graph, that is to be turned into an MST.
		int graphVertices;
		// Total number of allowed networkMaxAirports in the graph.
		int networkMaxAirports;
		// Total number of allowed networkMaxRoads in the graph.
		int networkMaxRoads;
		// Listing of all concrete edges in the graph, edge = <connection, cost>.
		list<Edge> edgeList;
	public:
		Graph() {
			this->skyCity = -1;
			this->graphVertices = 0;
			this->networkMaxAirports = 0;
			this->networkMaxRoads = 0;
		}
		~Graph() {}

		// Getters.
		int getSkyCity() { return skyCity; }
		int getGraphVertices() { return graphVertices; }
		int getMaxAirports() { return networkMaxAirports; }
		int getMaxRoads() { return networkMaxRoads; }

		// Setters.
		void setGraphVertices(int i) { graphVertices = i; }
		void setMaxAirports(int i) { networkMaxAirports = i; }
		void setMaxRoads(int i) { networkMaxRoads = i; }

		// Adds the new adge to the graph.
		void addEdge(Edge e) { edgeList.push_back(e); }
		// Sorts in crescent order the list of concrete edges in this graph.
		void sortEdgeList() { edgeList.sort(edgeWeightComparator); }

		// Prints to standart output the information of a given edge.
	  void showEdge(Edge e) {
	    Connection connection = e.first;
	    int connectionCost = e.second;
	    int cityA = connection.first;
	    int cityB = connection.second;
	    cout << "city a: " << cityA << ", city b: " << cityB << ", cost: " << connectionCost << endl;
	  }

		// Prints to standart output the information of all edges in this graph.
	  void printEdgeList() {
	    cout << endl << "graph.printEdgeList()" << endl;
	    list<Edge>::const_iterator ci;
	    for (ci = edgeList.begin(); ci != edgeList.end(); ++ci) {
	      showEdge(*ci);
	    }
	  }

		// Calculates the MST for this graph, generating the desired network.
		void kruskalMST();

		// Checks if the vertex has connections.
		bool vertexCheckConnected(int city) {
			list<Edge>::const_iterator it;
			for (it = edgeList.begin(); it != edgeList.end(); ++it) {
				Connection connection = (*it).first; //Gets the connection of the edge.
				int cityA = connection.first; //Gets one city.
				int cityB = connection.second; //Gets the other city.
				if (cityA == city || cityB == city) { //If any of the cities match the given one.
					return true; //Returns true if it finds the given city as connections.
				}
			}
			return false; //Returns false if the given city as no connections.
		}

		// Creates a new edge, <connection, cost>.
		Edge newEdge(int a, int b, int c) {
			Connection con = make_pair(a, b);
			Edge e = make_pair(con, c);
			return e;
		}

};

void Graph::kruskalMST() {
	// TODO - @RafaelRibeiro
}

/*------------------------------------------------------------------------------

			CODE EXECUTION

------------------------------------------------------------------------------*/

/**
* Application that calculates the MST.
*/
int main() {
	int i;
	/**
	* Input variables.
	* int:aux variable is used to fetch the numbers such as number of cities, etc.
	* int:a, int:b are both graphVertices, representing two cities.
	* int:c represents the cost of building an airport on city a or a road between a and b.
	*/
	int aux, a, b, c;
	// Edge (auxiliar).
	Edge e;
	// Instanciate new graph with default constructor.
	Graph graph;

	/* Creation of the Graph */
	scanf("%d", &aux); //Gets the number of vertices (graphVertices) to connect.
	graph.setGraphVertices(aux);

	scanf("%d", &aux); //Gets the max number of airports (networkMaxAirports).
	graph.setMaxAirports(aux);
	for (i = 0; i < aux; i++) { //Creates all the airways.
		scanf("%d %d", &a, &c);
		e = graph.newEdge(a, graph.getSkyCity(), c);
		graph.addEdge(e);
	}

	scanf("%d", &aux); //Gets the max number of roads (networkMaxRoads).
	graph.setMaxRoads(aux);
	for (i = 0; i < aux; i++) { //Creates all the roads.
		scanf("%d %d %d", &a, &b, &c);
		if (a != b) { //If the vertices aren's the same - avoids self loops.
			e = graph.newEdge(a, b, c);
			graph.addEdge(e);
		}
	}

	/* Insuficiency Verification */
	// If the number os total connections isn't enough to connect the graph.
	if (graph.getMaxRoads() + graph.getMaxAirports() < graph.getGraphVertices() - 1) {
		outputInsuficient(); //Prints the output.
		return 0; //Quits.
	}
	// Checks if all the vertices have at least one connection.
	for (i = 1; i <= graph.getGraphVertices(); i++) {
		if(!graph.vertexCheckConnected(i)) { //If the city has no connections.
			outputInsuficient(); //Prints the output.
			return 0; //Quits.
		}
	}

	/*--------------------------------------------------------------------------

			Minimum Spanning Tree finding (Kruskal's algorithm)

	--------------------------------------------------------------------------*/
	graph.printEdgeList();
	graph.sortEdgeList();
	graph.printEdgeList();
	graph.kruskalMST();
}
