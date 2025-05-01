#include "geometry.hpp"
#include <algorithm>
#include <iterator>
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
  if (!(in >> vrtx_cnt) || vrtx_cnt < 3)
  {
    in.setstate(std::ios::failbit);
    return in;
  }

  std::vector< Point > pts(vrtx_cnt);
  std::copy_n(std::istream_iterator< Point >(in), vrtx_cnt, pts.begin());

  if (!in)
  {
    in.setstate(std::ios::failbit);
    return in;
  }

  plg.points = pts;
  return in;
}

//double demehin::getPlgArea(const Polygon& plg)
//{
  //double area = 0;
//}
