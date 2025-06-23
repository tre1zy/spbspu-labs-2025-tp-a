#include "datastruct.h"
#include <cctype>
#include <algorithm>
#include <cstring>
#include <complex>

namespace
{
  unsigned long long parseULLBin(std::istream& is)
  {
    char ch;
    if (!(is >> ch) || ch != '0' || !(is >> ch) || ch != 'b')
    {
      is.putback(ch);
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

    return has_digits ? result : 0;
  }

  std::complex< double > parseCmpLsp(std::istream& is)
  {
    char buffer[5] = {0};
    is.read(buffer, 3);
    if (strncmp(buffer, "#c(", 3) != 0)
    {
      for (int i = is.gcount() - 1; i >= 0; --i)
        is.putback(buffer[i]);
      return {0.0, 0.0};
    }

    double real = 0.0;
    is >> real;
    if (!is)
    {
      is.clear();
      while (is.get() != ')')
      {
      }
      return {0.0, 0.0};
    }

    char sep;
    is >> sep;
    if (sep == ')')
      return {real, 0.0};

    double imag = 0.0;
    is >> imag;
    if (!is)
    {
      is.clear();
      while (is.get() != ')')
      {
      }
      return {0.0, 0.0};
    }

    is >> sep;
    if (sep != ')')
    {
      is.putback(sep);
      return {0.0, 0.0};
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
      is.putback(ch);

      if (key == "key1")
      {
        std::string value_str;
        char next = is.peek();
        if (next == '0')
        {
          is.get(ch);
          value_str += ch;
          next = is.peek();
          if (next == 'b')
          {
            is.get(ch);
            value_str += ch;
            unsigned long long value = parseULLBin(is);
            if (value != 0 || value_str == "0b")
            {
              temp.key1 = value;
              has_key1 = true;
            }
          }
          else
          {
            is.putback(ch);
          }
        }
      }
      else if (key == "key2")
      {
        char next = is.peek();
        if (next == '#')
        {
          std::string prefix;
          for (int i = 0; i < 3; ++i)
          {
            is.get(ch);
            prefix += ch;
          }

          if (prefix == "#c(")
          {
            double real, imag = 0.0;
            is >> real;

            char sep;
            is >> sep;
            if (sep == ')')
            {
              temp.key2 = {real, 0.0};
              has_key2 = true;
            }
            else
            {
              is >> imag >> sep;
              if (sep == ')')
              {
                temp.key2 = {real, imag};
                has_key2 = true;
              }
            }
          }
          else
          {
            for (int i = prefix.size() - 1; i >= 0; --i)
              is.putback(prefix[i]);
          }
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
