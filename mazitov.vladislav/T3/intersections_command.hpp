#ifndef INTERSECTIONS_COMMAND_HPP
#define INTERSECTIONS_COMMAND_HPP

#include <cstddef>
#include <vector>
#include "polygon.hpp"

namespace mazitov
{
  std::size_t getIntersections(const std::vector< Polygon > &, const Polygon &);
}

#endif
