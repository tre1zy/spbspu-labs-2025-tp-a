#include <delimiter-io.hpp>
#include "shapes.hpp"

std::istream& cherepkov::operator>>(std::istream& in, Point& point)
{
  std::istream::sentry sent(in);
  if (!sent) 
  {
    return in;
  }

  Point tmp;
  in >> DelimiterIO{'('} >> tmp.x >> DelimiterIO{';'} >> tmp.y >> DelimiterIO{')'};

  if (in)
  {
    point = tmp;
  }
  return in;
}

std::istream& cherepkov::operator>>(std::istream& in, Polygon& polygon)
{
  
}