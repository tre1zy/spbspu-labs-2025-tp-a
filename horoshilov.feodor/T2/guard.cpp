#include "guard.h"

namespace horoshilov
{
  Guard::Guard(std::basic_ios< char >& s):
    s_(s),
    fmt_(s.flags()),
    width_(s.width()),
    precision_(s.precision()),
    fill_(s.fill())
  {
  }

  Guard::~Guard()
  {
    s_.width(width_);
    s_.fill(fill_);
    s_.precision(precision_);
    s_.flags(fmt_);
  }
}
