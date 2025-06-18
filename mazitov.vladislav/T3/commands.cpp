#include "commands.hpp"
#include <iomanip>
#include "area_commands.hpp"
#include "../common/format_guard.hpp"
#include "max_commands.hpp"
#include "min_commands.hpp"
#include "count_commands.hpp"
#include "intersections_command.hpp"

void mazitov::areaCommand(std::istream &in, std::ostream &out, const std::vector< Polygon > &polygons)
{
  FormatGuard g(out);
  out << std::fixed << std::setprecision(1) << calcAreas(polygons, in);
}

void mazitov::maxCommand(std::istream &in, std::ostream &out, const std::vector< Polygon > &polygons)
{
  printMax(polygons, in, out);
}

void mazitov::minCommand(std::istream &in, std::ostream &out, const std::vector< Polygon > &polygons)
{
  printMin(polygons, in, out);
}

void mazitov::countCommand(std::istream &in, std::ostream &out, const std::vector< Polygon > &polygons)
{
  out << getCount(polygons, in);
}

void mazitov::intersectsCommand(std::istream &in, std::ostream &out, const std::vector< Polygon > &polygons)
{
  Polygon ref;
  in >> ref;
  if (in)
  {
    out << getIntersections(polygons, ref);
  }
  else
  {
    throw std::invalid_argument("<INVALID COMMAND>");
  }
}
