#include "data_input.hpp"
#include <limits>

namespace trukhanov
{
  std::istream& operator>>(std::istream& in, DelimiterIO&& dest)
  {
    std::istream::sentry sentry(in);
    if (!sentry) return in;

    char c = '\0';
    in >> c;
    if (in && c != dest.exp)
    {
      in.setstate(std::ios::failbit);
    }
    return in;
  }

  std::istream& operator>>(std::istream& in, Point& point)
  {
    std::istream::sentry sentry(in);
    if (!sentry) return in;

    in >> DelimiterIO{ '(' } >> point.x >> DelimiterIO{ ';' } >> point.y >> DelimiterIO{ ')' };
    return in;
  }

  std::ostream& operator<<(std::ostream& out, const Point& point)
  {
    out << '(' << point.x << ';' << point.y << ')';
    return out;
  }

  std::istream& operator>>(std::istream& in, Polygon& polygon)
  {
    std::istream::sentry sentry(in);
    if (!sentry) return in;

    size_t n = 0;
    in >> n;
    if (!in || n < 3)
    {
      in.setstate(std::ios::failbit);
      return in;
    }

    Polygon temp;
    temp.points.reserve(n);
    for (size_t i = 0; i < n; ++i)
    {
      Point p;
      in >> p;
      if (!in)
      {
        in.setstate(std::ios::failbit);
        return in;
      }
      temp.points.push_back(p);
    }

    polygon = std::move(temp);
    return in;
  }

  std::ostream& operator<<(std::ostream& out, const Polygon& polygon)
  {
    out << polygon.points.size() << ' ';
    for (const auto& p : polygon.points)
    {
      out << p << ' ';
    }
    return out;
  }
}
