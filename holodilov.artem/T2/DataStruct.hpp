#ifndef DATA_STRUCT_HPP
#define DATA_STRUCT_HPP

#include <string>
#include <iostream>
#include <complex>

namespace holodilov
{
  struct DataStruct
  {
  public:
    unsigned long long key1;
    std::complex< double > key2;
    std::string key3;
  };

  namespace manipulator
  {
    struct CharIO
    {
      char ch;
    };

    struct KeyNameIO
    {
      char keyNameNumber;
    };

    struct UllOctIO
    {
      unsigned long long value;
    };

    struct CmpLspIO
    {
      std::complex< double >& value;
    };

    struct StringIO
    {
      std::string& value;
    };
  }

  std::istream& operator>>(std::istream& is, DataStruct& dataStruct);
  std::istream& operator>>(std::istream& is, manipulator::CharIO& charIO);
  std::istream& operator>>(std::istream& is, manipulator::KeyNameIO& keyNameIO);
  std::istream& operator>>(std::istream& is, manipulator::UllOctIO& ullOctIO);
  std::istream& operator>>(std::istream& is, manipulator::CmpLspIO& cmpLspIO);
  std::istream& operator>>(std::istream& is, manipulator::StringIO& stringIO);

  std::ostream& operator<<(std::ostream& os, const DataStruct& dataStruct);
  std::ostream& operator<<(std::ostream& os, const manipulator::KeyNameIO& keyNameIO);

  bool operator<(const DataStruct& lhs, const DataStruct& rhs);
}

#endif //DATA_STRUCT_HPP
