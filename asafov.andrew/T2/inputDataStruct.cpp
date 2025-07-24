#include "datastruct.h"
#include <iomanip>
#include <string>
#include <stdexcept>

namespace
{
  std::complex<double> read_complex(std::istream& in)
  {
    char ch;
    double real, imag;
    for (char expected : {' ', '#', 'c', '('})
    {
      if (!(in >> ch) || ch != expected)
      {
        throw std::logic_error("err");
      }
    }
    in >> real;
    if (!(in >> ch) || ch != ' ')
    {
      throw std::logic_error("err");
    }
    in >> imag;
    for (char expected : {')', ':'})
    {
      if (!(in >> ch) || ch != expected)
      {
        throw std::logic_error("err");
      }
    }
    return {real, imag};
  }

  unsigned long long read_binary(std::istream& in)
  {
    char ch;
    for (char expected : {' ', '0', 'b'})
    {
      if (!(in >> ch) || ch != expected)
      {
        throw std::logic_error("err");
      }
    }

    std::string t2;
    while (true)
    {
      if (!(in >> ch))
      {
        throw std::logic_error("err");
      }
      if (ch == ':')
      {
        break;
      }
      t2 += ch;
    }
    return std::stoull(t2);
  }

  std::string read_string(std::istream& in)
  {
    char ch;
    for (char expected : {' ', '"'}) if (!(in >> ch) || ch != expected) throw std::logic_error("err");

    std::string t2;
    bool inside_quotes = true;
    while (inside_quotes)
    {
      if (!(in >> ch))
      {
        throw std::logic_error("err");
      }
      if (ch == '"')
      {
        inside_quotes = false;
      }
      else
      {
        t2 += ch;
      }
    }
    if (!(in >> ch) || ch != ':')
    {
      throw std::logic_error("err");
    }
    return t2;
  }

  void unsafe_read(std::istream& in, asafov::DataStruct& data)
  {
    char ch;

    for (char expected : {'(', ':', 'k', 'e', 'y'})
    {
      if (!(in >> ch) || ch != expected)
      {
        throw std::logic_error("err");
      }
    }

    if (!(in >> ch) || (ch != '1' && ch != '2' && ch != '3'))
    {
      throw std::logic_error("err");
    }

    if (ch == '1')
    {
      data.key1 = read_binary(in);
    }
    else if (ch == '2')
    {
      data.key2 = read_complex(in);
    }
    else if (ch == '3')
    {
      data.key3 = read_string(in);
    }

    for (char expected : {'k', 'e', 'y'})
    {
      if (!(in >> ch) || ch != expected)
      {
        throw std::logic_error("err");
      }
    }

    if (!(in >> ch) || (ch != '1' && ch != '2' && ch != '3'))
    {
      throw std::logic_error("err");
    }
    if (ch == '1')
    {
      data.key1 = read_binary(in);
    }
    else if (ch == '2')
    {
      data.key2 = read_complex(in);
    }
    else if (ch == '3')
    {
      data.key3 = read_string(in);
    }

    for (char expected : {'k', 'e', 'y'})
    {
      if (!(in >> ch) || ch != expected)
      {
        throw std::logic_error("err");
      }
    }

    if (!(in >> ch) || (ch != '1' && ch != '2' && ch != '3'))
    {
      throw std::logic_error("err");
    }
    if (ch == '1')
    {
      data.key1 = read_binary(in);
    }
    else if (ch == '2')
    {
      data.key2 = read_complex(in);
    }
    else if (ch == '3')
    {
      data.key3 = read_string(in);
    }

    if (!(in >> ch) || ch != ')')
    {
      throw std::logic_error("err");
    }
  }
}

std::istream& asafov::operator>>(std::istream& in, asafov::DataStruct& data)
{
  DataStruct temp;
  in >> std::noskipws;
  try
  {
    unsafe_read(in, temp);
  }
  catch (...)
  {
    in.setstate(std::ios::failbit);
    std::string line;
    getline(in, line);
    return in;
  }
  data = temp;
  std::string line;
  getline(in, line);
  return in;
}
