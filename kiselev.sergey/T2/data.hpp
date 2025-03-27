#ifndef DATA_HPP
#define DATA_HPP
#include <iostream>
#include <istream>
#include <string>

namespace kiselev
{
  struct DataStruct
  {
    unsigned long long key1;
    char key2;
    std::string key3;
  };
  struct DelimeterIO
  {
    char exp;
  };

  struct DelimetersIO
  {
    std::string exp;
  };
  struct UllIO
  {
    unsigned long long& ref;
  };
  struct CharIO
  {
    char& ref;
  };
  struct StringIO
  {
    std::string& ref;
  };
  struct KeyIO
  {
    DataStruct data;
    int key;
  };
  std::istream& operator>>(std::istream& input, DelimeterIO&& dest);
  std::istream& operator>>(std::istream& input, DelimetersIO&& dest);
  std::istream& operator>>(std::istream& input, UllIO&& dest);
  std::istream& operator>>(std::istream& input, CharIO&& dest);
  std::istream& operator>>(std::istream& input, StringIO&& dest);
  std::istream& operator>>(std::istream& input, KeyIO&& dest);
  std::istream& operator>>(std::istream& input, DataStruct& dest);
  std::ostream& operator<<(std::ostream& output, CharIO&& dest);
  std::ostream& operator<<(std::ostream& output, StringIO&& dest);
  std::ostream& operator<<(std::ostream& output, DataStruct& dest);
}
#endif
