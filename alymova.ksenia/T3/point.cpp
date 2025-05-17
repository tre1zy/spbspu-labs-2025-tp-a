#include "shapes.hpp"

std::istream& alymova::operator>>(std::istream& in, Point& point)
{
  std::istream::sentry s(in);
  if (!s)
  {
    return in;
  }
  in >> DelimiterIO{'('} >> point.x >> DelimiterIO{';'} >> point.y >> DelimiterIO{')'};
  return in;
}
std::ostream& alymova::operator<<(std::ostream& out, const Point& point)
{
  std::ostream::sentry s(out);
  if (!s)
  {
    return out;
  }
  std::cout << '(' << point.x << ';' << point.y << ')';
  return out;
}
double alymova::multPoints(const Point& point1, const Point& point2)
{
  return point1.x * point2.y - point1.y * point2.x;
}
