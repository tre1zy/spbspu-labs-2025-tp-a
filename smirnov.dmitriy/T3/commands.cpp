#include "commands.hpp"
#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include <algorithm>
#include <functional>
#include <map>
#include "polygon.hpp"

namespace smirnov
{
using namespace std::placeholders;

template< class UnaryPredicate >
double getSumArea(const std::vector< Polygon >& polygons, UnaryPredicate P);
double doAreaEven(const std::vector< Polygon >& polygons);
double doAreaOdd(const std::vector< Polygon >& polygons);
double doAreaMean(const std::vector< Polygon >& polygons);
double doAreaNum(const std::vector< Polygon >& polygons, size_t n);

void doAreaCommand(const std::vector< Polygon >& polygons, std::istream& in, std::ostream& out)
{
  std::string s;
  in >> s;
  double result = 0.0;
  std::map< std::string, std::function< double() > > subcommand;
  {
    using namespace std::placeholders;
    subcommand["EVEN"] = std::bind(doAreaEven, polygons);
    subcommand["ODD"] = std::bind(doAreaOdd, polygons);
    subcommand["MEAN"] = std::bind(doAreaMean, polygons);
  }
  try
  {
    size_t n = std::stoull(s);
    if (n < 3)
    {
      throw std::logic_error("FEW VERTEXES");
    }
    result = doAreaNum(polygons, n);
  }
  catch (const std::invalid_argument&)
  {
    result = subcommand[s]();
  }
  std::vector< double > areas;
  out << std::fixed << std::setprecision(1) << result << "\n";
}


}
