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
    while (is.get(ch) && (ch == '0' || ch == '1'))
    {
      result = result << 1;
      if (ch == '1')
      {
        result = result | 1;
      }
    }
    if (is)
    {
      is.unget();
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
  bool has_key1 = false;
  bool has_key2 = false;
  bool has_key3 = false;

  while (is.get(ch))
  {
    if (ch == ':')
    {
      std::string key;
      while (is.get(ch) && ch != ' ' && ch != '\n')
      {
        key.push_back(ch);
      }
      if (key == "key1")
      {
        unsigned long long val = parseULLBin(is);
        if (is || is.eof())
        {
          temp.key1 = val;
          has_key1 = true;
          is.clear();
        }
      }
      else if (key == "key2")
      {
        std::complex< double > val = parseCmpLsp(is);
        if (is || is.eof())
        {
          temp.key2 = val;
          has_key2 = true;
          is.clear();
        }
      }
      else if (key == "key3")
      {
        if (is.get(ch) && ch == '"')
        {
          std::string str;
          while (is.get(ch) && ch != '"' && ch != '\n')
          {
            str.push_back(ch);
          }
          if (ch == '"')
          {
            temp.key3 = str;
            has_key3 = true;
          }
        }
      }
    }
    if (has_key1 && has_key2 && has_key3)
    {
      data = temp;
      return is;
    }
    if (ch == '\n')
    {
      break;
    }
  }

  if (!has_key1 || !has_key2 || !has_key3)
  {
    is.setstate(std::ios::failbit);
  }
  return is;
}
