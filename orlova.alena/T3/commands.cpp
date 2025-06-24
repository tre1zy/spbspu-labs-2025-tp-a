#include "commands.h"
#include <functional>
#include <limits>
#include <algorithm>
#include <iomanip>
#include <map>
#include <numeric>
#include "geom.h"

namespace
{
  double subArea(const orlova::Point& a, const orlova::Point& b)
  {
    return a.x * b.y - a.y * b.x;
  }

  double areaPolygon(const orlova::Polygon& polygon)
  {
    const auto& p = polygon.points;
    double sum = std::inner_product(p.begin(), p.end() - 1, p.begin() + 1, 0.0, std::plus< double >{}, subArea);
    sum += subArea (p.back(), p.front());
    return std::abs(sum) / 2.0;
  }

  double sumPolygonAreas(double sum, const orlova::Polygon& polygon)
  {
    return sum + areaPolygon(polygon);
  }

  bool isEven(const orlova::Polygon& polygon)
  {
    return polygon.points.size() % 2 == 0;
  }

  bool isOdd(const orlova::Polygon& polygon)
  {
    return !isEven(polygon);
  }

  bool isNum(const orlova::Polygon& polygon, size_t numOfVertexes)
  {
    return polygon.points.size() == numOfVertexes;
  }

  int isEqual(const orlova::Polygon& a, const orlova::Polygon& b)
  {
    return a == b;
  }

  template < typename T >
  double accumulator(double sum, const orlova::Polygon& polygon, T function)
  {
    return function(polygon) ? sum + areaPolygon(polygon) : sum;
  }

  template < typename T >
  double calculateAreaByCondition(const std::vector< orlova::Polygon >& polygons, T function)
  {
    using namespace std::placeholders;
    return std::accumulate(polygons.begin(), polygons.end(), 0.0, std::bind(accumulator< T >, _1, _2, function));
  }

  bool areaComparator(const orlova::Polygon& a, const orlova::Polygon& b)
  {
    return areaPolygon(a) < areaPolygon(b);
  }

  bool vertexesComparator(const orlova::Polygon& a, const orlova::Polygon& b)
  {
    return a.points.size() < b.points.size();
  }

  bool isPermutation(const orlova::Polygon& polygon1, const orlova::Polygon& polygon2)
  {
    if (polygon1.points.size() != polygon2.points.size())
    {
      return false;
    }
    return std::is_permutation(polygon1.points.begin(), polygon1.points.end(), polygon2.points.begin());
  }

  int getMaxSeq(int a, int b)
  {
    if (a == 0 || b == 0)
    {
      return a;
    }
    return a + b;
  }
}

double orlova::areaEven(const std::vector< Polygon >& polygons)
{
  return calculateAreaByCondition(polygons, isEven);
}

double orlova::areaOdd(const std::vector< Polygon >& polygons)
{
  return calculateAreaByCondition(polygons, isOdd);
}

double orlova::areaMean(const std::vector< Polygon >& polygons)
{
  if (polygons.size() == 0)
  {
    throw std::logic_error("<THERE ARE NO POLYGONS>");
  }
  return std::accumulate(polygons.begin(), polygons.end(), 0.0, sumPolygonAreas) / polygons.size();
}

double orlova::areaNum(const std::vector< Polygon >& polygons, size_t numOfVertexes)
{
  using namespace std::placeholders;
  std::function< bool(const Polygon&) > function = std::bind(isNum, _1, numOfVertexes);
  return calculateAreaByCondition(polygons, function);
}

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

void orlova::max(const std::vector< Polygon >& polygons, std::istream& in, std::ostream& out)
{
  if (polygons.size() == 0)
  {
    throw std::logic_error("<THERE ARE NO POLYGONS>");
  }
  std::string subcommand;
  in >> subcommand;
  out << std::fixed << std::setprecision(1);

  std::map< std::string, std::function< double(const std::vector< Polygon >&) > > subcmds1;
  std::map< std::string, std::function< size_t(const std::vector< Polygon >&) > > subcmds2;
  subcmds1["AREA"] = maxArea;
  subcmds2["VERTEXES"] = maxVertexes;
  auto it1 = subcmds1.find(subcommand);
  if (it1 != subcmds1.end())
  {
    out << it1->second(polygons);
  }
  else
  {
    auto it2 = subcmds2.find(subcommand);
    if (it2 != subcmds2.end())
    {
      out << it2->second(polygons);
    }
    else
    {
      throw std::logic_error("<WRONG SUBCOMMAND>");
    }
  }
}

double orlova::maxArea(const std::vector< Polygon >& polygons)
{
  return areaPolygon(*std::max_element(polygons.begin(), polygons.end(), areaComparator));
}

size_t orlova::maxVertexes(const std::vector< Polygon >& polygons)
{
  return (*std::max_element(polygons.begin(), polygons.end(), vertexesComparator)).points.size();
}

void orlova::min(const std::vector< Polygon >& polygons, std::istream& in, std::ostream& out)
{
  if (polygons.size() == 0)
  {
    throw std::logic_error("<THERE ARE NO POLYGONS>");
  }
  std::string subcommand;
  in >> subcommand;
  out << std::fixed << std::setprecision(1);

  std::map< std::string, std::function< double(const std::vector< Polygon >&) > > subcmds;
  subcmds["AREA"] = minArea;
  subcmds["VERTEXES"] = minVertexes;
  auto it = subcmds.find(subcommand);
  if (it != subcmds.end())
  {
    out << it->second(polygons);
  }
  else
  {
    throw std::logic_error("<WRONG SUBCOMMAND>");
  }
}

double orlova::minArea(const std::vector< Polygon >& polygons)
{
  return areaPolygon(*std::min_element(polygons.begin(), polygons.end(), areaComparator));
}

size_t orlova::minVertexes(const std::vector< Polygon >& polygons)
{
  return (*std::min_element(polygons.begin(), polygons.end(), vertexesComparator)).points.size();
}

void orlova::count(const std::vector< Polygon >& polygons, std::istream& in, std::ostream& out)
{
  std::string subcommand;
  in >> subcommand;
  out << std::fixed << std::setprecision(1);

  std::map< std::string, std::function< size_t(const std::vector< Polygon >&) > > subcmds;
  subcmds["EVEN"] = countEven;
  subcmds["ODD"] = countOdd;
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

    out << countNum(polygons, numOfVertexes);
  }
}

size_t orlova::countEven(const std::vector< Polygon >& polygons)
{
  return std::count_if(polygons.begin(), polygons.end(), isEven);
}

size_t orlova::countOdd(const std::vector< Polygon >& polygons)
{
  return std::count_if(polygons.begin(), polygons.end(), isOdd);
}

size_t orlova::countNum(const std::vector< Polygon >& polygons, size_t numOfVertexes)
{
  using namespace std::placeholders;
  return std::count_if(polygons.begin(), polygons.end(), std::bind(isNum, _1, numOfVertexes));
}

void orlova::perms(const std::vector< Polygon >& polygons, std::istream& in, std::ostream& out)
{
  Polygon polygon;
  in >> polygon;
  if (polygon.points.size() < 3)
  {
    throw std::logic_error("WRONG POLYGON SIZE");
  }
  using namespace std::placeholders;
  out << std::count_if(polygons.begin(), polygons.end(), std::bind(isPermutation, _1, polygon));
}

void orlova::maxseq(const std::vector< Polygon >& polygons, std::istream& in, std::ostream& out)
{
  using namespace std::placeholders;
  Polygon polygon;
  in >> polygon;
  if (polygon.points.size() < 3)
  {
    throw std::logic_error("<WRONG POLYGON SIZE>");
  }
  if (std::count(polygons.begin(), polygons.end(), polygon) == 0)
  {
    throw std::logic_error("THERE ARE NO IDENTIC POLYGONS");
  }
  std::vector< int > seqVector(polygons.size());
  std::transform(polygons.begin(), polygons.end(), seqVector.begin(), std::bind(isEqual, _1, std::cref(polygon)));
  std::transform(seqVector.begin() + 1, seqVector.end(), seqVector.begin(), seqVector.begin() + 1, getMaxSeq);
  out << *std::max_element(seqVector.begin(), seqVector.end());
}
