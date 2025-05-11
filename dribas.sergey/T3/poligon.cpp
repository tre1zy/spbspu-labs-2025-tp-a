#include "poligon.hpp"
#include <algorithm>
#include <functional>
#include <iterator>
#include <numeric>
#include <streamGuard.hpp>
#include <delimiter.hpp>

namespace
{
  struct Gauss
  {
    const dribas::Point& pt;
    dribas::Point ptNext;
    double operator()(double res, const dribas::Point& ptLast)
    {
      res += std::abs((ptLast.x - pt.x) * (ptNext.y - pt.y) - (ptNext.x - pt.x) * (ptLast.y - pt.y)) / 2;
      ptNext = ptLast;
      return res;
    }
  };
}

namespace dribas
{
  bool operator==(const Point& r, const Point& l)
  {
    return r.x == l.x && r.y == l.y;
  }

  std::istream& operator>>(std::istream& in, Point& point)
  {
    std::istream::sentry sentry(in);
    if (!sentry) {
      return in;
    }
    StreamGuard guard(in);

    in >> DelimiterI{ '(' } >> point.x;
    in >> DelimiterI{ ';' } >> point.y;
    in >> DelimiterI{ ')' };

    return in;
  }

  std::istream& operator>>(std::istream& in, Poligon& plg)
  {
    std::istream::sentry sentry(in);
    if (!sentry) {
      return in;
    }
    StreamGuard guard(in);

    size_t size = 0;
    if (!(in >> size) || size < 3) {
      in.setstate(std::ios::failbit);
      return in;
    }
    std::vector< Point > pnts(size);
    std::copy_n(std::istream_iterator< Point >(in), size, pnts.begin());

    if (!in) {
      in.setstate(std::ios::failbit);
      return in;
    }

    plg.points = pnts;
    return in;
  }
  double getPoligonArea(const Poligon& poligon)
  {
    Gauss area{ poligon.points[0], poligon.points[1] };
    return std::accumulate(poligon.points.begin() + 2, poligon.points.end(), 0., std::ref(area));
  }
}
