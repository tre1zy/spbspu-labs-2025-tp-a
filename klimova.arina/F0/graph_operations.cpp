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

void klimova::removeVertex(GraphManager& graphs, std::istream& in, std::ostream& out)
{
  std::string graphName, vertex;
  in >> graphName >> vertex;
  auto it = graphs.find(graphName);
  if (it == graphs.end()) {
    out << "<INVALID COMMAND>\n";
    return;
  }
  try {
    it->second.removeVertex(vertex);
    out << "vertex removed\n";
  }
  catch (...) {
    out << "<INVALID COMMAND>\n";
  }
}

void klimova::removeEdge(GraphManager& graphs, std::istream& in, std::ostream& out)
{
  std::string graphName, v1, v2;
  in >> graphName >> v1 >> v2;
  auto it = graphs.find(graphName);
  if (it == graphs.end()) {
    out << "<INVALID COMMAND>\n";
    return;
  }
  try {
    it->second.removeEdge(v1, v2);
    out << "edge between " << v1 << " and " << v2 << " removed\n";
  }
  catch (...) {
    out << "<INVALID COMMAND>\n";
  }
}

void klimova::clearGraph(GraphManager& graphs, std::istream& in, std::ostream& out)
{
  std::string name;
  in >> name;
  auto it = graphs.find(name);
  if (it == graphs.end()) {
    out << "<INVALID COMMAND>\n";
    return;
  }
  it->second.clear();
  out << "This graph cleared\n";
}

void klimova::checkConnectivity(const GraphManager& graphs, std::istream& in, std::ostream& out)
{
  std::string name;
  in >> name;
  auto it = graphs.find(name);
  if (it == graphs.end()) {
    out << "<INVALID COMMAND>\n";
    return;
  }
  if (it->second.checkConnectivity()) {
    out << "This graph is connected\n";
  } else {
    out << "This graph isn't connected\n";
  }
}

void klimova::countVertices(const GraphManager& graphs, std::istream& in, std::ostream& out)
{
  std::string name;
  in >> name;
  auto it = graphs.find(name);
  if (it == graphs.end()) {
    out << "<INVALID COMMAND>\n";
    return;
  }
  out << "Count of vertices in this graph: " << it->second.countVertices() << "\n";
}

void klimova::countEdges(const GraphManager& graphs, std::istream& in, std::ostream& out)
{
  std::string name;
  in >> name;
  auto it = graphs.find(name);
  if (it == graphs.end()) {
    out << "<INVALID COMMAND>\n";
    return;
  }
  out << "Count of edges in this graph: " << it->second.countEdges() << "\n";
}

void klimova::findNeighbors(const GraphManager& graphs, std::istream& in, std::ostream& out)
{
  std::string name, vertex;
  in >> name >> vertex;
  auto it = graphs.find(name);
  if (it == graphs.end()) {
    out << "<INVALID COMMAND>\n";
    return;
  }
  try {
    out << "neighbors for this vertex: ";
    it->second.findNeighbors(vertex);
  }
  catch (...) {
    out << "<INVALID COMMAND>\n";
  }
}

void klimova::degreeOfVertex(const GraphManager& graphs, std::istream& in, std::ostream& out)
{
  std::string name, vertex;
  in >> name >> vertex;
  auto it = graphs.find(name);
  if (it == graphs.end()) {
    out << "<INVALID COMMAND>\n";
    return;
  }
  try {
    int degree = it->second.degreeOfVertex(vertex);
    if (degree != -1) {
      out << "degree of this vertex: " << degree << "\n";
    }
  }
  catch (...) {
    out << "<INVALID COMMAND>\n";
  }
}

void klimova::findShortestPath(const GraphManager& graphs, std::istream& in, std::ostream& out)
{
  std::string name, start, end;
  in >> name >> start >> end;
  auto it = graphs.find(name);
  if (it == graphs.end()) {
    out << "<INVALID COMMAND>\n";
    return;
  }
  try {
    it->second.findShortestPath(start, end);
  }
  catch (...) {
    out << "<NO PATH FOUND>\n";
  }
}

void klimova::findLongestPath(const GraphManager& graphs, std::istream& in, std::ostream& out)
{
  std::string name, start, end;
  in >> name >> start >> end;
  auto it = graphs.find(name);
  if (it == graphs.end()) {
    out << "<INVALID COMMAND>\n";
    return;
  }
  try {
    it->second.findLongestPath(start, end);
  }
  catch (...) {
    out << "<NO PATH FOUND>\n";
  }
}

void klimova::showHelp(std::ostream& out) {
    out << "Graph Program - Available Commands:\n\n";
    out << "   [graph] <filename>          - Load graph from file\n";
    out << "   --check <filename>          - Check graph file correctness\n\n";

    out << "   --shortest-path <start> <end> - Find shortest path between vertices\n";
    out << "   --longest-path <start> <end>  - Find longest path between vertices\n";
    out << "   --check-connectivity         - Check if graph is connected\n\n";

    out << "   --add-vertex <graph> <vertex> - Add vertex to graph\n";
    out << "   --add-edge <graph> <v1> <v2>  - Add edge between vertices\n";
    out << "   --remove-vertex <graph> <vertex> - Remove vertex from graph\n";
    out << "   --remove-edge <graph> <v1> <v2> - Remove edge between vertices\n";
    out << "   --clear <graph>              - Clear graph\n\n";

    out << "   --print <graph>             - Print graph structure\n";
    out << "   --count-vertices <graph>    - Count vertices in graph\n";
    out << "   --count-edges <graph>       - Count edges in graph\n";
    out << "   --find-neighbors <graph> <vertex> - Find neighbors of vertex\n";
    out << "   --degree <graph> <vertex>   - Get degree of vertex\n\n";

    out << "   --help                     - Show this help message\n\n";

    out << "File Format Example:\n";
    out << "-------------------\n";
    out << "MyGraph\n";
    out << "Vertices:\n";
    out << "A\n";
    out << "B\n";
    out << "C\n";
    out << "Edges:\n";
    out << "A -- B\n";
    out << "B -- C\n";
}
