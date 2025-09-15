#include "subcommands.hpp"
#include <algorithm>
#include <functional>
#include <iomanip>
#include "ScopeGuard.hpp"
#include "polygon.hpp"

using VecPolygons = std::vector< holodilov::Polygon >;

bool checkPolygonVertexesAmount(const holodilov::Polygon& polygon, const size_t amountVertexes)
{
  return polygon.points.size() == amountVertexes;
}

bool checkPolygonVertexesEven(const holodilov::Polygon& polygon)
{
  return polygon.points.size() % 2 == 0;
}

bool checkPolygonVertexesOdd(const holodilov::Polygon& polygon)
{
  return polygon.points.size() % 2 == 1;
}

bool comparePolygonsByVertexes(const holodilov::Polygon& lhs, const holodilov::Polygon& rhs)
{
  return lhs.points.size() < rhs.points.size();
}

bool comparePolygonsByArea(const holodilov::Polygon& lhs, const holodilov::Polygon& rhs)
{
  return lhs.getArea() < rhs.getArea();
}

void holodilov::subcommands::countEven(std::istream& is, std::ostream& os, const VecPolygons& vecPolygons) {
  size_t count = std::count_if(vecPolygons.begin(), vecPolygons.end(), checkPolygonVertexesEven);
  os << count << '\n';
}

void holodilov::subcommands::countOdd(std::istream& is, std::ostream& os, const std::vector<Polygon>& vecPolygons) {
  size_t count = std::count_if(vecPolygons.begin(), vecPolygons.end(), checkPolygonVertexesOdd);
  os << count << '\n';
}

void holodilov::subcommands::countAmount(std::istream& is, std::ostream& os, const std::vector<Polygon>& vecPolygons, size_t amountVertexes) {
  auto amountVertexesChecker = std::bind(checkPolygonVertexesAmount, std::placeholders::_1, amountVertexes);
  size_t count = std::count_if(vecPolygons.begin(), vecPolygons.end(), amountVertexesChecker);
  os << count << '\n';
}

void holodilov::subcommands::maxArea(std::istream& is, std::ostream& os, const VecPolygons& vecPolygons)
{
  ScopeGuard scopeGuard(os);

  Polygon polygon = *(std::max_element(vecPolygons.begin(), vecPolygons.end(), comparePolygonsByArea));
  std::cout << std::fixed << std::setprecision(1);
  std::cout << polygon.getArea() << '\n';
}

void holodilov::subcommands::maxVertexes(std::istream& is, std::ostream& os, const VecPolygons& vecPolygons)
{
  ScopeGuard scopeGuard(os);

  Polygon polygon = *(std::max_element(vecPolygons.begin(), vecPolygons.end(), comparePolygonsByVertexes));
  std::cout << std::fixed << std::setprecision(1);
  std::cout << polygon.points.size() << '\n';
}