#ifndef SPBSPU_LABS_2025_TP_A_KIZHIN_EVGENIY_T2_DATA_STRUCT_HPP
#define SPBSPU_LABS_2025_TP_A_KIZHIN_EVGENIY_T2_DATA_STRUCT_HPP

#include <string>
#include <utility>

namespace kizhin {
  struct DataStruct;
  bool operator<(const DataStruct&, const DataStruct&);
}

struct kizhin::DataStruct final
{
  double key1;
  struct Rational
  {
    long long numerator;
    unsigned long long denominator;
  } key2;
  std::string key3;
};

#endif

