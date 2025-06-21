#include <iostream>
#include <unordered_map>
#include <functional>
#include <limits>
#include "graph.hpp"
#include "graph_operations.hpp"

int main() {
  using namespace std::placeholders;
  klimova::GraphManager graphs;

  auto createGraphCmd = std::bind(klimova::createGraph, std::ref(graphs), _1, _2);
  auto addVertexCmd = std::bind(klimova::addVertex, std::ref(graphs), _1, _2);
  auto addEdgeCmd = std::bind(klimova::addEdge, std::ref(graphs), _1, _2);
  auto printGraphCmd = std::bind(klimova::printGraph, std::cref(graphs), _1, _2);
  auto removeVertexCmd = std::bind(klimova::removeVertex, std::ref(graphs), _1, _2);
  auto removeEdgeCmd = std::bind(klimova::removeEdge, std::ref(graphs), _1, _2);
  auto clearGraphCmd = std::bind(klimova::clearGraph, std::ref(graphs), _1, _2);
  auto checkConnectivityCmd = std::bind(klimova::checkConnectivity, std::cref(graphs), _1, _2);
  auto countVerticesCmd = std::bind(klimova::countVertices, std::cref(graphs), _1, _2);
  auto countEdgesCmd = std::bind(klimova::countEdges, std::cref(graphs), _1, _2);
  auto findNeighborsCmd = std::bind(klimova::findNeighbors, std::cref(graphs), _1, _2);
  auto degreeOfVertexCmd = std::bind(klimova::degreeOfVertex, std::cref(graphs), _1, _2);
  auto findShortestPathCmd = std::bind(klimova::findShortestPath, std::cref(graphs), _1, _2);
  auto findLongestPathCmd = std::bind(klimova::findLongestPath, std::cref(graphs), _1, _2);
  std::unordered_map< std::string, std::function< void(std::istream&, std::ostream&) > > cmdMap = {
        {"creategraph", createGraphCmd},
        {"addvertex", addVertexCmd},
        {"addedge", addEdgeCmd},
        {"printgraph", printGraphCmd},
        {"removevertex", removeVertexCmd},
        {"removeedge", removeEdgeCmd},
        {"clear", clearGraphCmd},
        {"checkconnectivity", checkConnectivityCmd},
        {"countvertices", countVerticesCmd},
        {"countedges", countEdgesCmd},
        {"findneighbors", findNeighborsCmd},
        {"degreeofvertex", degreeOfVertexCmd},
        {"findshortestpath", findShortestPathCmd},
        {"findlongestpath", findLongestPathCmd}
  };

  std::string command;
  while (std::cin >> command) {
    try {
      auto it = cmdMap.find(command);
      if (it != cmdMap.end()) {
        it->second(std::cin, std::cout);
      }
      else {
        std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
        std::cout << "<INVALID COMMAND>\n";
      }
    }
    catch (const std::exception&) {
      std::cin.clear();
      std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
      std::cout << "<INVALID COMMAND>\n";
    }
  }

  return 0;
}
