#ifndef GRAPH_OPERATIONS_HPP
#define GRAPH_OPERATIONS_HPP

#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include "graph.h"

namespace klimova {
  using GraphType = Graph< std::string >;
  using GraphManager = std::unordered_map< std::string, GraphType >;
  void createGraph(GraphManager& graphs, std::istream& in, std::ostream& out);
  void addVertex(GraphManager& graphs, std::istream& in, std::ostream& out);
  void addEdge(GraphManager& graphs, std::istream& in, std::ostream& out);
  void printGraph(const GraphManager& graphs, std::istream& in, std::ostream& out);
}

#endif
