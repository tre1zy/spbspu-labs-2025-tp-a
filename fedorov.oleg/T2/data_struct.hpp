#ifndef DATA_STRUCT_HPP
#define DATA_STRUCT_HPP

#include <complex>
#include <string>
#include <utility>

namespace fedorov {
struct DataStruct {
  std::complex<double> key1;
  std::pair<long long, unsigned long long> key2;
  std::string key3;
};

bool operator<(const DataStruct &a, const DataStruct &b);
std::istream &operator>>(std::istream &in, DataStruct &dest);
std::ostream &operator<<(std::ostream &out, DataStruct &dest);
} // namespace fedorov

#endif
