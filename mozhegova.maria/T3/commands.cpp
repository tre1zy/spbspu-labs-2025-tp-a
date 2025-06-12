#include "commands.hpp"
#include <iterator>
#include <algorithm>
#include <numeric>
#include <functional>
#include <map>
#include <scopeGuard.hpp>

namespace
{
  double calcPoints(const mozhegova::Point & p1, const mozhegova::Point & p2)
  {
    return p1.x * p2.y - p1.y * p2.x;
  }

  double getArea(const mozhegova::Polygon & poly)
  {
    if (poly.points.size() < 3)
    {
      return 0.0;
    }
    mozhegova::Polygon rotPoly = poly;
    std::rotate(rotPoly.points.begin(), rotPoly.points.begin() + 1, rotPoly.points.end());
    auto it1 = poly.points.begin();
    auto it2 = poly.points.end();
    auto it3 = rotPoly.points.begin();
    double sum = std::inner_product(it1, it2, it3, 0.0, std::plus< double >(), calcPoints);
    return std::abs(sum) * 0.5;
  }

  bool isEven(size_t num)
  {
    return num % 2 == 0;
  }

  bool isOdd(size_t num)
  {
    return num % 2 != 0;
  }

  bool isEvenPoly(const mozhegova::Polygon & poly)
  {
    return isEven(poly.points.size());
  }

  bool isOddPoly(const mozhegova::Polygon & poly)
  {
    return isOdd(poly.points.size());
  }

  void getAreaEven(std::ostream & out, const std::vector< mozhegova::Polygon > & polygons)
  {
    std::vector< mozhegova::Polygon > temp;
    std::copy_if(polygons.begin(), polygons.end(), std::back_inserter(temp), isEvenPoly);
    std::vector< double > areaPoly(temp.size());
    std::transform(temp.begin(), temp.end(), areaPoly.begin(), getArea);
    double result = std::accumulate(areaPoly.begin(), areaPoly.end(), 0.0);
    mozhegova::iofmtguard fmtguard(out);
    out << std::fixed << std::setprecision(1) << result << '\n';
  }

  void getAreaOdd(std::ostream & out, const std::vector< mozhegova::Polygon > & polygons)
  {
    std::vector< mozhegova::Polygon > temp;
    std::copy_if(polygons.begin(), polygons.end(), std::back_inserter(temp), isOddPoly);
    std::vector< double > areaPoly(temp.size());
    std::transform(temp.begin(), temp.end(), areaPoly.begin(), getArea);
    double result = std::accumulate(areaPoly.begin(), areaPoly.end(), 0.0);
    mozhegova::iofmtguard fmtguard(out);
    out << std::fixed << std::setprecision(1) << result << '\n';
  }

  void getAreaMean(std::ostream & out, const std::vector< mozhegova::Polygon > & polygons)
  {
    std::vector< mozhegova::Polygon > temp = polygons;
    std::vector< double > areaPoly(temp.size());
    std::transform(temp.begin(), temp.end(), areaPoly.begin(), getArea);
    double result = std::accumulate(areaPoly.begin(), areaPoly.end(), 0.0) / polygons.size();
    mozhegova::iofmtguard fmtguard(out);
    out << std::fixed << std::setprecision(1) << result << '\n';
  }
}

void mozhegova::printArea(std::istream & in, std::ostream & out, const std::vector< Polygon > & polygons)
{
  if (polygons.empty())
  {
    throw std::logic_error("not polygons");
  }
  std::map< std::string, std::function< void() > > subcmds;
  subcmds["EVEN"] = std::bind(getAreaEven, std::ref(out), std::cref(polygons));
  subcmds["ODD"] = std::bind(getAreaOdd, std::ref(out), std::cref(polygons));
  subcmds["MEAN"] = std::bind(getAreaMean, std::ref(out), std::cref(polygons));
  std::string subcommand;
  in >> subcommand;
  try
  {
    subcmds.at(subcommand)();
  }
  catch (...)
  {
    throw std::logic_error("not that command");
  }
}

void mozhegova::printMax(std::istream & in, std::ostream & out, const std::vector< Polygon > & polygons);
void mozhegova::printMin(std::istream & in, std::ostream & out, const std::vector< Polygon > & polygons);
void mozhegova::printCount(std::istream & in, std::ostream & out, const std::vector< Polygon > & polygons);
void mozhegova::printEcho(std::istream & in, std::ostream & out, const std::vector< Polygon > & polygons);
void mozhegova::printSame(std::istream & in, std::ostream & out, const std::vector< Polygon > & polygons);
