#include "commands.hpp"
#include <iomanip>
#include <streamGuard.hpp>
#include <algorithm>
#include <functional>
#include <iterator>

namespace
{
  bool isOdd(const finaev::Polygon& poly)
  {
    return poly.points.size() % 2 != 0;
  }
  bool isEven(const finaev::Polygon& poly)
  {
    return poly.points.size() % 2 == 0;
  }
  bool isNumOfVertex(const finaev::Polygon& poly, size_t s)
  {
    return poly.points.size() == s;
  }
  double areaOnePoly(const finaev::Polygon& poly)
  {
    if (poly.points.size() < 3)
    {
      return 0.0;
    }
    double res = 0.0;
    size_t s = poly.points.size();
    for (size_t i = 0; i < s; ++i)
    {
      int j = (i + 1) % s;
      res += (poly.points[i].x * poly.points[j].y) - (poly.points[i].y * poly.points[j].x);
    }
    return std::abs(res) / 2.0;
  }
  bool isDigit(const std::string& s) 
  {
    return !s.empty() && std::all_of(s.begin(), s.end(), isdigit);
  }
}

void finaev::count(std::istream& in, std::ostream& out, const std::vector< Polygon >& shapes)
{
  std::string substr;
  in >> substr;
  if (substr == "ODD")
  {
    std::vector< Polygon > odd;
    std::copy_if(shapes.begin(), shapes.end(), std::back_inserter(odd), isOdd);
    out << odd.size();
  }
  else if (substr == "EVEN")
  {
    std::vector< Polygon > even;
    std::copy_if(shapes.begin(), shapes.end(), std::back_inserter(even), isEven);
    out << even.size();
  }
  else if(isDigit(substr))
  {
    size_t s = std::stoul(substr);
    std::vector< Polygon > temp;
    auto compare = std::bind(isNumOfVertex, std::placeholders::_1, s);
    std::copy_if(shapes.begin(), shapes.end(), std::back_inserter(temp), compare);
    out << temp.size();
  }
  else
  {
    std::invalid_argument("<INVALID COMMAND>");
  }
}

void finaev::area(std::istream& in, std::ostream& out, const std::vector< Polygon >& shapes)
{
  std::string substr;
  in >> substr;
  StreamGuard streamGuard(out);
  out << std::fixed << std::setprecision(1);
  if (substr == "ODD")
  {
    std::vector< Polygon > odd;
    std::copy_if(shapes.begin(), shapes.end(), std::back_inserter(odd), isOdd);
    double res = 0.0;
    for (size_t i = 0; i < odd.size(); ++i)
    {
      res += areaOnePoly(odd[i]);
    }
    out << res;
  }
  else if (substr == "EVEN")
  {
    std::vector< Polygon > even;
    std::copy_if(shapes.begin(), shapes.end(), std::back_inserter(even), isEven);
    double res = 0.0;
    for (size_t i = 0; i < even.size(); ++i)
    {
      res += areaOnePoly(even[i]);
    }
    out << res;
  }
  else if (substr == "MEAN")
  {
    if (shapes.empty())
    {
      throw std::invalid_argument("<INVALID COMMAND>");
    }
    double res = 0.0;
    for (size_t i = 0; i < shapes.size(); ++i)
    {
      res += areaOnePoly(shapes[i]);
    }
    out << res / shapes.size();
  }
  else if (isDigit(substr))
  {
    size_t s = std::stoul(substr);
    std::vector< Polygon > temp;
    auto compare = std::bind(isNumOfVertex, std::placeholders::_1, s);
    std::copy_if(shapes.begin(), shapes.end(), std::back_inserter(temp), compare);
    double res = 0.0;
    for (size_t i = 0; i < temp.size(); ++i)
    {
      res += areaOnePoly(temp[i]);
    }
    out << res;
  }
  else
  {
    throw std::invalid_argument("<INVALID COMMAND>");
  }
}
