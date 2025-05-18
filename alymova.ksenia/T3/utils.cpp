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

void alymova::inFrame(std::istream& in, std::ostream& out, const std::vector< Polygon >& polygons)
{
  using namespace std::placeholders;

  Polygon framed;
  in >> framed;
  if (in.fail() && polygons.empty())
  {
    throw std::logic_error("<INVALID COMMAND>");
  }
  int start_frame_x = framed.points[0].x;
  int start_frame_y = framed.points[0].y;
  int max_framed_x = findMaxMinXYPolygon(start_frame_x, framed, compareMaxXPoint);
  int max_framed_y = findMaxMinXYPolygon(start_frame_y, framed, compareMaxYPoint);
  int min_framed_x = findMaxMinXYPolygon(start_frame_x, framed, compareMinXPoint);
  int min_framed_y = findMaxMinXYPolygon(start_frame_y, framed, compareMinYPoint);

  int start_x = polygons[0].points[0].x;
  int start_y = polygons[0].points[0].y;
  int max_x = findMaxMinXYVector(start_x, polygons, compareMaxXPoint);
  int max_y = findMaxMinXYVector(start_y, polygons, compareMaxYPoint);
  int min_x = findMaxMinXYVector(start_x, polygons, compareMinXPoint);
  int min_y = findMaxMinXYVector(start_y, polygons, compareMinYPoint);
  if (max_framed_x <= max_x && max_framed_y <= max_y && min_framed_x >= min_x && min_framed_y >= min_y)
  {
    out << "<TRUE>";
    return;
  }
  out << "<FALSE>";
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
    {"COUNT", std::bind(count, std::ref(std::cin), std::ref(std::cout), _1)},
    {"INFRAME", std::bind(inFrame, std::ref(std::cin), std::ref(std::cout), _1)}
  };
}
