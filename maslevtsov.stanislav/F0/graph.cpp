#include "graph.hpp"
#include <iostream>

namespace {
  bool find_if_contain(const std::vector< unsigned >& vertices, unsigned vertice)
  {
    for (size_t i = 0; i != vertices.size(); ++i) {
      if (vertices[i] == vertice) {
        return true;
      }
    }
    return false;
  }

  void delete_solo_vertice(std::vector< unsigned >& vertices, unsigned vertice)
  {
    for (auto i = vertices.begin(); i != vertices.end();) {
      if (vertice == *i) {
        i = vertices.erase(i);
      } else {
        ++i;
      }
    }
  }

  struct DelimiterIn
  {
    char exp;
  };

  std::istream& operator>>(std::istream& in, DelimiterIn&& rhs)
  {
    std::istream::sentry sentry(in);
    if (!sentry) {
      return in;
    }
    char c = '0';
    in >> c;
    if (in && (c != rhs.exp)) {
      in.setstate(std::ios::failbit);
    }
    return in;
  }
}

void maslevtsov::Graph::add_vertice(unsigned vertice)
{
  if (find_if_contain(solo_vertices_, vertice)) {
    throw std::invalid_argument("vertice already exist");
  }
  solo_vertices_.push_back(vertice);
}

void maslevtsov::Graph::add_edge(unsigned vertice1, unsigned vertice2)
{
  auto it1 = edges_.find(vertice1);
  if (it1 != edges_.end() && find_if_contain(it1->second, vertice2)) {
    throw std::invalid_argument("edge already exist");
  }
  edges_[vertice1].push_back(vertice2);
  edges_[vertice2].push_back(vertice1);
  delete_solo_vertice(solo_vertices_, vertice1);
  delete_solo_vertice(solo_vertices_, vertice2);
}

void maslevtsov::Graph::print_adjacency_list(std::ostream& out) const
{
  if (edges_.empty()) {
    return;
  }
  out << edges_.cbegin()->first << " :";
  for (auto i = edges_.cbegin()->second.cbegin(); i != edges_.cbegin()->second.cend(); ++i) {
    out << ' ' << *i;
  }
  for (auto i = ++edges_.cbegin(); i != edges_.cend(); ++i) {
    out << '\n' << i->first << " :";
    for (auto j = i->second.cbegin(); j != i->second.cend(); ++j) {
      out << ' ' << *j;
    }
  }
}

std::istream& maslevtsov::operator>>(std::istream& in, Graph& gr)
{
  std::istream::sentry sentry(in);
  if (!sentry) {
    return in;
  }
  Graph result;
  size_t vertice_count = 0;
  in >> vertice_count;
  for (size_t i = 0; i != vertice_count; ++i) {
    unsigned vertice = 0, neighbour = 0;
    in >> vertice >> DelimiterIn{':'};
    bool is_neighbour = false;
    while (in.peek() != '\n') {
      in >> neighbour;
      try {
        result.add_edge(vertice, neighbour);
      } catch (const std::invalid_argument&) {
        continue;
      } catch (...) {
        in.setstate(std::ios::failbit);
        return in;
      }
      is_neighbour = true;
    }
    if (!is_neighbour) {
      try {
        result.add_vertice(vertice);
      } catch (const std::invalid_argument&) {
        continue;
      } catch (...) {
        in.setstate(std::ios::failbit);
        return in;
      }
    }
  }
  if (in) {
    try {
      gr = result;
    } catch (...) {
      in.setstate(std::ios::failbit);
    }
  }
  return in;
}
