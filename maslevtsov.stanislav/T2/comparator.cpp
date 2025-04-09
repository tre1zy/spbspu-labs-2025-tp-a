#include "comparator.hpp"

bool maslevtsov::comparator(const maslevtsov::DataStruct& left, const maslevtsov::DataStruct& right)
{
  bool fst_compare = left.key1_ < right.key1_;
  bool sec_compare = left.key2_ < right.key2_;
  bool thd_compare = left.key3_.size() < right.key3_.size();
  return left.key1_ != right.key1_ ? fst_compare : (left.key1_ != right.key1_ ? sec_compare : thd_compare);
}
