#include "geometry.hpp"
#include <delimiter.hpp>

namespace
{
  using namespace demehin::io;
}

std::istream& demehin::operator>>(std::istream& in, Point& pt)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }

  {
    using sep = DelimiterIO;

    return in >> sep{ '(' } >> pt.x >> sep{ ';' } >> pt.y >> sep{ ')' };
  }
}

std::istream& demehin::operator>>(std::istream& in, Polygon& plg)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }

  size_t vrtx_cnt;
  if (!(in >> vrtx_cnt))
  {
    in.setstate(std::ios::failbit);
    return in;
  }

  Point pt;
  for (size_t i = 0; i < vrtx_cnt; i++)
  {
    in >> pt;
    plg.points.push_back(pt);
  }

  return in;
}
