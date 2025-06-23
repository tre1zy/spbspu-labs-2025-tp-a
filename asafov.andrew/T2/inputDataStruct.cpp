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
    if (!(is >> real))
    {
      is.clear();
      while (is && is.get() != ')')
      {
      }
      return {0.0, 0.0};
    }

    char sep;
    if (!(is >> sep))
      return {0.0, 0.0};

    if (sep == ')')
      return {real, 0.0};

    double imag = 0.0;
    if (!(is >> imag))
    {
      is.clear();
      while (is && is.get() != ')')
      {
      }
      return {0.0, 0.0};
    }

    if (!(is >> sep) || sep != ')')
      return {0.0, 0.0};

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
        char prefix[2];
        if (is.read(prefix, 2) && prefix[0] == '0' && prefix[1] == 'b')
        {
          unsigned long long value = parseULLBin(is);
          if (value != 0 || (is.peek() == '0'))
          {
            temp.key1 = value;
            has_key1 = true;
          }
        }
        else
        {
          for (int i = is.gcount() - 1; i >= 0; --i)
            is.putback(prefix[i]);
        }
      }
      else if (key == "key2")
      {
        std::complex< double > value = parseCmpLsp(is);
        if (value != std::complex< double >{0.0, 0.0} ||
          (is.peek() == '#' && is.get() == '#' && is.peek() == 'c' && is.get() == 'c' && is.peek() == '(' && is.get() ==
            '('))
        {
          temp.key2 = value;
          has_key2 = true;
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
