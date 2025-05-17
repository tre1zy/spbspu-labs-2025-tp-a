#include "utils.hpp"
#include <algorithm>
#include <numeric>
#include <exception>
#include <string>
#include <cctype>
#include <functional>
#include <iomanip>

void alymova::area(std::istream& in, std::ostream& out, const std::vector< Polygon >& polygons)
{
  using namespace std::placeholders;
  AreaSubcommandDataset subs{
    {"EVEN", areaEven},
    {"ODD", areaOdd},
    {"MEAN", std::bind(areaMean, _1, _2, polygons.size())}
  };
  double res;
  std::string command;
  in >> command;
  if (command != "EVEN" && command != "ODD" && polygons.empty())
  {
    throw std::logic_error("<INVALID COMMAND>");
  }
  try
  {
    res = std::accumulate(polygons.begin(), polygons.end(), 0.0, subs.at(command));
  }
  catch (...)
  {
    if (std::count_if(command.begin(), command.end(), isDigit) != command.size())
    {
      throw std::logic_error("<INVALID COMMAND>");
    }
    size_t vertexes = std::stoull(command);
    res = std::accumulate(polygons.begin(), polygons.end(), 0.0, std::bind(areaNumber, _1, _2, vertexes));
  }
  out << std::fixed << std::setprecision(1) << res;
}

double alymova::areaEven(double value, const Polygon& polygon)
{
  if (isPolygonEven(polygon))
  {
    return value + areaPolygon(polygon);
  }
  return value;
}

double alymova::areaOdd(double value, const Polygon& polygon)
{
  if (!isPolygonEven(polygon))
  {
    return value + areaPolygon(polygon);
  }
  return value;
}

double alymova::areaMean(double value, const Polygon& polygon, size_t size)
{
  return value + areaPolygon(polygon) / size;
}

double alymova::areaNumber(double value, const Polygon& polygon, size_t vertexes)
{
  if (polygon.points.size() == vertexes)
  {
    return value + areaPolygon(polygon);
  }
  return value;
}

double alymova::areaPolygon(const Polygon& polygon)
{
  Polygon polygon_rotate(polygon);
  std::rotate(
    polygon_rotate.points.begin(),
    polygon_rotate.points.begin() + 1,
    polygon_rotate.points.end()
  );
  double res = std::inner_product(
    polygon.points.begin(),
    polygon.points.end(),
    polygon_rotate.points.begin(),
    0,
    std::plus< double >{},
    multPoints
  );
  return std::abs(res) / 2.0;
}

void alymova::maxAndMin(const MaxMinSubcommandDataset& subs, std::istream& in,
  std::ostream& out, const std::vector< Polygon >& polygons)
{
  if (polygons.empty())
  {
    throw std::logic_error("<INVALID ERRROR>");
  }
  std::string command;
  in >> command;
  if (command == "AREA")
  {
    double res = std::accumulate(polygons.begin(), polygons.end(), areaPolygon(polygons[0]), subs.at(command));
    out << std::fixed << std::setprecision(1) << res;
    return;
  }
  out << std::accumulate(polygons.begin(), polygons.end(), polygons[0].points.size(), subs.at(command));
}

double alymova::compareMaxArea(double value, const Polygon& polygon)
{
  return std::max(value, areaPolygon(polygon));
}

size_t alymova::compareMaxVertexes(size_t value, const Polygon& polygon)
{
  return std::max(value, polygon.points.size());
}

double alymova::compareMinArea(double value, const Polygon& polygon)
{
  return std::min(value, areaPolygon(polygon));
}

size_t alymova::compareMinVertexes(size_t value, const Polygon& polygon)
{
  return std::min(value, polygon.points.size());
}

bool alymova::isDigit(char c)
{
  return std::isdigit(c);
}

alymova::CommandDataset alymova::complectCommands()
{
  using namespace std::placeholders;
  return
  {
    {"AREA", std::bind(area, std::ref(std::cin), std::ref(std::cout), _1)},
    {"MAX", std::bind(maxAndMin,
      MaxMinSubcommandDataset{{"AREA", compareMaxArea}, {"VERTEXES", compareMaxVertexes}},
      std::ref(std::cin),
      std::ref(std::cout),
      _1)
    },
    {"MIN", std::bind(maxAndMin,
      MaxMinSubcommandDataset{{"AREA", compareMinArea}, {"VERTEXES", compareMinVertexes}},
      std::ref(std::cin),
      std::ref(std::cout),
      _1)
    }
  };
}