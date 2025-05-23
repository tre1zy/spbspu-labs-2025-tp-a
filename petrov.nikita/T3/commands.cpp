#include "commands.hpp"
#include <functional>
#include <iomanip>
#include <numeric>
#include <map>
#include <string>
#include <cctype>
#include <stdexcept>

namespace
{
  bool isDigit(std::string & string);

  bool isDigit(std::string & string)
  {
    bool is_digit = true;
    for (size_t i = 0; i < string.size() && is_digit; i++)
    {
      if (!bool(isdigit(string[i])))
      {
        is_digit = false;
      }
    }
    return is_digit;
  }
}

double petrov::calculateTrapezeArea(const Point & point_1, const Point & point_2)
{
  return 0.5 * (point_1.y + point_2.y) * (point_2.x - point_1.x);
}

double petrov::calculateArea(const Polygon & polygon)
{
  double result = 0.0;
  for (size_t i = 0; i < polygon.points.size() - 1; i++)
  {
    result += calculateTrapezeArea(polygon.points[i], polygon.points[i + 1]);
  }
  return result;
}

void petrov::even(const std::vector< Polygon > & polygons, std::ostream & out)
{
  double result = 0.0;
  for (size_t i = 0; i < polygons.size(); i++)
  {
    if (polygons[i].points.size() % 2 == 1)
    {
      result += calculateArea(polygons[i]);
    }
  }
  out << result;
}

void petrov::odd(const std::vector< Polygon > & polygons, std::ostream & out)
{
  double result = 0.0;
  for (size_t i = 0; i < polygons.size(); i++)
  {
    if (polygons[i].points.size() % 2 == 0)
    {
      result += calculateArea(polygons[i]);
    }
  }
  out << result;
}
void petrov::mean(const std::vector< Polygon > & polygons, std::ostream & out)
{
  double result = 0.0;
  for (size_t i = 0; i < polygons.size(); i++)
  {
    result += calculateArea(polygons[i]);
  }
  out << result / polygons.size();
}

void petrov::sum(size_t & num_of_vertexes, const std::vector< Polygon > & polygons, std::ostream & out)
{
  double result = 0.0;
  for (size_t i = 0; i < polygons.size(); i++)
  {
    if (polygons[i].points.size() == num_of_vertexes)
    {
      result += calculateArea(polygons[i]);
    }
  }
  out << result;
}


void petrov::area(const std::vector< Polygon > & polygons, std::istream & in, std::ostream & out)
{
  std::map< std::string, std::function< void() > > subcmds;
  size_t num_of_vertexes = 0;
  out << std::fixed << std::setprecision(1);
  subcmds["EVEN"] = std::bind(even, std::cref(polygons), std::ref(out));
  subcmds["ODD"] = std::bind(odd, std::cref(polygons), std::ref(out));
  subcmds["MEAN"] = std::bind(mean, std::cref(polygons), std::ref(out));
  subcmds["SUM"] = std::bind(sum, std::ref(num_of_vertexes), std::cref(polygons), std::ref(out));
  std::string subcommand;
  if (in >> subcommand)
  {
    if (isDigit(subcommand))
    {
      char * p_end;
      num_of_vertexes = strtoull(subcommand.c_str(), &p_end, subcommand.size() - 1);
      subcmds["SUM"]();
    }
    else
    {
      try
      {
        subcmds.at(subcommand)();
      }
      catch (...)
      {
        throw std::logic_error("<INVALID COMMAND>");
      }
    }
  }
}