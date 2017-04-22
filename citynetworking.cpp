//ASA Project 1
//2016/2017 - 2st Semestre
//Instituto Superior Tecnico
//U.C. Análise e Síntese de Algoritmos
//Francisco Barros, nº 85069
//Rafael Ribeiro, nº 84758

//Must be compiled with "g++ -std=c++11 ..."

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
int skyCity; //City that connects all the cities with airports.

//Output variables.
int networkCost = 0; //Total weight of the MST (Minimum Spanning Tree).
int networkRoads = 0; //Number of roads in the MST.
int networkAirports = 0; //Number of airports in the MST.


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
//Auxiliar boolean function used on edgeWeightComparator for vector::sort
//Returns true if and only if edge is not an airway and anotherEdge is.
bool airwayComparator(Edge edge1, Edge edge2) {
	int a = edge1.first.second; //Gets cityB of the connection1.
	int b = edge2.first.second; //Gets cityB of the connection2.
	if (a != skyCity && b == skyCity) { //If cityB1 is not skyCity and cityB2 is.
		return true;
	}
	else { 
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
* Edges have same weight and both are airways;
* Edges have same weight, Edge is an airway but anotherEdge isnt;
*/
bool edgeWeightComparator(const Edge& edge, const Edge& anotherEdge) {
	if (edge.second < anotherEdge.second) //If the weight of the first edge is less.
    	return true;
  	else if (edge.second == anotherEdge.second) { //If the weights are equal.
		if (airwayComparator(edge, anotherEdge)) { //Prioritizes roads over airports.
			return true;
		}
 	}
	else //If the weight of the first city is more.
  		return false;
}

//Prints the output in case of impossibility of creating a proper network.
void outputInsuficient() { cout << "Insuficiente" << endl; }
//Prints the output when a proper newtwork is created.
void outputMST(int totalCost, int numberAirports, int numberRoads) {
	cout << totalCost << endl;
	cout << numberAirports << " " << numberRoads << endl;
}


/*------------------------------------------------------------------------------

			CLASSES

------------------------------------------------------------------------------*/
struct DisjointSets {
	int *parent, *rnk;
	int n;

	DisjointSets(int vert) {
		this->n = vert + 1;
		rnk = new int[vert + 1];
		parent = new int[vert + 1];
		for (int i = 0; i < vert + 1; i++) {
			rnk[i] = 0;
			parent[i] = i;
		}
	}
	int find(int u) {
		if (u != parent[u]) {
			parent[u] = find(parent[u]);
		}
		return parent[u];
	}
	void merge(int x, int y) {
		x = find(x);
		y = find(y);
		if (rnk[x] > rnk[y]) {
			parent[y] = x;
		}
		else {
			parent[x] = y;
		}
		if(rnk[x] == rnk[y]) {
			rnk[y]++;
		}
	}
};


class Graph {
private:
	//Abstract city to which all graphVertices with an airport connect.
	int skyCity;
	//Number of vertices in the graph, that is to be turned into an MST.
	int graphVertices;
	//Total number of allowed networkMaxAirports in the graph.
	int networkMaxAirports;
	//Total number of allowed networkMaxRoads in the graph.
	int networkMaxRoads;
	//Listing of all concrete edges in the graph, edge = <connection, cost>.
	vector<Edge> edgeVector;
public:
	Graph(int vertices) {
		this->skyCity = vertices + 1;
		this->graphVertices = vertices;
		this->networkMaxAirports = 0;
		this->networkMaxRoads = 0;
	}
	~Graph() {}

	//Getters.
	int getSkyCity() { return skyCity; }
	int getGraphVertices() { return graphVertices; }
	int getMaxAirports() { return networkMaxAirports; }
	int getMaxRoads() { return networkMaxRoads; }

	//Setters.
	void setMaxAirports(int i) { networkMaxAirports = i; }
	void setMaxRoads(int i) { networkMaxRoads = i; }

	//Checks if the vertex has connections.
	bool vertexCheckConnected(int city) {
		for (vector<Edge>::const_iterator it = edgeVector.begin(); it != edgeVector.end(); it++) {
			Connection connection = (*it).first; //Gets the connection of the edge.
			int cityA = connection.first; //Gets one city.
			int cityB = connection.second; //Gets the other city.
			if (cityA == city || cityB == city) { //If any of the cities match the given one.
				return true; //Returns true if it finds the given city as connections.
			}
		}
		return false; //Returns false if the given city as no connections.
	}

	//Creates a new edge, <connection, cost>.
	Edge newEdge(int a, int b, int c) {
		Connection con = make_pair(a, b);
		Edge e = make_pair(con, c);
		return e;
	}
	//Adds the new adge to the graph.
	void addEdge(Edge e) { edgeVector.push_back(e); }
	//Prints to standard output the information of a given edge.
	void printEdge(Edge e) {
		Connection connection = e.first;
		int connectionCost = e.second;
		int cityA = connection.first;
		int cityB = connection.second;
		cout << "city a: " << cityA << ", city b: " << cityB << ", cost: " << connectionCost << endl;
	}
	//Prints to standart output the information of all edges in this graph.
	void printEdgeList() {
		cout << endl << "graph.printEdgeList()" << endl;
		for (vector<Edge>::const_iterator ci = edgeVector.begin(); ci != edgeVector.end(); ci++) {
			printEdge(*ci);
		}
	}

	int kruskalMST(); //Calculates the MST for this graph, generating the desired network.
	void answerFormat(); //Checks the MST for the required values;
};

//Calculates the MST for this graph, generating the desired network.
int Graph::kruskalMST() {
	DisjointSets ds(getGraphVertices());
	//Sorts the edges nondecrescent by weight, prioritazing roads.
	sort(edgeVector.begin(), edgeVector.end(), edgeWeightComparator);
	printEdgeList(); //Debug.
	int mst_wt = 0;
	for (vector<Edge>::const_iterator it = edgeVector.begin(); it != edgeVector.end(); it++) {
		int u = (*it).first.first;
		int v = (*it).first.second;
		int set_u = ds.find(u);
		int set_v = ds.find(v);
		if (set_u != set_v) {
			cout << u << " - " << v << endl;
			mst_wt += (*it).second;
			ds.merge(set_u, set_v);
		}
	}
	networkCost = mst_wt;
	answerFormat();
	return mst_wt;
}
//Checks the MST for the required values;
void Graph::answerFormat() {
	//TODO - @RafaelRibeiro
	return;
}


/*------------------------------------------------------------------------------

			CODE EXECUTION

------------------------------------------------------------------------------*/
//Application that calculates the MST.
int main() {
	/**
	* Input variables.
	* int:aux variable is used to fetch the numbers such as number of cities, etc.
	* int:a, int:b are both graphVertices, representing two cities.
	* int:c represents the cost of building an airport on city a or a road between a and b.
	*/
	int aux, a, b, c;
	int i;
	Edge e; //Edge (auxiliar).
	
	scanf("%d", &aux); //Gets the number of vertices (graphVertices) to connect.
	Graph graph(aux); //Instanciate new graph with default constructor.
	skyCity = aux + 1; //Sets the value of the global variable.

	scanf("%d", &aux); //Gets the max number of airports (networkMaxAirports).
	graph.setMaxAirports(aux); //Sets the maximum number of airports.
	for (i = 0; i < aux; i++) { //Creates all the airways.
		scanf("%d %d", &a, &c);
		e = graph.newEdge(a, graph.getSkyCity(), c);
		graph.addEdge(e);
	}

	scanf("%d", &aux); //Gets the max number of roads (networkMaxRoads).
	graph.setMaxRoads(aux); //Sets the maximum number of roads.
	for (i = 0; i < aux; i++) { //Creates all the roads.
		scanf("%d %d %d", &a, &b, &c);
		if (a != b) { //If the vertices aren't the same - avoids self loops.
			e = graph.newEdge(a, b, c);
			graph.addEdge(e);
		}
	}

	//If the number os total connections isn't enough to connect the graph.
	if (graph.getMaxRoads() + graph.getMaxAirports() < graph.getGraphVertices() - 1) {
		outputInsuficient(); //Prints the output.
		return 0; //Quits.
	}
	//Checks if all the vertices have at least one connection.
	for (i = 1; i <= graph.getGraphVertices(); i++) {
		if(!graph.vertexCheckConnected(i)) { //If the city has no connections.
			outputInsuficient(); //Prints the output.
			return 0; //Quits.
		}
	}

	graph.printEdgeList(); //Debug.
	graph.kruskalMST(); //Runs Kruskal's algorithm to find the MST.
	outputMST(networkCost, networkAirports, networkRoads); //Prints the answer.
	return 0;
}
