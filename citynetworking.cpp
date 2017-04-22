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
//	STRUCTS
------------------------------------------------------------------------------*/

/* Connection represents a road or airway from city int:a to city int:b */
typedef pair<int, int> Connection;
/* Edge is a pair whose elements are a connection and the respective int:cost */
typedef pair<Connection, int> Edge;
/* Network is a */

/*------------------------------------------------------------------------------
//	CONSTANTS
------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
//	GLOBAL VARIABLES
------------------------------------------------------------------------------*/

/*
* Output variables.
* networkCost is the total weight of the MST - "Cost of connecting all cities".
* networkRoads is the int: value of roads used for the calculated network.
* networkAirports is the int: value of airports used for the calculated network.
*/
int networkCost = 0;
int networkRoads = 0;
int networkAirports = 0;

/*
* Input variables.
* mc is the number of cities to connect. (vertices)
* ma is the maximum amount of airports allowed in the network.
* mr is the maximum amount of roads allowed in the network.
* a and b represent cities.
* c is the cost of building an airport on city a or a road between a and b.
*/
int mc, ma, mr, a, b, c;

class Graph {
  private:
    /* Abstract city to which all cities with airports connect */
    int skyCity;
    /* Number of vertices in the graph, that is to be turned into an MST */
    int cities;
    /* Total number of allowed airports in the graph */
    int airports;
    /* Total number of allowed roads in the graph */
    int roads;
    /* Listing of all concrete edges in the graph, edge = <connection, cost> */
    list<Edge> edgeList;

  public:
    Graph() {
        this->skyCity = -1;
        this->cities = 0;
        this->airports = 0;
        this->roads = 0;
    }

    ~Graph() {}

    int getSky() {
      return skyCity;
    }

    int getCities() {
      return cities;
    }

    int getAiports() {
      return airports;
    }

    int getRoads() {
      return roads;
    }

    void setCities(int c) {
      cities = c;
    }

    void setAirports(int a) {
      airports = a;
    }

    void setRoads(int r) {
      roads = r;
    }

    void addEdge(Edge e) {
      edgeList.push_back(e);
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

/* Application that calculates the MST */
int main() {
  /* int iterator */
  int i;
  /* edge auxiliar */
  Edge e;
  /* Instanciate new graph with default constructor */
  Graph graph;

  /* Reading inputs from standart input, given by user */

  /* Gets amount of cities to connect */
  scanf("%d", &mc);
  graph.setCities(mc);

  /* Gets maximum amount of airports */
  scanf("%d", &ma);
  graph.setAirports(ma);

  /* Creates all airways */
  for (i = 0; i < ma; i++) {
    scanf("%d %d", &a, &c);
    e = graph.newEdge(a, graph.getSky(), c);
    graph.addEdge(e);
  }

  /* Gets maximum amount of roads */
  scanf("%d", &mr);
  graph.setRoads(mr);

  /* Creates all roads */
  for (i = 0; i < mr; i++) {
    scanf("%d %d %d", &a, &b, &c);
    e = graph.newEdge(a, b, c);
    graph.addEdge(e);
  }

}
