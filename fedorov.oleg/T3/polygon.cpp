#include "polygon.hpp"

#include <algorithm>
#include <iterator>

namespace fedorov
{
  bool operator==(const Point &p1, const Point &p2)
  {
    return (p1.x == p2.x) && (p1.y == p2.y);
  }

  std::istream &operator>>(std::istream &in, Point &dest)
  {
    char open, sep, close;
    int x, y;

    if (in >> open >> x >> sep >> y >> close && open == '(' && sep == ';' && close == ')')
    {
      dest.x = x;
      dest.y = y;
    }
    else
    {
      in.setstate(std::ios::failbit);
    }
    return in;
  }

  std::ostream &operator<<(std::ostream &out, const Point &src)
  {
    return out << '(' << src.x << ';' << src.y << ')';
  }

  bool operator==(const Polygon &p1, const Polygon &p2)
  {
    return p1.points == p2.points;
  }

  bool operator!=(const Polygon &p1, const Polygon &p2)
  {
    return !(p1 == p2);
  }
  std::istream &operator>>(std::istream &in, Polygon &dest)
  {
    int n;
    if (!(in >> n) || n < 3)
    {
      in.setstate(std::ios::failbit);
      return in;
    }

    dest.points.clear();
    for (int i = 0; i < n; ++i)
    {
      Point p;
      if (!(in >> p))
      {
        dest.points.clear();
        in.setstate(std::ios::failbit);
        return in;
      }
      dest.points.push_back(p);
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
