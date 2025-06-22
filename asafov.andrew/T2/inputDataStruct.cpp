#include "datastruct.h"
#include <cctype>
#include <algorithm>

namespace
{
  unsigned long long parseULLBin(std::istream& is)
  {
    char ch;
    if (!(is >> ch) || ch != '0' || !(is >> ch) || ch != 'b')
    {
      return 0;
    }

    unsigned long long result = 0;
    while (is >> ch)
    {
      if (ch == '0' || ch == '1')
      {
        result = (result << 1) | (ch - '0');
      }
      else
      {
        is.putback(ch);
        break;
      }
    }
    return result;
  }

  std::complex< double > parseCmpLsp(std::istream& is)
  {
    char ch;
    if (!(is >> ch) || ch != '#' || !(is >> ch) || ch != 'c' || !(is >> ch) || ch != '(')
    {
      return {0.0, 0.0};
    }

    double real = 0.0;
    double imag = 0.0;
    is >> real;
    if (!is)
    {
      return {0.0, 0.0};
    }

    is >> ch;
    if (ch != ')')
    {
      is.putback(ch);
      is >> imag;
      if (!is)
      {
        return {0.0, 0.0};
      }
      is >> ch;
      if (ch != ')')
      {
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
  while (is >> ch)
  {
    if (ch == ':')
    {
      std::string key;
      while (is >> ch && ch != ' ' && ch != ':')
      {
        key.push_back(ch);
      }

      if (key == "key1")
      {
        unsigned long long value = parseULLBin(is);
        if (value != 0 || (is.peek() == '0' && is.get() == '0' && is.peek() ==
          'b' && is.get() == 'b' && is.peek() ==
          '0' && is.get() == '0'))
        {
          temp.key1 = value;
          has_key1 = true;
        }
      }
      else if (key == "key2")
      {
        std::complex< double > value = parseCmpLsp(is);
        if (value != std::complex< double >{0.0, 0.0} ||
          (is.peek() == '#' && is.get() == '#' && is.peek() == 'c' && is.get() ==
            'c' && is.peek() == '(' && is.get() ==
            '(' &&
            is.peek() == '0' && is.get() == '0' && is.peek() == '.' && is.get() ==
            '.' && is.peek() == '0' && is.get()
            == '0' &&
            is.peek() == ' ' && is.get() == ' ' &&
            is.peek() == '0' && is.get() == '0' && is.peek() == '.' && is.get() ==
            '.' && is.peek() == '0' && is.get()
            == '0' &&
            is.peek() == ')' && is.get() == ')'))
        {
          temp.key2 = value;
          has_key2 = true;
        }
      }
      else if (key == "key3")
      {
        if (is >> ch && ch == '"')
        {
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
  }

  if (has_key1 && has_key2 && has_key3)
  {
    data = temp;
    return is;
  }

  is.setstate(std::ios::failbit);
  return is;
}
