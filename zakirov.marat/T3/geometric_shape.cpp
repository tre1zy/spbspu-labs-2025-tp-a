#include "geometric_shape.hpp"
#include <cctype>
#include <algorithm>

namespace
{
  struct MinorSymbol
  {
    char symbol;
  };

  std::istream & operator>>(std::istream & in, MinorSymbol && sym)
  {
    std::istream::sentry sentry(in);
    if (!sentry)
    {
      return in;
    }

    char symbol;
    if (!(in >> symbol) || symbol != sym.symbol)
    {
      in.setstate(std::ios::failbit);
    }

    return in;
  }

}

std::istream & zakirov::operator>>(std::istream & in, Point & point)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }

  Point point_temp{0, 0};
  in >> MinorSymbol{'('};
  in >> point_temp.x_;
  in >> MinorSymbol{';'};
  in >> point_temp.y_;
  in >> MinorSymbol{')'};
  if (in)
  {
    std::swap(point, point_temp);
  }

  return in;
}

std::istream & zakirov::operator>>(std::istream & in, Polygon & polygon)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }

  Polygon polygon_temp;
  size_t point_quantity = 0;
  while (!in.eof())
  {
    in >> point_quantity;
    auto it = polygon.points_.begin();
    Point inserting_point{0, 0};
    for (size_t i = 0; i < point_quantity; ++i, ++it)
    {
      in >> inserting_point;
      polygon_temp.points_.insert(it, inserting_point);
    }
  }

  if (in)
  {
    std::swap(polygon, polygon_temp);
  }

  return in;
}

std::ostream & zakirov::operator<<(std::ostream & out, Point & point)
{
  std::ostream::sentry sentry(out);
  if (!sentry)
  {
    return out;
  }

  out << '(' << point.x_ << ';' << point.y_ << ')';

  return out;
}


