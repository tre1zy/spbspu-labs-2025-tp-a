#include "comparator.hpp"

bool maslevtsov::comparator(const maslevtsov::DataStruct& left, const maslevtsov::DataStruct& right)
{
  bool compare1 = left.key1_ < right.key1_;
  bool compare2 = left.key2_ < right.key2_;
  bool compare3 = left.key3_.size() < right.key3_.size();
  return left.key1_ != right.key1_ ? compare1 : (left.key2_ != right.key2_ ? compare2 : compare3);
}
