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
/*
Output variables.
	int:networkCost				total weight of the MST - "Minimum cost of connecting all graphVertices".
	int:networkRoads			number of networkMaxRoads used.
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

	int getNumberVertices() {
		return graphVertices;
	}

	int getMaxAirports() {
		return networkMaxAirports;
	}

	int getMaxRoads() {
		return networkMaxRoads;
	}

	void setNumberVertices(int i) {
		graphVertices = i;
	}

	void setMaxAirports(int i) {
		networkMaxAirports = i;
	}

	void setMaxRoads(int i) {
		networkMaxRoads = i;
	}

	//Creates a new edge, <connection, cost>.
	Edge edgeCreate(int a, int b, int c) {
		Connection con = make_pair(a, b);
		Edge e = make_pair(con, c);
		return e;
	}
	//Adds the new adge to the graph.
	void edgeAdd(Edge e) { edgeList.push_back(e); }

	//Checks if the vertex has connections.
	bool vertexCheckConnected(int city) {
		for (list<Edge>::const_iterator it = edgeList.begin(); it != edgeList.end(); ++it) {
			Connection connection = (*it).first; //Gets the connection of the edge.
			int cityA = connection.first; //Gets one city.
			int cityB = connection.second; //Gets the other city.
			if (cityA == city || cityB == city) { //If any of the cities match the given one.
				return true; //Returns true if it finds the given city as connections.
			}
		}
		return false; //Returns false if the given city as no connections.
	}


	//Calculates the MST for this graph, generating the desired network.
	void kruskalMST();
};

void Graph::kruskalMST() {
	//TODO - @RafaelRibeiro
}

/*------------------------------------------------------------------------------

			AUXILIAR FUNCTIONS

------------------------------------------------------------------------------*/
//Auxiliar function to print the output.
void outputInsuficient() { cout << "Insuficiente" << endl; }


/*------------------------------------------------------------------------------

			CODE EXECUTION

------------------------------------------------------------------------------*/
//Application that calculates the MST.
int main() {
	int i;
	/*
	Input variables.
		int scanfAux		variable to fetch the numbers.
		int:a, int:b		graphVertices.
		int:c				cost of building an airport on city a or a road between a and b.
	*/
	int scanfAux, a, b, c;
	Edge e; //Edge (auxiliar).
	Graph graph; //Instanciate new graph with default constructor.

	/*--------------------------------------------------------------------------

			Creation of the Graph

	--------------------------------------------------------------------------*/
	scanf("%d", &scanfAux); //Gets the number of vertices (graphVertices) to connect.
	graph.setNumberVertices(scanfAux);

	scanf("%d", &scanfAux); //Gets the max number of airports (networkMaxAirports).
	graph.setMaxAirports(scanfAux);
	for (i = 0; i < scanfAux; i++) { //Creates all the airways.
		scanf("%d %d", &a, &c);
		e = graph.edgeCreate(a, graph.getSky(), c);
		graph.edgeAdd(e);
	}
	
	scanf("%d", &scanfAux); //Gets the max number of roads (networkMaxRoads).
	graph.setMaxRoads(scanfAux);
	for (i = 0; i < scanfAux; i++) { //Creates all the roads.
		scanf("%d %d %d", &a, &b, &c);
		e = graph.edgeCreate(a, b, c);
		graph.edgeAdd(e);
	}

	/*--------------------------------------------------------------------------

			Insuficiency Verification

	--------------------------------------------------------------------------*/
	//If the number os total connections isn't enough to connect the graph.
	if (graph.getMaxRoads() + graph.getMaxAirports() < graph.getNumberVertices() - 1) {
		outputInsuficient(); //Prints the output.
		return 0; //Quits.
	}
	//Checks if all the vertices have at least one connection.
	for (i = 1; i <= graph.getNumberVertices(); i++) {
		if(!graph.vertexCheckConnected(i)) { //If the city has no connections.
			outputInsuficient(); //Prints the output.
			return 0; //Quits.
		}
	}

}
