#include <iostream>
#include <sstream>
#include <iterator>
#include <limits>
#include <algorithm>
#include <iomanip>
#include <map>
#include <functional>
#include <numeric>
#include "commands.h"

double orlova::subArea(const Point& a, const Point& b)
{
  return a.x * b.y - a.y * b.x;
}

double orlova::areaPolygon(const Polygon& polygon)
{
  const auto& p = polygon.points;
  double sum = std::inner_product(p.begin(), p.end() - 1, p.begin() + 1, 0.0, std::plus< double >{}, subArea);
  sum += subArea (p.back(), p.front());
  return std::abs(sum) / 2.0;
}

void orlova::getPolygons(std::istream& in, std::vector< Polygon >& polygons)
{
  while (!in.eof())
  {
    std::copy(std::istream_iterator< Polygon >(in), std::istream_iterator< Polygon >(), std::back_inserter(polygons));
    if (!in)
    {
      in.clear();
      in.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    }
  }
}

bool orlova::isEven(const Polygon& polygon)
{
  return polygon.points.size() & 2 == 0;
}

double orlova::evenAreaAccumulator(double sum, const Polygon& polygon)
{
  return isEven(polygon) ? sum + areaPolygon(polygon) : sum;
}

double orlova::areaEven(const std::vector< Polygon >& polygons)
{
  return std::accumulate(polygons.begin(), polygons.end(), 0.0, evenAreaAccumulator);
};

void orlova::area(const std::vector< Polygon >& polygons, std::istream& in, std::ostream& out)
{
  std::string subcommand;
  in >> subcommand;
  out << std::fixed << std::setprecision(1);

  std::map< std::string, std::function< double(const std::vector<Polygon>&) > > subcmds;
  subcmds["EVEN"] = areaEven;
  subcmds["ODD"] = areaOdd;
  subcmds["MEAN"] = areaMean;
  auto it = subcmds.find(subcommand);
  if (it != subcmds.end())
  {
    out << it->second(polygons);
  }
  else
  {
    size_t numOfVertexes = std::stoull(subcommand);
    if (numOfVertexes < 3)
    {
      throw std::logic_error("<WRONG SUBCOMMAND>");
    }

    out << areaNum(polygons, numOfVertexes);
  }
}

void orlova::max(const std::vector< Polygon >&, std::istream&, std::ostream&);
double orlova::maxArea(const std::vector< Polygon >&);
size_t orlova::maxVertexes(const std::vector< Polygon >&);

void orlova::min(const std::vector< Polygon >&, std::istream&, std::ostream&);
double orlova::minArea(const std::vector< Polygon >&);
size_t orlova::minVertexes(const std::vector< Polygon >&);

void orlova::count(const std::vector< Polygon >&, std::istream&, std::ostream&);
size_t orlova::countEven(const std::vector< Polygon >&);
size_t orlova::countOdd(const std::vector< Polygon >&);
size_t orlova::countNum(const std::vector< Polygon >&, size_t);
