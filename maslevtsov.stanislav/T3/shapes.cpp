#include "shapes.hpp"
#include <algorithm>
#include <iterator>

namespace {
  struct DelimiterIn
  {
    char exp;
  };

  std::istream& operator>>(std::istream& in, DelimiterIn&& rhs)
  {
    std::istream::sentry sentry(in);
    if (!sentry) {
      return in;
    }
    char c = '0';
    in >> c;
    if (in && (c != rhs.exp)) {
      in.setstate(std::ios::failbit);
    }
    return in;
  }
}

std::istream& maslevtsov::operator>>(std::istream& in, Point& rhs)
{
  std::istream::sentry sentry(in);
  if (!sentry) {
    return in;
  }
  return in >> DelimiterIn{'('} >> rhs.x >> DelimiterIn{';'} >> rhs.y >> DelimiterIn{')'};
}

std::istream& maslevtsov::operator>>(std::istream& in, Polygon& rhs)
{
  std::istream::sentry sentry(in);
  if (!sentry) {
    return in;
  }
  std::size_t vertex_num = 0;
  in >> vertex_num;
  if (vertex_num < 3) {
    in.setstate(std::ios::failbit);
  }
  std::copy_n(std::istream_iterator< Point >(in), vertex_num, std::back_inserter(rhs.points));
  if (!in || rhs.points.size() != vertex_num) {
    rhs.points.clear();
    in.setstate(std::ios::failbit);
  }
  return in;
}
