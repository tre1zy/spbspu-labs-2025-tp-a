#include "sub-commands.hpp"
#include <numeric>
#include <algorithm>
#include <string>

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

bool alymova::isEqualSize(size_t size, const Polygon& polygon)
{
  return size == polygon.points.size();
}

size_t alymova::getVertexes(std::string str)
{
  if (!std::all_of(str.begin(), str.end(), isDigit))
  {
    throw std::logic_error("<INVALID COMMAND>");
  }
  return std::stoull(str);
}

double alymova::multPoints(const Point& point1, const Point& point2)
{
  return point1.x * point2.y - point1.y * point2.x;
}

bool alymova::isPolygonEven(const Polygon& polygon)
{
  return polygon.points.size() % 2 == 0;
}

