#ifndef SPBSPU_LABS_2025_TP_A_KIZHIN_EVGENIY_T2_DATA_STRUCT_HPP
#define SPBSPU_LABS_2025_TP_A_KIZHIN_EVGENIY_T2_DATA_STRUCT_HPP

#include <string>
#include <utility>

namespace kizhin {
  struct DataStruct;
  bool operator<(const DataStruct&, const DataStruct&);
}

struct kizhin::DataStruct
{
  double key1;
  std::pair< long long, unsigned long long > key2;
  std::string key3;
};

#endif

