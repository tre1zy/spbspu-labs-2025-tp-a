#ifndef DATASTRUCT_HPP
#define DATASTRUCT_HPP
#include <string>

namespace krylov
{
  struct DataStruct
  {
    unsigned long long key1;
    unsigned long long key2;
    std::string key3;
  };
  std::ostream &operator<<(std::ostream &out, const DataStruct &data);
  std::istream &operator>>(std::istream &in, DataStruct &data);
  bool operator<(const DataStruct &lhs, const DataStruct &rhs);
  struct DelimiterIO
  {
    char delim;
  };
  std::istream &operator>>(std::istream &in, DelimiterIO &&dest);

  struct UllHexIO
  {
    unsigned long long int &ref;
  };
  std::istream &operator>>(std::istream &in, UllHexIO &&dest);

  struct UllBinIO
  {
    unsigned long long int &ref;
  };
  std::istream &operator>>(std::istream &in, UllBinIO &&dest);

  struct StringIO
  {
    std::string &ref;
  };
  std::istream &operator>>(std::istream &in, StringIO &&dest);
}

#endif

