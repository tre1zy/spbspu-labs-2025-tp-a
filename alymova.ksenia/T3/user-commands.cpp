#include "user-commands.hpp"
#include <algorithm>
#include <exception>
#include <string>

void alymova::area(std::istream& in, std::ostream& out, const std::vector< Polygon >& polygons)
{
  using AreaSubcommands = std::map< std::string, std::function< double(const std::vector< Polygon >&) > >;
  AreaSubcommands subs{{"EVEN", areaEven}, {"ODD", areaOdd}, {"MEAN", areaMean}};

  double res;
  std::string command;
  in >> command;
  try
  {
    res = subs.at(command)(polygons);
  }
  catch (const std::out_of_range& e)
  {
    size_t vertexes = getVertexes(command);
    res = areaNumber(vertexes, polygons);
  }
  StreamGuard guard(out);
  out << std::fixed << std::setprecision(1) << res;
}

void alymova::count(std::istream& in, std::ostream& out, const std::vector< Polygon >& polygons)
{
  using CountSubcommands = std::map< std::string, std::function< bool(const Polygon&) > >;
  CountSubcommands subs{{"EVEN", isPolygonEven}, {"ODD", isPolygonOdd}};

  std::string command;
  in >> command;
  try
  {
    out << std::count_if(polygons.begin(), polygons.end(), subs.at(command));
  }
  catch (const std::out_of_range& e)
  {
    size_t vertexes = getVertexes(command);
    out << std::count_if(polygons.begin(), polygons.end(), std::bind(isEqualSize, vertexes, _1));
  }
}

void alymova::inFrame(std::istream& in, std::ostream& out, const std::vector< Polygon >& polygons)
{
  Polygon framed;
  in >> framed;
  if (in.fail() || polygons.empty())
  {
    throw std::logic_error("");
  }
  int start_frame_x = framed.points[0].x;
  int start_frame_y = framed.points[0].y;
  int max_framed_x = findMaxXPolygon(start_frame_x, framed);
  int max_framed_y = findMaxXPolygon(start_frame_y, framed);
  int min_framed_x = findMinXPolygon(start_frame_x, framed);
  int min_framed_y = findMinXPolygon(start_frame_y, framed);

  int start_x = polygons[0].points[0].x;
  int start_y = polygons[0].points[0].y;
  int max_x = findMaxXVector(start_x, polygons);
  int max_y = findMaxYVector(start_y, polygons);
  int min_x = findMinXVector(start_x, polygons);
  int min_y = findMinYVector(start_y, polygons);

  bool res = max_framed_x <= max_x && max_framed_y <= max_y && min_framed_x >= min_x && min_framed_y >= min_y;
  inFrameOutput(out, res);
}

void alymova::rightShapes(std::ostream& out, const std::vector< Polygon >& polygons)
{
  out << std::count_if(polygons.begin(), polygons.end(), haveRightAngles);
}
