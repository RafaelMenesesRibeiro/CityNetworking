// ASA Project 1
// 2016/2017 - 2st Semestre
// Instituto Superior Tecnico
// U.C. Análise e Síntese de Algoritmos
// Francisco Barros, nº 85069
// Rafael Ribeiro, nº 84758

//Must be compiled with "g++ -std=c++11 ..."

#include <algorithm>
#include <iostream>
#include <list>
#include <stdio.h>
#include <string>
#include <utility>
#include <functional>

using namespace std;
using std::pair;

/*------------------------------------------------------------------------------

			CONSTANTS

------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------

			GLOBAL VARIABLES

------------------------------------------------------------------------------*/
/*
Input variables.
	int:mc 						  number of graphVertices to connect (vertices).
	int:ma 						  maximum number of networkMaxAirports allowed in the network.
	int:mr 						  maximum number of networkMaxRoads allowed in the network.
	int:a, int:b				graphVertices.
	int:c 						 cost of building an airport on city a or a road between a and b.
*/
int mc, ma, mr, a, b, c;
/*
Output variables.
	int:networkCost				  total weight of the MST - "Minimum cost of connecting all graphVertices".
	int:networkRoads 			   number of networkMaxRoads used.
	int:networkAirports 		number of networkMaxAirports used.
*/
int networkCost = 0;
int networkRoads = 0;
int networkAirports = 0;


/*------------------------------------------------------------------------------

			STRUCTS

------------------------------------------------------------------------------*/
//Connection represents a road or airway from city int:a to city int:b.
typedef pair<int, int> Connection;
//Edge is the pair of elements that make a connection and the respective int:cost.
typedef pair<Connection, int> Edge;
//Graph

/*------------------------------------------------------------------------------

			AUXILIAR FUNCTIONS

------------------------------------------------------------------------------*/

bool edgeWeightComparator(const Edge& edge, const Edge& anotherEdge) {
  if (edge.second < anotherEdge.second) {
    return true;
  } else if (edge.second == anotherEdge.second) {
    return false;
  } else {
    return false;
  }
}

/*------------------------------------------------------------------------------

			CLASSES

------------------------------------------------------------------------------*/

class Graph {
private:
	//Abstract city to which all graphVertices with networkMaxAirports connect.
	int skyCity;
	//Number of vertices in the graph, that is to be turned into an MST.
	int graphVertices;
	//Total number of allowed networkMaxAirports in the graph.
	int networkMaxAirports;
	//Total number of allowed networkMaxRoads in the graph.
	int networkMaxRoads;
	//Listing of all concrete edges in the graph, edge = <connection, cost>.
	list<Edge> edgeList;

public:
	Graph() {
		this->skyCity = -1;
		this->graphVertices = 0;
		this->networkMaxAirports = 0;
		this->networkMaxRoads = 0;
	}

	~Graph() {}

	int getSky() {
		return skyCity;
	}

	int getCities() {
		return graphVertices;
	}

	int getAiports() {
		return networkMaxAirports;
	}

	int getRoads() {
		return networkMaxRoads;
	}

	void setCities(int c) {
		graphVertices = c;
	}

	void setAirports(int a) {
		networkMaxAirports = a;
	}

	void setRoads(int r) {
		networkMaxRoads = r;
	}

	void addEdge(Edge e) {
		edgeList.push_back(e);
	}

  void sortEdgeList() {
      edgeList.sort(edgeWeightComparator);
  }

  void showEdge(Edge e) {
    Connection connection = e.first;
    int connectionCost = e.second;
    int cityA = connection.first;
    int cityB = connection.second;
    cout << "city a: " << cityA << ", city b: " << cityB << ", cost: " << connectionCost << endl;
  }

  void printEdgeList() {
    cout << endl << "graph.printEdgeList()" << endl;
    list<Edge>::const_iterator ci;
    for (ci = edgeList.begin(); ci != edgeList.end(); ++ci) {
      showEdge(*ci);
    }
  }

	Edge newEdge(int a, int b, int c) {
		Connection con = make_pair(a, b);
		Edge e = make_pair(con, c);
		return e;
	}

	/* Calculates the MST for this graph, generating the desired network */
	void kruskalMST();
};

void Graph::kruskalMST() {
  // TODO
}

/*------------------------------------------------------------------------------

			APPLICATION

------------------------------------------------------------------------------*/

/* Application that calculates the MST */
int main() {
  /* int iterator */
	int i;
  /* edge auxiliar */
	Edge e;
  /* Instanciate new graph with default constructor */
	Graph graph;

  /* Reading inputs from standart input, given by user */

  /* Gets amount of graphVertices to connect */
	scanf("%d", &mc);
	graph.setCities(mc);

  /* Gets maximum amount of networkMaxAirports */
	scanf("%d", &ma);
	graph.setAirports(ma);

  /* Creates all airways */
	for (i = 0; i < ma; i++) {
		scanf("%d %d", &a, &c);
		e = graph.newEdge(a, graph.getSky(), c);
		graph.addEdge(e);
	}

  /* Gets maximum amount of networkMaxRoads */
	scanf("%d", &mr);
	graph.setRoads(mr);

  /* Creates all networkMaxRoads */
	for (i = 0; i < mr; i++) {
		scanf("%d %d %d", &a, &b, &c);
		e = graph.newEdge(a, b, c);
		graph.addEdge(e);
	}

  graph.printEdgeList();
  graph.sortEdgeList();
  graph.printEdgeList();
}
