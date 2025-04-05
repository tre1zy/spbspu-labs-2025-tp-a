#ifndef KEYS_HPP
#define KEYS_HPP
#include <string>
#include <iostream>

namespace alymova
{
  struct UllOct
  {
    unsigned long long int& i_oct;
  };
  struct ChrLit
  {
    char& c;
  };
  struct StringIO
  {
    std::string& s;
  };
  struct Delimiter
  {
    char exp;
  };
  struct Label
  {
    std::string exp;
  };
  struct DataStruct
  {
    unsigned long long int key1;
    char key2;
    std::string key3;
  };
  std::istream& operator>>(std::istream& in, Delimiter&& object);
  std::istream& operator>>(std::istream& in, Label&& object);
  std::istream& operator>>(std::istream& in, UllOct&& object);
  std::istream& operator>>(std::istream& in, ChrLit&& object);
  std::istream& operator>>(std::istream& in, StringIO&& object);
  std::istream& operator>>(std::istream& in, DataStruct& object);
  std::ostream& operator<<(std::ostream& out, const DataStruct& object);

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
