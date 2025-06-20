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

  std::unordered_map< std::string, std::function< void(std::istream&, std::ostream&) > > cmdMap = {
        {"creategraph", createGraphCmd},
        {"addvertex", addVertexCmd},
        {"addedge", addEdgeCmd},
        {"printgraph", printGraphCmd}
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
