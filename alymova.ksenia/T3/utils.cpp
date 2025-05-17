#include "utils.hpp"
#include <algorithm>
#include <numeric>
#include <exception>
#include <string>
#include <cctype>
#include <functional>

double alymova::area(std::istream& in, const std::vector< Polygon >& polygons)
{
  using namespace std::placeholders;
  AreaSubcommandDataset commands = {
    {"EVEN", areaEven},
    {"ODD", areaOdd},
    {"MEAN", std::bind(areaMean, _1, _2, polygons.size())}
  };
  std::string command;
  in >> command;
  if (command == "MEAN" && polygons.empty())
  {
    throw std::logic_error("<INVALID COMMAND>");
  }
  try
  {
    return std::accumulate(polygons.begin(), polygons.end(), 0.0, commands.at(command));
  }
  catch (...)
  {
    if (std::count_if(command.begin(), command.end(), isDigit) != command.size())
    {
      throw std::logic_error("<INVALID COMMAND>");
    }
  }
  size_t vertexes = std::stoull(command);
  return std::accumulate(polygons.begin(), polygons.end(), 0.0, std::bind(areaNumber, _1, _2, vertexes));
}

double alymova::areaEven(double value, const Polygon& polygon)
{
  if (isPolygonEven(polygon))
  {
    return value + areaOne(polygon);
  }
  return value;
}

double alymova::areaOdd(double value, const Polygon& polygon)
{
  if (!isPolygonEven(polygon))
  {
    return value + areaOne(polygon);
  }
  return value;
}

double alymova::areaMean(double value, const Polygon& polygon, size_t size)
{
  return value + areaOne(polygon) / size;
}

double alymova::areaNumber(double value, const Polygon& polygon, size_t vertexes)
{
  if (polygon.points.size() == vertexes)
  {
    return value + areaOne(polygon);
  }
  return value;
}

double alymova::areaOne(const Polygon& polygon)
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

bool alymova::isDigit(char c)
{
  return std::isdigit(c);
}

alymova::CommandDataset alymova::complectCommands()
{
  using namespace std::placeholders;
  return
  {
    {"AREA", std::bind(area, std::ref(std::cin), _1)}
  };
}