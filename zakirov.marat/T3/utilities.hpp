#ifndef UTILITES_HPP
#define UTILITES_HPP
#include "geometric_shape.hpp"
#include <list>

namespace zakirov
{
  bool odd_polygon_pred(const Polygon & polygon);
  bool even_polygon_pred(const Polygon & polygon);
  bool less_area_pred(double area_base, double area_other);
  bool equal_vertexes_pred(double vertex_base, double vertex_other);
  size_t get_vertex(const Polygon & polygon);
  double count_area(const Polygon & plgn);
  int extract_num_until(const std::string & str, size_t start_pos, char el);
  Point parce_point (const std::string & str);
  template <class P>
  double count_sum_area(const std::list< Polygon > & points, P pred);
}

#endif
