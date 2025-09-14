#include "commandHandling.hpp"
#include <algorithm>
#include <functional>
#include "exceptions.hpp"
#include "polygon.hpp"

bool holodilov::commands::compareAreaWithTarget(const Polygon &polygon, const double targetArea) {
  std::cout << "Target area = " << targetArea << '\n';
  std::cout << "polygon area = " << polygon.getArea() << "\n";
  return polygon.getArea() < targetArea;
}


void holodilov::commands::echo(std::istream& is, std::ostream& os, std::vector< Polygon >& vecPolygons)
{
  Polygon polygon;
  is >> polygon;
  if (!is || is.peek() != '\n')
  {
    throw InvalidCommandException();
  }

  // std::vector< Polygon > vecResult;
  // const size_t amountDuplicates = std::count(vecPolygons.begin(), vecPolygons.end(), polygon);
  // vecResult.reserve(vecPolygons.size() + amountDuplicates);
  //
  // std::transform(vecPolygons.begin(), vecPolygons.end(), vecResult.begin(), [&](const Polygon& polygon) {
  //   if (polygon == polygon) {
  //     vecResult.push_back(polygon);
  //   }
  //   return polygon;
  // });
  //
  // vecPolygons.swap(vecResult);
  // os << amountDuplicates << "\n";
}

void holodilov::commands::lessArea(std::istream& is, std::ostream& os, const std::vector< Polygon >& vecPolygons)
{
  Polygon targetPolygon;
  is >> targetPolygon;
  if (!is || is.peek() != '\n')
  {
    throw InvalidCommandException();
  }

  auto areaComparator = std::bind(compareAreaWithTarget, std::placeholders::_1, targetPolygon.getArea());
  size_t result = std::count_if(vecPolygons.begin(), vecPolygons.end(), areaComparator);
  os << result << '\n';
}