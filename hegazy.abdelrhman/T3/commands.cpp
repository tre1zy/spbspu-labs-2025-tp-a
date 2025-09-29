#include "commands.hpp"
#include <algorithm>
#include <iomanip>
#include <limits>
#include <numeric>
#include <functional>
#include <tuple>
#include <unordered_map>
#include <string>
#include <set>
#include "geometry.hpp"
#include "stream_guard.hpp"
#include <iomanip>  

namespace
{
  using geom::Polygon;

  bool intersectionCheck(const geom::Polygon& p1, const geom::Polygon& p2)
  {
    if (p1.points.empty() || p2.points.empty()) return false;

    double min_x1 = p1.points[0].x, max_x1 = min_x1, min_y1 = p1.points[0].y, max_y1 = min_y1;
    for (const auto& pt : p1.points) {
      min_x1 = std::min(min_x1, pt.x);
      max_x1 = std::max(max_x1, pt.x);
      min_y1 = std::min(min_y1, pt.y);
      max_y1 = std::max(max_y1, pt.y);
    }

    double min_x2 = p2.points[0].x, max_x2 = min_x2, min_y2 = p2.points[0].y, max_y2 = min_y2;
    for (const auto& pt : p2.points) {
      min_x2 = std::min(min_x2, pt.x);
      max_x2 = std::max(max_x2, pt.x);
      min_y2 = std::min(min_y2, pt.y);
      max_y2 = std::max(max_y2, pt.y);
    }

    bool x_overlap = !(max_x1 < min_x2 || max_x2 < min_x1);
    bool y_overlap = !(max_y1 < min_y2 || max_y2 < min_y1);
    return x_overlap && y_overlap;
  }

}

void bob::printAreaSum(std::istream& input, const std::vector< Polygon >& polygons, std::ostream& output)
{
  std::unordered_map< std::string, std::function< double() > > handlers;
  handlers["EVEN"] = std::bind(totalEvenArea, std::cref(polygons));
  handlers["ODD"] = std::bind(totalOddArea, std::cref(polygons));
  handlers["MEAN"] = std::bind(AverageAreaCalculator{ }, std::cref(polygons));

  std::string param;
  input >> param;

  input >> std::ws;  
  char ch = input.peek();
  if (ch != '\n' && ch != std::char_traits<char>::eof()) {
    throw std::logic_error("<INVALID COMMAND>");
  }

  double result{};
  try
  {
    result = handlers.at(param)();
  }
  catch (const std::out_of_range& e)
  {
    size_t vertices = std::stoull(param);
    if (vertices < 3)
    {
      throw std::invalid_argument("Vertex count too small");
    }
    result = totalAreaByVertexCount(polygons, vertices);
  }

  bob::Iofmtguard guard(output);
  output << std::fixed << std::setprecision(1) << result;
}

void bob::printMaxValueOf(std::istream& input, const std::vector< Polygon >& polygons, std::ostream& output)
{
  if (polygons.empty())
  {
    throw std::invalid_argument("No polygons available");
  }

  std::string param;
  input >> param;

  input >> std::ws;
  char ch = input.peek();
  if (ch != '\n' && ch != std::char_traits<char>::eof()) {
    throw std::logic_error("<INVALID COMMAND>");
  }

  std::unordered_map< std::string, std::function< void() > > handlers;
  handlers["AREA"] = std::bind(outputMaxArea, std::ref(output), std::cref(polygons));
  handlers["VERTEXES"] = std::bind(outputMaxVertices, std::ref(output), std::cref(polygons));

  bob::Iofmtguard guard(output);
  handlers.at(param)();
}

void bob::printMinValueOf(std::istream& input, const std::vector< Polygon >& polygons, std::ostream& output)
{
  if (polygons.empty())
  {
    throw std::invalid_argument("No polygons available");
  }

  std::string param;
  input >> param;

  input >> std::ws;
  char ch = input.peek();
  if (ch != '\n' && ch != std::char_traits<char>::eof()) {
    throw std::logic_error("<INVALID COMMAND>");
  }

  std::unordered_map< std::string, std::function< void() > > handlers;
  handlers["AREA"] = std::bind(outputMinArea, std::ref(output), std::cref(polygons));
  handlers["VERTEXES"] = std::bind(outputMinVertices, std::ref(output), std::cref(polygons));

  bob::Iofmtguard guard(output);
  handlers.at(param)();
}

void bob::printCountOf(std::istream& input, const std::vector< Polygon >& polygons, std::ostream& output)
{
  std::unordered_map< std::string, std::function< size_t() > > handlers;
  handlers["EVEN"] = std::bind(countEvenVertices, std::cref(polygons));
  handlers["ODD"] = std::bind(countOddVertices, std::cref(polygons));

  std::string param;
  input >> param;

  input >> std::ws;
  char ch = input.peek();
  if (ch != '\n' && ch != std::char_traits<char>::eof()) {
    throw std::logic_error("<INVALID COMMAND>");
  }

  size_t count{};
  try
  {
    count = handlers.at(param)();
  }
  catch (const std::out_of_range& e)
  {
    size_t vertices = std::stoull(param);
    if (vertices < 3)
    {
      throw std::invalid_argument("Invalid vertex count parameter");
    }
    count = countByVertexCount(polygons, vertices);
  }

  output << count;
}

void bob::printLessAreaCnt(std::istream& input, const std::vector< Polygon >& polygons, std::ostream& output)
{
  Polygon ref;
  input >> ref;
  if (!input) {
    throw std::logic_error("<INVALID COMMAND>");
  }

  input >> std::ws;
  char ch = input.peek();
  if (ch != '\n' && ch != std::char_traits<char>::eof()) {
    throw std::logic_error("<INVALID COMMAND>");
  }

  using namespace std::placeholders;
  output << std::count_if(polygons.begin(), polygons.end(), std::bind(compareByArea, _1, ref));
}

void bob::printIntersectionsCnt(std::istream& input, const std::vector< geom::Polygon >& polygons, std::ostream& output)
{
  Polygon ref;
  input >> ref;
  if (!input) {
    throw std::invalid_argument("Wrong argument");
  }

  input >> std::ws;
  char ch = input.peek();
  if (ch != '\n' && ch != std::char_traits<char>::eof()) {
    throw std::logic_error("<INVALID COMMAND>");
  }

  std::set< geom::Point > checkForEqualPoints(ref.points.cbegin(), ref.points.cend());
  if (checkForEqualPoints.size() != ref.points.size())
  {
    throw std::invalid_argument("Have equal points");
  }
  using namespace std::placeholders;
  output << std::count_if(polygons.cbegin(), polygons.cend(), std::bind(intersectionCheck, ref, _1));
}