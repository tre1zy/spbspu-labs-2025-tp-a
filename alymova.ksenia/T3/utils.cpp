#include "utils.hpp"
#include <algorithm>
#include <numeric>
#include <exception>
#include <string>
#include <iomanip>
#include "sub-utils.hpp"

void alymova::area(std::istream& in, std::ostream& out, const std::vector< Polygon >& polygons)
{
  using namespace std::placeholders;

  AreaSubcommands subs{
    {"EVEN", areaEven},
    {"ODD", areaOdd},
    {"MEAN", std::bind(areaMean, _1, _2, polygons.size())}
  };
  double res;
  std::string command;
  in >> command;
  if (command == "MEAN" && polygons.empty())
  {
    throw std::logic_error("<INVALID COMMAND>");
  }
  try
  {
    res = std::accumulate(polygons.begin(), polygons.end(), 0.0, subs.at(command));
  }
  catch (...)
  {
    size_t vertexes = getVertexes(command);
    res = std::accumulate(polygons.begin(), polygons.end(), 0.0, std::bind(areaNumber, _1, _2, vertexes));
  }
  out << std::fixed << std::setprecision(1) << res;
}

void alymova::maxAndMin(const MaxMinSubcommands& subs, std::istream& in,
  std::ostream& out, const std::vector< Polygon >& polygons)
{
  if (polygons.empty())
  {
    throw std::logic_error("<INVALID COMMAND>");
  }
  std::string command;
  in >> command;
  std::cout << command << '\n';
  if (command == "AREA")
  {
    double res = std::accumulate(polygons.begin(), polygons.end(), areaPolygon(polygons[0]), subs.at(command));
    out << std::fixed << std::setprecision(1) << res;
    return;
  }
  out << std::accumulate(polygons.begin(), polygons.end(), polygons[0].points.size(), subs.at(command));
}

void alymova::count(std::istream& in, std::ostream& out, const std::vector< Polygon >& polygons)
{
  using namespace std::placeholders;

  if (polygons.empty())
  {
    throw std::logic_error("<INVALID COMMAND>");
  }
  CountSubcommands subs{
    {"EVEN", isPolygonEven},
    {"ODD", std::not1(std::function< bool(const Polygon&) >(isPolygonEven))}
  };
  std::string command;
  in >> command;
  try
  {
    out << std::count_if(polygons.begin(), polygons.end(), subs.at(command));
  }
  catch(const std::exception& e)
  {
    size_t vertexes = getVertexes(command);
    out << std::count_if(polygons.begin(), polygons.end(), std::bind(isEqualSize, vertexes, _1));
  }
}

alymova::CommandDataset alymova::complectCommands()
{
  using namespace std::placeholders;

  return
  {
    {"AREA", std::bind(area, std::ref(std::cin), std::ref(std::cout), _1)},
    {"MAX", std::bind(
      maxAndMin,
      MaxMinSubcommands{{"AREA", compareMaxArea}, {"VERTEXES", compareMaxVertexes}},
      std::ref(std::cin),
      std::ref(std::cout),
      _1)
    },
    {"MIN", std::bind(
      maxAndMin,
      MaxMinSubcommands{{"AREA", compareMinArea}, {"VERTEXES", compareMinVertexes}},
      std::ref(std::cin),
      std::ref(std::cout),
      _1)
    },
    {"COUNT", std::bind(count, std::ref(std::cin), std::ref(std::cout), _1)}
  };
}
