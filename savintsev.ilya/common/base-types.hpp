#ifndef BASE_TYPES_HPP
#define BASE_TYPES_HPP
#include <iostream>

namespace savintsev
{
  struct point_t
  {
    short int x;
    short int y;
  };

  std::istream & operator>>(std::istream & in, point_t & dest);
  std::ostream & operator<<(std::ostream & out, const point_t & dest);

  struct rectangle_t
  {
    short int width;
    short int height;
    point_t pos;
  };
}
#endif
