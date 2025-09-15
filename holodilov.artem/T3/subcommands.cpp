#include "subcommands.hpp"
#include <algorithm>
#include <functional>
#include <iomanip>
#include <numeric>

#include "exceptions.hpp"
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

double accumulatePolygonAreaEvenOdd(double sum, const holodilov::Polygon& polygon, bool isOdd)
{
  if (polygon.points.size() % 2 == isOdd)
  {
    sum += polygon.getArea();
  }
  return sum;
}

double accumulatePolygonArea(double sum, const holodilov::Polygon& polygon)
{
  return sum + polygon.getArea();
}

double accumulatePolygonAreaVertexes(double sum, const holodilov::Polygon& polygon, size_t amountVertexes)
{
  if (polygon.points.size() == amountVertexes)
  {
    sum += polygon.getArea();
  }
  return sum;
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

void holodilov::subcommands::minArea(std::istream& is, std::ostream& os, const VecPolygons& vecPolygons)
{
  ScopeGuard scopeGuard(os);

  Polygon polygon = *(std::min_element(vecPolygons.begin(), vecPolygons.end(), comparePolygonsByArea));
  std::cout << std::fixed << std::setprecision(1);
  std::cout << polygon.getArea() << '\n';
}

void holodilov::subcommands::minVertexes(std::istream& is, std::ostream& os, const VecPolygons& vecPolygons)
{
  ScopeGuard scopeGuard(os);

  Polygon polygon = *(std::min_element(vecPolygons.begin(), vecPolygons.end(), comparePolygonsByVertexes));
  std::cout << std::fixed << std::setprecision(1);
  std::cout << polygon.points.size() << '\n';
}

void holodilov::subcommands::areaEven(std::istream& is, std::ostream& os, const std::vector< Polygon >& vecPolygons)
{
  ScopeGuard scopeGuard(os);

  auto areaAccumulator = std::bind(accumulatePolygonAreaEvenOdd, std::placeholders::_1, std::placeholders::_2, false);
  double sumArea = std::accumulate(vecPolygons.begin(), vecPolygons.end(), 0.0, areaAccumulator);
  std::cout << std::fixed << std::setprecision(1);
  std::cout << sumArea << '\n';
}

void holodilov::subcommands::areaOdd(std::istream& is, std::ostream& os, const std::vector< Polygon >& vecPolygons)
{
  ScopeGuard scopeGuard(os);

  auto areaAccumulator = std::bind(accumulatePolygonAreaEvenOdd, std::placeholders::_1, std::placeholders::_2, true);
  double sumArea = std::accumulate(vecPolygons.begin(), vecPolygons.end(), 0.0, areaAccumulator);
  std::cout << std::fixed << std::setprecision(1);
  std::cout << sumArea << '\n';
}

void holodilov::subcommands::areaMean(std::istream& is, std::ostream& os, const std::vector< Polygon >& vecPolygons)
{
  if (vecPolygons.empty())
  {
    throw InvalidCommandException();
  }

  ScopeGuard scopeGuard(os);

  double sumArea = std::accumulate(vecPolygons.begin(), vecPolygons.end(), 0.0, accumulatePolygonArea);
  double meanArea = sumArea / static_cast< double >(vecPolygons.size());
  std::cout << std::fixed << std::setprecision(1);
  std::cout << meanArea << '\n';
}

void holodilov::subcommands::areaAmount(std::istream& is, std::ostream& os, const std::vector< Polygon >& vecPolygons, size_t amountVertexes)
{
  ScopeGuard scopeGuard(os);

  auto areaAccumulator = std::bind(accumulatePolygonAreaVertexes, std::placeholders::_1, std::placeholders::_2, amountVertexes);
  double sumArea = std::accumulate(vecPolygons.begin(), vecPolygons.end(), 0.0, areaAccumulator);
  std::cout << std::fixed << std::setprecision(1);
  std::cout << sumArea << '\n';
}