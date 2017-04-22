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
#include <vector>
#include <utility>

/*------------------------------------------------------------------------------
//	STRUCTS
------------------------------------------------------------------------------*/

/* Connection represents a road or airway from city int:a to city int:b */
typedef pair<int, int> connection;
/* Edge is a pair whose elements are a connection and the respective int:cost */
typedef pair<connection, int> edge;
/* Network is a */

/*------------------------------------------------------------------------------
//	CONSTANTS
------------------------------------------------------------------------------*/

using namespace std;

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
    int _skyCity = -1;
    /* Number of vertices in the graph, that is to be turned into an MST */
    int _cities = 0;
    /* Total number of allowed airports in the graph */
    int _airports = 0;
    /* Total number of allowed roads in the graph */
    int _roads = 0;
    /* Listing of all concrete edges in the graph, edge = <connection, cost> */
    list<edge> _edgeList;

  public:
    Graph();
    ~Graph();

    int getSky() {
      return _skyCity;
    }

    int getCities() {
      return _cities;
    }

    int getAiports() {
      return _airports;
    }

    int getRoads() {
      return _roads;
    }

    void setCities(int c) {
      _cities = c;
    }

    void setAirports(int a) {
      _airports = a;
    }

    void setRoads(int r) {
      _roads = r;
    }

    void addEdge(edge e) {
      _edgeList.push_back(e);
    }

    edge newEdge(int a, int b, int c) {
      return {{a, b}, c};
    }

    /* Calculates the MST for this graph, generating the desired network */
    void kruskalMST();


};

Graph::kruskalMST() {
  // TODO
}

/* Application that calculates the MST */
int main() {
  /* int iterator */
  int i;
  /* edge auxiliar */
  edge e;
  /* Instanciate new graph */
  Graph graph();

  /* Reading inputs from standart input, given by user */

  /* Gets amount of cities to connect */
  scanf(%d, &mc);
  graph.setCities(mc);

  /* Gets maximum amount of airports */
  scanf(%d, &ma);
  graph.setAirports(ma);

  /* Creates all airways */
  for (i = 0; i < ma; i++) {
    scanf("%d %d", &a, &c);
    e = newEdge(a, graph.getSky(), c);
    graph.addEdge(e);
  }

  /* Gets maximum amount of roads */
  scanf(%d, &mr);

  /* Creates all roads */
  for (i = 0; i < mr; i++) {
    scanf("%d %d %d", &a, &b, &c);
    e = newEdge(a, b, c);
    graph.addEdge(e);
  }

}
