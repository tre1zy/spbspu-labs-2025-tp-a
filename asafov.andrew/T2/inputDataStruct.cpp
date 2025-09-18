#include "datastruct.h"
#include <stdexcept>
#include <initializer_list>

namespace
{
  void expect(std::istream& in, std::initializer_list<char> expected)
  {
    char ch;
    for (char e : expected)
    {
      if (!(in >> ch) || ch != e)
      {
        in.setstate(std::ios::failbit);
      }
    }
  }

  std::complex<double> read_complex(std::istream& in)
  {
    double real, imag;
    expect(in, {' ', '#', 'c', '('});
    if (!(in >> real))
    {
      in.setstate(std::ios::failbit);
    }
    expect(in, {' '});
    if (!(in >> imag))
    {
      in.setstate(std::ios::failbit);
    }
    expect(in, {')', ':'});
    return {real, imag};
  }

  unsigned long long read_binary(std::istream& in)
  {
    char ch = 0;
    expect(in, {' ', '0', 'b'});

    std::string t2;
    while (true)
    {
      if (!(in >> ch))
      {
        in.setstate(std::ios::failbit);
      }
      if (ch == ':')
      {
        break;
      }
      t2 += ch;
    }
    unsigned long long res = 0;
    try
    {
      res = std::stoull(t2);
    }
    catch (...)
    {
      in.setstate(std::ios::failbit);
    }
    return res;
  }

  std::string read_string(std::istream& in)
  {
    char ch;
    expect(in, {' ', '"'});

    std::string t2;
    bool inside_quotes = true;
    while (inside_quotes)
    {
      if (!(in >> ch))
      {
        in.setstate(std::ios::failbit);
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
    expect(in, {':'});
    return t2;
  }

  void read_key(std::istream& in, asafov::DataStruct& data)
  {
    char ch;
    expect(in, {'k', 'e', 'y'});

    if (!(in >> ch) || (ch != '1' && ch != '2' && ch != '3'))
    {
      in.setstate(std::ios::failbit);
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
  }
}

std::istream& asafov::operator>>(std::istream& in, asafov::DataStruct& data)
{
  DataStruct temp;
  in >> std::noskipws;
  expect(in, {'(', ':'});
  read_key(in, data);
  read_key(in, data);
  read_key(in, data);
  expect(in, {')'});
  if (in.good())
  {
    data = temp;
  }
  return in;
}
