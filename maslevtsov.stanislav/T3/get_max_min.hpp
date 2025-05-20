#ifndef GET_MAX_MIN_HPP
#define GET_MAX_MIN_HPP

#include "shapes.hpp"

namespace maslevtsov {
  void get_max(const std::vector< Polygon >& polygons, std::istream& in, std::ostream& out);
  void get_min(const std::vector< Polygon >& polygons, std::istream& in, std::ostream& out);
}

#endif
