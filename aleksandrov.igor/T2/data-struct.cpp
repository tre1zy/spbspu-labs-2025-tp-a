#include "data-struct.hpp"

namespace aleksandrov
{
  namespace detail
  {
    iofmtguard::iofmtguard(std::basic_ios< char >& s):
      s_(s),
      width_(s.width()),
      fill_(s.fill()),
      precision_(s.precision()),
      fmt_(s.flags())
    {}

    iofmtguard::~iofmtguard()
    {
      s_.width(width_);
      s_.fill(fill_);
      s_.precision(precision_);
      s_.flags(fmt_);
    }
  }
  bool operator<(const DataStruct& lhs, const DataStruct& rhs)
  {
    if (lhs.key1 == rhs.key1)
    {
      if (lhs.key2 == rhs.key2)
      {
        return lhs.key3.size() < rhs.key3.size();
      }
      return (lhs.key2.first / lhs.key2.second) < (rhs.key2.first / rhs.key2.second);
    }
    return lhs.key1 < rhs.key1;
  }
}

