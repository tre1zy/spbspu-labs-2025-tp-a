#include "Commands.hpp"
#include <algorithm>
#include <functional>
#include <iomanip>
#include <numeric>
#include <sstream>

namespace {
  double calcArea(const Polygon& polygon)
  {
    return polygon.area();
  }

  double calcAreaOdd(const Polygon& polygon)
  {
    return polygon.isOdd() ? calcArea(polygon) : 0.0;
  }

  double calcAreaEven(const Polygon& polygon)
  {
    return polygon.isEven() ? calcArea(polygon) : 0.0;
  }

  double calcAreaNum(const Polygon& polygon, size_t num)
  {
    return (polygon.points.size() == num) ? calcArea(polygon) : 0.0;
  }

  bool minAreaComp(const Polygon& polygon_1, const Polygon& polygon_2)
  {
    return polygon_1.area() < polygon_2.area();
  }

  bool minVertexesComp(const Polygon& polygon_1, const Polygon& polygon_2)
  {
    return polygon_1.points.size() < polygon_2.points.size();
  }

  const Polygon& findMinPolygon(const std::vector< Polygon >& polygons,
      const std::function< bool(const Polygon&, const Polygon&) >& comp)
  {
    return *std::min_element(polygons.begin(), polygons.end(), comp);
  }

  const Polygon& findMaxPolygon(const std::vector< Polygon >& polygons,
      const std::function< bool(const Polygon&, const Polygon&) >& comp)
  {
    return *std::max_element(polygons.begin(), polygons.end(), comp);
  }

  size_t calcCountOdd(const Polygon& polygon)
  {
    return polygon.isOdd() ? 1 : 0;
  }

  size_t calcCountEven(const Polygon& polygon)
  {
    return polygon.isEven() ? 1 : 0;
  }

  double calcCountNum(const Polygon& polygon, size_t num)
  {
    return (polygon.points.size() == num) ? 1 : 0.0;
  }
}

void gavrilova::processArea(const std::vector< Polygon >& polygons, const std::vector< std::string >& commands)
{
  if (polygons.empty()) {
    std::cout << "No polygons detected." << "\n";
    return;
  }

  auto& subcommand = commands.at(1);
  double area = 0.0;

  static auto apply_command = std::bind(
      gavrilova::applyCommand< std::vector< Polygon >, double, std::function< double(const Polygon&) >, std::plus<> >,
      std::placeholders::_1,
      0.0,
      std::placeholders::_2,
      std::plus<>());

  if (subcommand == "MEAN") {
    area = apply_command(polygons, calcArea);
    area = area / static_cast< double >(polygons.size());
  } else if (subcommand == "ODD") {
    area = apply_command(polygons, calcAreaOdd);
  } else if (subcommand == "EVEN") {
    area = apply_command(polygons, calcAreaEven);
  } else {
    size_t num = 0;
    try {
      num = std::stoll(subcommand);
      area = apply_command(
          polygons,
          std::bind(
              calcAreaNum,
              std::placeholders::_1,
              num));
    } catch (...) {
      std::cout << "Invalid subcommand." << "\n";
      return;
    }
  }
  std::cout << std::fixed << std::setprecision(1) << area << "\n";
}

void gavrilova::processMinMax(const std::vector< Polygon >& polygons, const std::vector< std::string >& commands)
{
  if (polygons.empty()) {
    std::cout << "No polygons detected." << "\n";
    return;
  }

  static std::map< std::string, std::function< const Polygon&(
                                    const std::vector< Polygon >&,
                                    const std::function< bool(const Polygon&, const Polygon&) >&) > >
      min_max_map = {
          {"MIN", findMinPolygon},
          {"MAX", findMaxPolygon}};

  auto& command = commands.at(0);
  auto& subcommand = commands.at(1);

  if (subcommand == "AREA") {
    auto val = min_max_map.at(command)(polygons, minAreaComp);
    std::cout << std::fixed << std::setprecision(1) << val.area() << "\n";
  } else if (subcommand == "VERTEXES") {
    auto val = min_max_map.at(command)(polygons, minVertexesComp);
    std::cout << val.points.size() << "\n";
  } else {
    std::cout << "Invalid subcommand." << "\n";
  }
}

void gavrilova::processCount(const std::vector< Polygon >& polygons, const std::vector< std::string >& commands)
{
  if (polygons.empty()) {
    std::cout << "No polygons detected." << "\n";
    return;
  }

  auto& subcommand = commands.at(1);
  static auto apply_command = std::bind(
      gavrilova::applyCommand< std::vector< Polygon >, size_t, std::function< size_t(const Polygon&) >, std::plus<> >,
      std::placeholders::_1,
      static_cast< size_t >(0),
      std::placeholders::_2,
      std::plus<>());

  size_t count = 0;

  if (subcommand == "ODD") {
    count = apply_command(polygons, calcCountOdd);
  } else if (subcommand == "EVEN") {
    count = apply_command(polygons, calcCountEven);
  } else {
    size_t num = 0;
    try {
      num = std::stoll(subcommand);
      count = apply_command(
          polygons,
          std::bind(
              calcCountNum,
              std::placeholders::_1,
              num));
    } catch (...) {
      std::cout << "Invalid subcommand." << "\n";
      return;
    }
  }

  std::cout << count << "\n";
}

void gavrilova::processPerms(const std::vector< Polygon >& polygons, const std::vector< std::string >& commands)
{
  auto polygon_str = std::accumulate(
      commands.begin() + 1,
      commands.end(),
      std::string(""),
      [](const std::string& str_1, const std::string& str_2)
      {
        return str_1 + " " + str_2;
      });

  std::istringstream iss(polygon_str);
  Polygon ref_polygon;

  if (!(iss >> ref_polygon && iss.eof())) {
    std::cout << "Error while parsing Polygon" << "\n";
    return;
  }

  std::sort(ref_polygon.points.begin(), ref_polygon.points.end());

  size_t count = std::count_if(
      polygons.begin(),
      polygons.end(),
      [&ref_polygon](const Polygon& polygon)
      {
        auto points = polygon.points;
        std::sort(points.begin(), points.end());
        return points == ref_polygon.points;
      });

  std::cout << count << "\n";
}

void gavrilova::processLessArea(const std::vector< Polygon >& polygons, const std::vector< std::string >& commands)
{
  auto polygon_str = std::accumulate(
      commands.begin() + 1,
      commands.end(),
      std::string(""),
      [](const std::string& str_1, const std::string& str_2)
      {
        return str_1 + " " + str_2;
      });

  std::istringstream iss(polygon_str);
  Polygon ref_polygon;

  if (!(iss >> ref_polygon && iss.eof())) {
    std::cout << "Error while parsing Polygon" << "\n";
    return;
  }

  size_t count = std::count_if(
      polygons.begin(),
      polygons.end(),
      [&ref_polygon](const Polygon& polygon)
      {
        return polygon.area() < ref_polygon.area();
      });

  std::cout << count << "\n";
}
