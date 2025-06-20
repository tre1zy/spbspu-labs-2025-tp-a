#ifndef GRAPH_H
#define GRAPH_H

#include <unordered_map>
#include <vector>
#include <string>
#include <ostream>
#include <iostream>
#include <queue>

template< typename T >
class Graph {
public:
  Graph() = default;
  void addVertex(const T& vertex);
  void addEdge(const T& src, const T& dest);
  void removeVertex(const T& vertex);
  void removeEdge(const T& src, const T& dest);
  void printGraph() const;
  bool checkConnectivity() const;
  size_t countVertices() const;
  size_t countEdges() const;
  void findNeighbors(const T& vertex) const;
  int degreeOfVertex(const T& vertex) const;
  void clear();
  void findLongestPath(const T& startVertex, const T& endVertex);
  void findShortestPath(const T& startVertex, const T& endVertex);

private:
  std::vector< T > vertices;
  std::unordered_map< T, size_t > vertexMap;
  std::vector< std::vector< bool > > adjMatrix;
  bool dfsUtil(size_t v, std::vector< bool >& visited) const;
  bool findLongestPathUtil(size_t u, size_t endIdx,
        std::vector< bool >& visited,
        std::vector< T >& path,
        std::vector< T >& longestPath) const;
  size_t getVertexIndex(const T& vertex) const;
};

template < typename T >
void Graph< T >::addVertex(const T& vertex)
{
  if (vertexMap.find(vertex) == vertexMap.end()) {
    vertexMap[vertex] = vertices.size();
    vertices.push_back(vertex);
    for (auto& row : adjMatrix) {
      row.resize(vertices.size(), false);
    }
    adjMatrix.resize(vertices.size(), std::vector< bool >(vertices.size(), false));
  }
}

template < typename T >
size_t Graph< T >::getVertexIndex(const T& vertex) const
{
  auto it = vertexMap.find(vertex);
  if (it == vertexMap.end()) {
    throw std::out_of_range("Vertex not found");
  }
  return it->second;
}

template < typename T >
void Graph< T >::addEdge(const T& src, const T& dest)
{
  size_t srcIdx = getVertexIndex(src);
  size_t destIdx = getVertexIndex(dest);
  adjMatrix[srcIdx][destIdx] = true;
  adjMatrix[destIdx][srcIdx] = true;
}

template < typename T >
void Graph< T >::removeEdge(const T& src, const T& dest)
{
  size_t srcIdx = getVertexIndex(src);
  size_t destIdx = getVertexIndex(dest);
  adjMatrix[srcIdx][destIdx] = false;
  adjMatrix[destIdx][srcIdx] = false;
}

template < typename T >
void Graph< T >::removeVertex(const T& vertex)
{
  size_t idx = getVertexIndex(vertex);
  vertices.erase(vertices.begin() + idx);
  vertexMap.erase(vertex);
  for (size_t i = idx; i < vertices.size(); ++i) {
    vertexMap[vertices[i]] = i;
  }
  adjMatrix.erase(adjMatrix.begin() + idx);
  for (auto& row : adjMatrix) {
    row.erase(row.begin() + idx);
  }
}

template < typename T >
void Graph< T >::printGraph() const
{
  std::cout << "Vertices:\n";
  for (const auto& v : vertices) {
    std::cout << v << "\n";
  }
  std::cout << "Edges:\n";
  for (size_t i = 0; i < vertices.size(); ++i) {
    for (size_t j = i + 1; j < vertices.size(); ++j) {
      if (adjMatrix[i][j]) {
        std::cout << vertices[i] << " -- " << vertices[j] << "\n";
      }
    }
  }
}

template < typename T >
bool Graph< T >::checkConnectivity() const
{
  if (vertices.empty()) {
    return true;
  }
  std::vector< bool > visited(vertices.size(), false);
  dfsUtil(0, visited);
  for (bool v : visited) {
        if (!v) {
            return false;
        }
  }
  return true;
}

template < typename T >
bool Graph< T >::dfsUtil(size_t v, std::vector< bool >& visited) const
{
  visited[v] = true;

  for (size_t u = 0; u < vertices.size(); ++u) {
    if (adjMatrix[v][u] && !visited[u]) {
      dfsUtil(u, visited);
    }
  }
  return true;
}

template < typename T >
size_t Graph< T >::countVertices() const
{
  return vertices.size();
}

template < typename T >
size_t Graph< T >::countEdges() const
{
  size_t count = 0;
  for (size_t i = 0; i < vertices.size(); ++i) {
    for (size_t j = i + 1; j < vertices.size(); ++j) {
      if (adjMatrix[i][j]) {
        ++count;
      }
    }
  }
  return count;
}

template < typename T >
void Graph< T >::findNeighbors(const T& vertex) const
{
  try {
    size_t idx = getVertexIndex(vertex);
    std::cout << "Neighbors of " << vertex << ": ";
    for (size_t v = 0; v < vertices.size(); ++v) {
      if (adjMatrix[idx][v]) {
        std::cout << vertices[v] << " ";
      }
    }
    std::cout << std::endl;
  } catch (const std::out_of_range&) {
    std::cout << "Vertex not found." << std::endl;
  }
}

template < typename T >
int Graph< T >::degreeOfVertex(const T& vertex) const
{
  try {
    size_t idx = getVertexIndex(vertex);
    int degree = 0;
    for (size_t v = 0; v < vertices.size(); ++v) {
      if (adjMatrix[idx][v]) {
        ++degree;
      }
    }
    return degree;
  } catch (const std::out_of_range&) {
    std::cerr << "Vertex not found." << std::endl;
    return -1;
  }
}

template < typename T >
void Graph< T >::findShortestPath(const T& startVertex, const T& endVertex)
{
  try {
    size_t startIdx = getVertexIndex(startVertex);
    size_t endIdx = getVertexIndex(endVertex);

    std::vector< bool > visited(vertices.size(), false);
    std::vector< int > parent(vertices.size(), -1);
    std::queue< size_t > queue;

    visited[startIdx] = true;
    queue.push(startIdx);

    bool found = false;

    while (!queue.empty()) {
      size_t curr = queue.front();
      queue.pop();

      if (curr == endIdx) {
        found = true;
        break;
      }

      for (size_t v = 0; v < vertices.size(); ++v) {
        if (adjMatrix[curr][v] && !visited[v]) {
          visited[v] = true;
          parent[v] = curr;
          queue.push(v);
        }
      }
    }

    if (!found) {
      std::cout << "Path from " << startVertex << " to " << endVertex << " not found\n";
      return;
    }

    std::vector< T > path;
    for (int v = endIdx; v != -1; v = parent[v]) {
      path.push_back(vertices[v]);
    }

    std::cout << "Shortest path from " << startVertex << " to " << endVertex << ": ";
    for (auto it = path.rbegin(); it != path.rend(); ++it) {
      std::cout << *it << " ";
    }
    std::cout << std::endl;
  } catch (const std::out_of_range&) {
    std::cout << "One of the vertices not found." << std::endl;
  }
}

template < typename T >
void Graph< T >::clear()
{
  vertices.clear();
  vertexMap.clear();
  adjMatrix.clear();
}

template < typename T >
bool Graph< T >::findLongestPathUtil(size_t u, size_t endIdx,
      std::vector< bool >& visited,
      std::vector< T >& path,
      std::vector< T >& longestPath) const
{
  visited[u] = true;
  path.push_back(vertices[u]);

  if (u == endIdx) {
    if (path.size() > longestPath.size()) {
      longestPath = path;
    }
  } else {
    for (size_t v = 0; v < vertices.size(); ++v) {
      if (adjMatrix[u][v] && !visited[v]) {
        findLongestPathUtil(v, endIdx, visited, path, longestPath);
      }
    }
  }

  path.pop_back();
  visited[u] = false;
  return !longestPath.empty();
}

template < typename T >
void Graph< T >::findLongestPath(const T& startVertex, const T& endVertex)
{
  try {
    size_t startIdx = getVertexIndex(startVertex);
    size_t endIdx = getVertexIndex(endVertex);

    std::vector< bool > visited(vertices.size(), false);
    std::vector< T > path, longestPath;

    if (findLongestPathUtil(startIdx, endIdx, visited, path, longestPath)) {
      std::cout << "Longest path from " << startVertex << " to " << endVertex << ": ";
      for (const auto& vertex : longestPath) {
        std::cout << vertex << " ";
      }
      std::cout << std::endl;
    } else {
      std::cout << "Path from " << startVertex << " to " << endVertex << " not found\n";
    }
  } catch (const std::out_of_range&) {
    std::cout << "One of the vertices not found." << std::endl;
  }
}

#endif



