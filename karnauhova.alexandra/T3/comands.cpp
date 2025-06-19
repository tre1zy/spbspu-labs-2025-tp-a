#include "comands.hpp"
#include <numeric>
#include <algorithm>
#include <map>
#include <string>
#include <functional>
#include <stdexcept>
#include <iomanip>

namespace
{
  bool isEven(const karnauhova::Polygon& pol)
  {
    return pol.points.size() % 2 == 0;
  }

  bool isOdd(const karnauhova::Polygon& pol)
  {
    return !isEven(pol);
  }

  bool allAreas(const karnauhova::Polygon& pol)
  {
    return true;
  }

  struct CountCompare
  {
    size_t count;
    bool operator()(const karnauhova::Polygon& pol)
    {
      return count == pol.points.size();
    }
  };

  template< typename Compare >
  double sumAreas(const std::vector< karnauhova::Polygon >& polygons, Compare C)
  {
    std::vector< karnauhova::Polygon > it;
    std::copy_if(polygons.begin(), polygons.end(), std::back_inserter(it), C);
    std::vector< double > areas;
    std::transform(it.begin(), it.end(), std::back_inserter(areas), karnauhova::getArea);
    return std::accumulate(areas.begin(), areas.end(), 0.0);
  }
}

void karnauhova::areaComands(std::istream& in, std::ostream& out, const std::vector< Polygon >& polygons)
{
  std::map< std::string, std::function< double() > > commands;
  commands["EVEN"] = std::bind(evenArea, std::cref(polygons));
  commands["ODD"] = std::bind(oddArea, std::cref(polygons));
  commands["MEAN"] = std::bind(meanArea, std::cref(polygons));
  std::string command;
  in >> command;
  double sum = 0;
  try
  {
    sum = commands.at(command)();
  }
  catch(...)
  {
    size_t count = std::stoull(command);
    if (count < 3)
    {
      throw std::logic_error("Error: few vertices");
    }
    sum = countArea(polygons, count);
  }
  //karnauhova::iofmtguard scope(out);
  out << std::fixed << std::setprecision(1) << sum << "\n";
}

double karnauhova::evenArea(const std::vector< Polygon >& polygons)
{
  return sumAreas(polygons, isEven);
}

double karnauhova::oddArea(const std::vector< Polygon >& polygons)
{
  return sumAreas(polygons, isOdd);
}

double karnauhova::meanArea(const std::vector< Polygon >& polygons)
{
  if (polygons.empty())
  {
    throw std::logic_error("Not enough polygons");
  }
  return sumAreas(polygons, allAreas) / polygons.size();
}

double karnauhova::countArea(const std::vector< Polygon >& polygons, size_t count)
{
  return sumAreas(polygons, CountCompare{ count });
}



