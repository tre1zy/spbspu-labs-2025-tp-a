#include "polygon.hpp"

#include <algorithm>
#include <iterator>

#include "input_delimiter.hpp"

namespace fedorov
{
  bool operator==(const Point &p1, const Point &p2)
  {
    return (p1.x == p2.x) && (p1.y == p2.y);
  }

  std::istream &operator>>(std::istream &in, Point &dest)
  {
    char open, sep, close;
    in >> open;
    if (open != '(')
    {
      in.setstate(std::ios::failbit);
      return in;
    }

    in >> dest.x >> sep >> dest.y >> close;
    if (sep != ';' || close != ')')
    {
      in.setstate(std::ios::failbit);
    }
    return in;
  }

  std::ostream &operator<<(std::ostream &out, const Point &src)
  {
    out << '(' << src.x << ';' << src.y << ')';
    return out;
  }

  bool operator==(const Polygon &p1, const Polygon &p2)
  {
    return p1.points == p2.points;
  }

  bool operator!=(const Polygon &p1, const Polygon &p2)
  {
    return !(p1 == p2);
  }

  struct PointGenerator
  {
    explicit PointGenerator(std::istream &in):
      in(in)
    {}
    Point operator()() const
    {
      Point p;
      in >> p;
      return p;
    }
    std::istream &in;
  };

  std::istream &operator>>(std::istream &in, Polygon &dest)
  {
    int n;
    if (!(in >> n) || n < 3)
    {
      in.setstate(std::ios::failbit);
      return in;
    }

    dest.points.clear();
    std::generate_n(std::back_inserter(dest.points), n, PointGenerator(in));

    if (in.fail() || dest.points.size() != static_cast< size_t >(n))
    {
      dest.points.clear();
      in.setstate(std::ios::failbit);
    }
    return in;
  }

  std::ostream &operator<<(std::ostream &out, const Polygon &src)
  {
    out << src.points.size() << ' ';
    std::copy(src.points.begin(), src.points.end(), std::ostream_iterator< Point >(out, " "));
    return out;
  }
}
