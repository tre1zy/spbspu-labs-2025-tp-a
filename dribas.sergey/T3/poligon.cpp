#include "poligon.hpp"
#include <algorithm>
#include <functional>
#include <iterator>
#include <numeric>
#include <streamGuard.hpp>
#include <delimiter.hpp>

namespace
{
  struct Gauss {
    dribas::Point prev;
    dribas::Point first;
    double area = 0.0;

    double operator()(double res, const dribas::Point& current) {
      res += (prev.x * current.y - current.x * prev.y);
      prev = current;
      return res;
    }

    double close() {
      area =  (prev.x * first.y - first.x * prev.y);
      return area;
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
    Gauss gauss{poligon.points[0]};
    double area = std::accumulate(poligon.points.begin() + 1, poligon.points.end(), 0.0, std::ref(gauss));
    gauss.close();
    return std::abs(area + gauss.area) / 2.0;
  }
}
