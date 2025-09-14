#include "commandHandling.hpp"
#include <algorithm>
#include <functional>
#include <numeric>
#include "exceptions.hpp"
#include "polygon.hpp"

using VecPolygons = std::vector< holodilov::Polygon >;

bool compareAreaWithTarget(const holodilov::Polygon& polygon, double targetArea)
{
  std::cout << "Target area = " << targetArea << '\n';
  std::cout << "polygon area = " << polygon.getArea() << "\n";
  return polygon.getArea() < targetArea;
}

VecPolygons accumulateEcho(VecPolygons vecPolygons, const holodilov::Polygon& polygon, const holodilov::Polygon& targetPolygon)
{
  vecPolygons.push_back(polygon);
  if (polygon == targetPolygon)
  {
    vecPolygons.push_back(polygon);
  }
  return vecPolygons;
}

void holodilov::commands::echo(std::istream& is, std::ostream& os, VecPolygons& vecPolygons)
{
  Polygon targetPolygon;
  is >> targetPolygon;
  if (!is || is.peek() != '\n')
  {
    throw InvalidCommandException();
  }

  const size_t amountDuplicates = std::count(vecPolygons.begin(), vecPolygons.end(), targetPolygon);

  auto accumulateBound = std::bind(accumulateEcho, std::placeholders::_1, std::placeholders::_2, std::cref(targetPolygon));
  vecPolygons = std::accumulate(vecPolygons.begin(), vecPolygons.end(), VecPolygons(0), accumulateBound);
  os << amountDuplicates << '\n';
}

void holodilov::commands::lessArea(std::istream& is, std::ostream& os, const VecPolygons& vecPolygons)
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
