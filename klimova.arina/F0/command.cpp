#include "command.hpp"
#include "graph_operations.hpp"

void klimova::loadGraphFromFile(klimova::GraphManager& graphs, const std::string& filename)
{
    (void)graphs;
    (void)filename;
    throw std::runtime_error(" ");
}

bool klimova::checkGraphFile(const std::string& filename)
{
    (void)filename;
    throw std::runtime_error("Function checkGraphFile not implemented yet");
    return false;
}

void klimova::showHelp(std::ostream& out)
{
  out << "Graph Program - Available Commands:\n\n";
  out << " - creategraph <graph> - Create graph with name <graph>";
  out << " - shortestpath <start> <end> - Find shortest path between vertices\n";
  out << " - longestpath <start> <end> - Find longest path between vertices\n";
  out << " - checkconnectivity <graph> - Check if graph is connected\n\n";

  out << " - addvertex <graph> <vertex> - Add vertex to graph\n";
  out << " - addedge <graph> <v1> <v2>  - Add edge between vertices\n";
  out << " - removevertex <graph> <vertex> - Remove vertex from graph\n";
  out << " - removeedge <graph> <v1> <v2> - Remove edge between vertices\n";
  out << " - clear <graph>  - Clear graph\n\n";

  out << " - print <graph> - Print graph structure\n";
  out << " - countvertices <graph> - Count vertices in graph\n";
  out << " - countedges <graph> - Count edges in graph\n";
  out << " - findneighbors <graph> <vertex> - Find neighbors of vertex\n";
  out << " - degree <graph> <vertex> - Get degree of vertex\n\n";

  out << " - help - Show this help message\n\n";
}

klimova::CommandHandler klimova::createCommandHandler(klimova::GraphManager& graphs)
{
  CommandHandler cmdMap;
  cmdMap["creategraph"] = std::bind(createGraph, std::ref(graphs), _1, _2);
  cmdMap["addvertex"] = std::bind(addVertex, std::ref(graphs), _1, _2);
  cmdMap["addedge"] = std::bind(addEdge, std::ref(graphs), _1, _2);
  cmdMap["printgraph"] = std::bind(printGraph, std::cref(graphs), _1, _2);
  cmdMap["removevertex"] = std::bind(removeVertex, std::ref(graphs), _1, _2);
  cmdMap["removeedge"] = std::bind(removeEdge, std::ref(graphs), _1, _2);
  cmdMap["clear"] = std::bind(clearGraph, std::ref(graphs), _1, _2);
  cmdMap["checkconnectivity"] = std::bind(checkConnectivity, std::cref(graphs), _1, _2);
  cmdMap["countvertices"] = std::bind(countVertices, std::cref(graphs), _1, _2);
  cmdMap["countedges"] = std::bind(countEdges, std::cref(graphs), _1, _2);
  cmdMap["findneighbors"] = std::bind(findNeighbors, std::cref(graphs), _1, _2);
  cmdMap["degreeofvertex"] = std::bind(degreeOfVertex, std::cref(graphs), _1, _2);
  cmdMap["findshortestpath"] = std::bind(findShortestPath, std::cref(graphs), _1, _2);
  cmdMap["findlongestpath"] = std::bind(findLongestPath, std::cref(graphs), _1, _2);
  cmdMap["help"] = std::bind(showHelp, _2);
  return cmdMap;
}
