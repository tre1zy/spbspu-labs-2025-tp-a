#include "Commands.hpp"
#include <algorithm>
#include <functional>
#include <iomanip>
#include <numeric>
#include <sstream>



void gavrilova::processArea(const std::vector< Polygon >& polygons, const std::vector< std::string >& commands)
{
  if (polygons.empty()) {
    std::cout << "No polygons detected." << std::endl;
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
      std::cout << "Invalid subcommand." << std::endl;
      return;
    }
  }
  std::cout << std::fixed << std::setprecision(1) << area << std::endl;
}

void gavrilova::processMinMax(const std::vector< Polygon >& polygons, const std::vector< std::string >& commands)
{
  if (polygons.empty()) {
    std::cout << "No polygons detected." << std::endl;
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
    std::cout << std::fixed << std::setprecision(1) << val.area() << std::endl;
  } else if (subcommand == "VERTEXES") {
    auto val = min_max_map.at(command)(polygons, minVertexesComp);
    std::cout << val.points.size() << std::endl;
  } else {
    std::cout << "Invalid subcommand." << std::endl;
  }
}

void gavrilova::processCount(const std::vector< Polygon >& polygons, const std::vector< std::string >& commands)
{
  if (polygons.empty()) {
    std::cout << "No polygons detected." << std::endl;
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
      std::cout << "Invalid subcommand." << std::endl;
      return;
    }
  }

  std::cout << count << std::endl;
}

void gavrilova::processPerms(const std::vector< Polygon >& polygons, const std::vector< std::string >& commands)
{
  auto polygon_str = std::accumulate(
      commands.begin() + 1,
      commands.end(),
      std::string(""),
      [](const std::string& str_1, const std::string& str_2) {
        return str_1 + " " + str_2;
      });

  std::istringstream iss(polygon_str);
  Polygon ref_polygon;

  if (!(iss >> ref_polygon && iss.eof())) {
    std::cout << "Error while parsing Polygon" << std::endl;
    return;
  }

  std::sort(ref_polygon.points.begin(), ref_polygon.points.end());

  size_t count = std::count_if(
      polygons.begin(),
      polygons.end(),
      [&ref_polygon](const Polygon& polygon) {
        auto points = polygon.points;
        std::sort(points.begin(), points.end());
        return points == ref_polygon.points;
      });

  std::cout << count << std::endl;
}

void gavrilova::processLessArea(const std::vector< Polygon >& polygons, const std::vector< std::string >& commands)
{
  auto polygon_str = std::accumulate(
      commands.begin() + 1,
      commands.end(),
      std::string(""),
      [](const std::string& str_1, const std::string& str_2) {
        return str_1 + " " + str_2;
      });

  std::istringstream iss(polygon_str);
  Polygon ref_polygon;

  if (!(iss >> ref_polygon && iss.eof())) {
    std::cout << "Error while parsing Polygon" << std::endl;
    return;
  }

  size_t count = std::count_if(
      polygons.begin(),
      polygons.end(),
      [&ref_polygon](const Polygon& polygon) {
        return polygon.area() < ref_polygon.area();
      });

  std::cout << count << std::endl;
}
