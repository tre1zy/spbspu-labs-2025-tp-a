#include "graph_operations.hpp"

void klimova::createGraph(GraphManager& graphs, std::istream& in, std::ostream& out)
{
  std::string name;
  in >> name;
  if (graphs.find(name) != graphs.end()) {
    out << "<INVALID COMMAND>\n";
    return;
  }
  graphs.emplace(name, GraphType{});
  std::cout << "graph created\n";
}

void klimova::addVertex(GraphManager& graphs, std::istream& in, std::ostream& out)
{
  std::string graphName, vertex;
  in >> graphName >> vertex;
  auto it = graphs.find(graphName);
  if (it == graphs.end()) {
    out << "<INVALID COMMAND>\n";
    return;
  }
  try {
    it->second.addVertex(vertex);
    std::cout << "vertex added\n";
  }
  catch (...) {
    out << "<INVALID COMMAND>\n";
  }
}

void klimova::addEdge(GraphManager& graphs, std::istream& in, std::ostream& out)
{
  std::string graphName, v1, v2;
  in >> graphName >> v1 >> v2;
  auto it = graphs.find(graphName);
  if (it == graphs.end()) {
    out << "<INVALID COMMAND>\n";
    return;
  }
  try {
    it->second.addEdge(v1, v2);
    std::cout << "edge added\n";
  }
  catch (...) {
    out << "<INVALID COMMAND>\n";
  }
}

void klimova::printGraph(const GraphManager& graphs, std::istream& in, std::ostream& out)
{
  std::string name;
  in >> name;
  auto it = graphs.find(name);
  if (it == graphs.end()) {
    out << "<INVALID COMMAND>\n";
    return;
  }
  out << "Graph: " << name << "\n";
  it->second.printGraph();
}
