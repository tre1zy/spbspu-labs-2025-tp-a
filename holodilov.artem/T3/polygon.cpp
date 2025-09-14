#include "polygon.hpp"

#include <algorithm>
#include <iterator>

#include "DelimIO.hpp"

double holodilov::Polygon::calculateTriangleArea(const size_t index) const
{
  if (index == points.size()-1)
  {
    return points[0].x*std::abs(points[1].y - points[index].y);
  }
  if (index == points.size() -2)
  {
    return points[index+1].x*std::abs(points[0].y - points[index].y) + calculateTriangleArea(index+1);
  }
  return points[index+1].x*std::abs(points[index+2].y - points[index].y) + calculateTriangleArea(index+1);
}

double holodilov::Polygon::getArea() const
{
  if (points.size() < 3)
  {
    return 0;
  }
  return calculateTriangleArea(0) / 2.0;
}

bool holodilov::Point::operator==(const Point& rhs) const
{
  return x == rhs.x && y == rhs.y;
}

bool holodilov::Polygon::operator==(const Polygon& rhs) const
{
  return std::equal(points.begin(), points.end(), rhs.points.begin());
}

std::istream& holodilov::operator>>(std::istream& is, Point& point)
{
  Point pointTemp { 0, 0 };
  is >> DelimIO{ '(' };
  is >> pointTemp.x >> DelimIO{ ';' };
  is >> pointTemp.y >> DelimIO{ ')' };
  if (is)
  {
    point = pointTemp;
  }
  return is;
}

std::istream& holodilov::operator>>(std::istream& is, Polygon& polygon)
{
  int amountPoints = 0;
  is >> amountPoints;
  if (!is)
  {
    return is;
  }

  Polygon polygonTemp;
  using istreamIter = std::istream_iterator< Point >;
  std::copy_n(istreamIter(is), amountPoints, std::back_inserter(polygonTemp.points));
  if (is && polygonTemp.points.size() == amountPoints)
  {
    polygon = polygonTemp;
  }
  return is;
}