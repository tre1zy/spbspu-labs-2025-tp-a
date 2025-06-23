#include "datastruct.h"
#include <cctype>
#include <algorithm>
#include <cstring>
#include <complex>

namespace
{
  bool parseULLBin(std::istream& is, unsigned long long& result)
  {
    char ch;
    if (!(is >> ch) || ch != '0') return false;
    if (!(is >> ch) || ch != 'b') return false;

    result = 0;
    bool has_digits = false;
    while (is.get(ch))
    {
      if (ch == '0' || ch == '1')
      {
        result = (result << 1) | (ch - '0');
        has_digits = true;
      }
      else
      {
        is.putback(ch);
        break;
      }
    }
    return has_digits;
  }

  bool parseCmpLsp(std::istream& is, std::complex< double >& result)
  {
    char ch;
    if (!(is >> ch) || ch != '#') return false;
    if (!(is >> ch) || ch != 'c') return false;
    if (!(is >> ch) || ch != '(') return false;

    double real, imag = 0.0;
    if (!(is >> real)) return false;

    if (!(is >> ch)) return false;
    if (ch == ')')
    {
      result = {real, 0.0};
      return true;
    }

    if (!(is >> imag)) return false;
    if (!(is >> ch) || ch != ')') return false;

    result = {real, imag};
    return true;
  }
}

std::istream& asafov::operator>>(std::istream& is, DataStruct& data)
{
  DataStruct temp;
  int keys_found = 0;
  char ch;

  while (is >> ch)
  {
    if (ch == ':')
    {
      std::string key;
      while (is.get(ch) && ch != ' ' && ch != ':')
      {
        key += ch;
      }

      if (key == "key1")
      {
        unsigned long long value;
        if (parseULLBin(is, value) || (is.peek() == '0'))
        {
          temp.key1 = value;
          keys_found |= 1;
        }
      }
      else if (key == "key2")
      {
        std::complex< double > value;
        if (parseCmpLsp(is, value))
        {
          temp.key2 = value;
          keys_found |= 2;
        }
      }
      else if (key == "key3")
      {
        is >> std::ws;
        if (is.peek() == '"')
        {
          is.get();
          std::string value;
          while (is.get(ch) && ch != '"')
          {
            value += ch;
          }
          if (ch == '"')
          {
            temp.key3 = value;
            keys_found |= 4;
          }
        }
      }
    }

    if (keys_found == 7) // 1 | 2 | 4
    {
      data = temp;
      return is;
    }

    if (ch == '\n') break;
  }

  if (keys_found == 7)
  {
    data = temp;
    return is;
  }

  is.setstate(std::ios::failbit);
  return is;
}
