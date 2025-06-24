#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <unordered_map>
#include <string>
#include <vector>

namespace maslevtsov {
  class Graph
  {
  public:
    Graph() = default;
    Graph(const Graph& gr1, const Graph& gr2);
    Graph(const Graph& gr, const std::vector< unsigned >& vertices);

    size_t get_vertice_count() const;

    void add_vertice(unsigned vertice);
    void add_edge(unsigned vertice1, unsigned vertice2);
    void delete_vertice(unsigned vertice);
    void delete_edge(unsigned vertice1, unsigned vertice2);
    void print_adjacency_list(std::ostream& out) const;

  private:
    using adjacency_list_t = std::unordered_map< unsigned, std::vector< unsigned > >;
    adjacency_list_t adjacency_list_;

    friend std::istream& operator>>(std::istream& in, Graph& gr);
  };

  std::istream& operator>>(std::istream& in, Graph& gr);
}

#endif
