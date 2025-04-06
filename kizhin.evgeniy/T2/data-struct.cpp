#include "data-struct.hpp"
#include <tuple>

bool kizhin::operator<(const DataStruct& lhs, const DataStruct& rhs)
{
  const auto lhsSize = lhs.key3.size();
  const auto rhsSize = rhs.key3.size();
  const auto lhsTuple = std::tie(lhs.key1, lhs.key2.first, lhs.key2.second, lhsSize);
  const auto rhsTuple = std::tie(rhs.key1, rhs.key2.first, rhs.key2.second, rhsSize);
  return lhsTuple < rhsTuple;
}

