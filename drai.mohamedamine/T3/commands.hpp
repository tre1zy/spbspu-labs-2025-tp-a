#ifndef AMINE_COMMANDS_HPP
#define AMINE_COMMANDS_HPP

#include <vector>
#include <string>
#include <istream>

namespace amine
{
  struct Point
  {
    int x, y;
  };

  struct Polygon
  {
    std::vector< Point > points;
  };

  bool parse_polygon(const std::string& str, Polygon& poly);

  double compute_area(const Polygon& poly);

  bool polygons_intersect(const Polygon& a, const Polygon& b);

  void process_rmecho(std::vector< Polygon >& polygons, const Polygon& query);

  void process_commands(std::vector< Polygon >& polygons);
}

#endif
