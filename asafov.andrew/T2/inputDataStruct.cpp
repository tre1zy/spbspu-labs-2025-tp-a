#include "datastruct.h"
#include <cctype>
#include <algorithm>

namespace
{
  unsigned long long parseULLBin(std::istream& is)
  {
    std::streampos start = is.tellg();
    char ch;
    if (!(is >> ch) || ch != '0' || !(is >> ch) || ch != 'b')
    {
      is.seekg(start);
      return 0;
    }

    unsigned long long result = 0;
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

    if (!has_digits)
    {
      is.seekg(start);
      return 0;
    }
    return result;
  }

  std::complex< double > parseCmpLsp(std::istream& is)
  {
    std::streampos start = is.tellg();
    char ch;
    if (!(is >> ch) || ch != '#' || !(is >> ch) || ch != 'c' || !(is >> ch) || ch != '(')
    {
      is.seekg(start);
      return {0.0, 0.0};
    }

    double real = 0.0;
    double imag = 0.0;
    is >> real;
    if (!is)
    {
      is.seekg(start);
      return {0.0, 0.0};
    }

    is >> ch;
    if (ch != ')')
    {
      is.putback(ch);
      is >> imag;
      if (!is)
      {
        is.seekg(start);
        return {0.0, 0.0};
      }
      is >> ch;
      if (ch != ')')
      {
        is.seekg(start);
        return {0.0, 0.0};
      }
    }

    return {real, imag};
  }
}

std::istream& asafov::operator>>(std::istream& is, DataStruct& data)
{
  DataStruct temp;
  bool has_key1 = false;
  bool has_key2 = false;
  bool has_key3 = false;

  char ch;
  while (is.get(ch))
  {
    if (ch == ':')
    {
      std::string key;
      while (is.get(ch) && ch != ' ' && ch != ':')
      {
        key.push_back(ch);
      }

      if (key == "key1")
      {
        std::streampos before_key = is.tellg();
        unsigned long long value = parseULLBin(is);
        if (value != 0)
        {
          temp.key1 = value;
          has_key1 = true;
        }
        else
        {
          is.clear();
          is.seekg(before_key);
          char prefix[3];
          if (is.read(prefix, 3) && prefix[0] == '0' && prefix[1] == 'b' && prefix[2] == '0')
          {
            temp.key1 = 0;
            has_key1 = true;
          }
          else
          {
            is.seekg(before_key);
          }
        }
      }
      else if (key == "key2")
      {
        std::streampos before_key = is.tellg();
        std::complex< double > value = parseCmpLsp(is);
        if (value != std::complex< double >{0.0, 0.0})
        {
          temp.key2 = value;
          has_key2 = true;
        }
        else
        {
          is.clear();
          is.seekg(before_key);
          std::string prefix;
          char c;
          while (is.get(c) && prefix.size() < 6)
          {
            prefix.push_back(c);
            if (prefix == "#c(0.0 0.0)" ||
              prefix == "#c(0 0)" ||
              prefix == "#c(0. 0.)")
            {
              temp.key2 = {0.0, 0.0};
              has_key2 = true;
              break;
            }
          }
          if (!has_key2)
          {
            is.seekg(before_key);
          }
        }
      }
      else if (key == "key3")
      {
        if (is >> std::ws && is.peek() == '"')
        {
          is.get();
          std::string value;
          while (is.get(ch) && ch != '"')
          {
            value.push_back(ch);
          }
          if (ch == '"')
          {
            temp.key3 = value;
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
      temp = DataStruct();
      has_key1 = has_key2 = has_key3 = false;
    }
  }

  if (has_key1 && has_key2 && has_key3)
  {
    data = temp;
    return is;
  }

  is.setstate(std::ios::failbit);
  return is;
}
