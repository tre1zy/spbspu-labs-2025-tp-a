#ifndef KEYS_HPP
#define KEYS_HPP
#include <string>
#include <iostream>
#include <vector>

namespace alymova
{
  struct UllOctIO
  {
    unsigned long long int& i_oct;
  };
  struct ChrLitIO
  {
    char& c;
  };
  struct StringIO
  {
    std::string& s;
  };
  struct DelimiterIO
  {
    char exp;
  };
  struct LabelIO
  {
    std::string exp;
  };
  struct DataStruct
  {
    unsigned long long int key1;
    char key2;
    std::string key3;
  };
  std::istream& operator>>(std::istream& in, DelimiterIO&& object);
  std::istream& operator>>(std::istream& in, LabelIO&& object);
  std::istream& operator>>(std::istream& in, UllOctIO&& object);
  std::istream& operator>>(std::istream& in, ChrLitIO&& object);
  std::istream& operator>>(std::istream& in, StringIO&& object);
  std::istream& operator>>(std::istream& in, DataStruct& object);
  std::ostream& operator<<(std::ostream& out, const DataStruct& object);

  bool comparator(const DataStruct& data1, const DataStruct& data2);

  class StreamGuard
  {
  public:
    StreamGuard(std::basic_ios< char >& s);
    ~StreamGuard();
  private:
    std::basic_ios< char >& s_;
    std::streamsize width_;
    char fill_;
    std::streamsize precision_;
    std::basic_ios< char >::fmtflags fmt_;
  };
}
#endif
