#include "datastruct.h"
#include <cctype>
#include <algorithm>

namespace
{
  unsigned long long parseULLBin(std::istream& is)
  {
    char ch;
    if (!is.get(ch) || ch != '0')
    {
      is.setstate(std::ios::failbit);
      return 0;
    }
    if (!is.get(ch) || ch != 'b')
    {
      is.setstate(std::ios::failbit);
      return 0;
    }

    unsigned long long result = 0;
    std::string bits;
    while (is.get(ch) && (ch == '0' || ch == '1'))
    {
      bits.push_back(ch);
    }
    if (bits.empty())
    {
      is.setstate(std::ios::failbit);
      return 0;
    }
    if (is)
    {
      is.unget();
    }

    for (char bit: bits)
    {
      result = (result << 1) | (bit == '1');
    }
    return result;
  }

  std::complex< double > parseCmpLsp(std::istream& is)
  {
    char ch;
    if (!is.get(ch) || ch != '#')
    {
      is.setstate(std::ios::failbit);
      return {0.0, 0.0};
    }
    if (!is.get(ch) || ch != 'c')
    {
      is.setstate(std::ios::failbit);
      return {0.0, 0.0};
    }
    if (!is.get(ch) || ch != '(')
    {
      is.setstate(std::ios::failbit);
      return {0.0, 0.0};
    }

    double real = 0.0;
    double imag = 0.0;
    is >> real;
    if (!is)
    {
      return {0.0, 0.0};
    }

    if (is.get(ch) && ch != ')')
    {
      is.unget();
      is >> imag;
      if (!is)
      {
        return {0.0, 0.0};
      }
      if (!is.get(ch) || ch != ')')
      {
        is.setstate(std::ios::failbit);
        return {0.0, 0.0};
      }
    }
    return {real, imag};
  }
}

std::istream& asafov::operator>>(std::istream& is, DataStruct& data)
{
  DataStruct temp;
  char ch;

  is >> std::ws;
  if (!is.get(ch) || ch != '(')
  {
    is.setstate(std::ios::failbit);
    return is;
  }

  is >> std::ws;
  if (!is.get(ch) || ch != ':')
  {
    is.setstate(std::ios::failbit);
    return is;
  }

  bool has_key1 = false;
  bool has_key2 = false;
  bool has_key3 = false;

  while (true)
  {
    std::string key;
    is >> std::ws;

    while (is.get(ch) && ch != ' ' && ch != ':')
    {
      key += ch;
    }

    if (!is)
    {
      is.setstate(std::ios::failbit);
      return is;
    }

    if (key == "key1")
    {
      is >> std::ws;
      temp.key1 = parseULLBin(is);
      if (is.fail())
      {
        return is;
      }
      has_key1 = true;
    }
    else if (key == "key2")
    {
      is >> std::ws;
      temp.key2 = parseCmpLsp(is);
      if (is.fail())
      {
        return is;
      }
      has_key2 = true;
    }
    else if (key == "key3")
    {
      is >> std::ws;
      if (!is.get(ch) || ch != '"')
      {
        is.setstate(std::ios::failbit);
        return is;
      }

      temp.key3.clear();
      while (is.get(ch) && ch != '"')
      {
        temp.key3 += ch;
      }

      if (ch != '"')
      {
        is.setstate(std::ios::failbit);
        return is;
      }
      has_key3 = true;
    }

    is >> std::ws;
    if (!is.get(ch))
    {
      is.setstate(std::ios::failbit);
      return is;
    }

    if (ch == ')')
    {
      break;
    }
    else if (ch != ':')
    {
      is.setstate(std::ios::failbit);
      return is;
    }
  }

  if (has_key1 && has_key2 && has_key3)
  {
    data = temp;
  }
  else
  {
    is.setstate(std::ios::failbit);
  }
  return is;
}
