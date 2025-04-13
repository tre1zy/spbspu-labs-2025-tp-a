#ifndef STRUCTURES_HPP
#define STRUCTURES_HPP
#include <string>

namespace zakirov
{
  //{:keyX 0xFFFA: :keyX 01001: :keyX 01001:}
  struct Data
  {
    unsigned long long key1;
    unsigned long long key2;
    std::string key3;
  };

  struct MinorSymbol
  {
    char symbol;
  };

  struct UllOctIO
  {
    unsigned long long & ref;
  };

  struct UllHexIO
  {
    unsigned long long & ref;
  };

  struct StringIO
  {
    std::string exp;
  };

  std::istream & operator>>(std::istream & in, Data & object);
  std::istream & operator>>(std::istream & in, MinorSymbol && object);
  std::istream & operator>>(std::istream & in, UllOctIO && object);
  std::istream & operator>>(std::istream & in, UllHexIO && object);
  std::istream & operator>>(std::istream & in, StringIO && object);
}

#endif
