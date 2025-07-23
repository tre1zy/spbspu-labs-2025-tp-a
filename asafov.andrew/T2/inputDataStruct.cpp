#include "datastruct.h"
#include <iomanip>
#include <string>

std::istream& asafov::operator>>(std::istream& in, asafov::DataStruct& data)
{
  DataStruct temp;
  in >> std::noskipws;
  char ch;
  in >> ch;
  if (ch != '(')
  {
    in.setstate(std::ios::failbit); std::string line; getline(in, line);
    return in;
  }
  in >> ch;
  if (ch != ':')
  {
    in.setstate(std::ios::failbit); std::string line; getline(in, line);
    return in;
  }
  in >> ch;
  if (ch != 'k')
  {
    in.setstate(std::ios::failbit); std::string line; getline(in, line);
    return in;
  }
  in >> ch;
  if (ch != 'e')
  {
    in.setstate(std::ios::failbit); std::string line; getline(in, line);
    return in;
  }
  in >> ch;
  if (ch != 'y')
  {
    in.setstate(std::ios::failbit); std::string line; getline(in, line);
    return in;
  }
  in >> ch;
  if (ch == '1')
  {
    in >> ch;
    if (ch != ' ')
    {
      in.setstate(std::ios::failbit); std::string line; getline(in, line);
      return in;
    }
    in >> ch;
    if (ch != '0')
    {
      in.setstate(std::ios::failbit); std::string line; getline(in, line);
      return in;
    }
    in >> ch;
    if (ch != 'b')
    {
      in.setstate(std::ios::failbit); std::string line; getline(in, line);
      return in;
    }
    std::string t2;
    while (ch != ':')
    {
      t2 += ch; in >> ch;
    }
    temp.key1 = std::stoull(t2);
  }
  else if (ch == '2')
  {
    double t3, t4;
    in >> ch;
    if (ch != ' ')
    {
      in.setstate(std::ios::failbit); std::string line; getline(in, line);
      return in;
    }
    in >> ch;
    if (ch != '#')
    {
      in.setstate(std::ios::failbit); std::string line; getline(in, line);
      return in;
    }
    in >> ch;
    if (ch != 'c')
    {
      in.setstate(std::ios::failbit); std::string line; getline(in, line);
      return in;
    }
    in >> ch;
    if (ch != '(')
    {
      in.setstate(std::ios::failbit); std::string line; getline(in, line);
      return in;
    }
    in >> t3;
    in >> ch;
    if (ch != ' ')
    {
      in.setstate(std::ios::failbit); std::string line; getline(in, line);
      return in;
    }
    in >> t4;
    in >> ch;
    if (ch != ')')
    {
      in.setstate(std::ios::failbit); std::string line; getline(in, line);
      return in;
    }
    temp.key2 = std::complex<double>(t3, t4);
  }
  else if (ch == '3')
  {
    in >> ch;
    if (ch != ' ')
    {
      in.setstate(std::ios::failbit); std::string line; getline(in, line);
      return in;
    }
    in >> ch;
    if (ch != '"')
    {
      in.setstate(std::ios::failbit); std::string line; getline(in, line);
      return in;
    }
    std::string t2;
    while (ch != ':')
    {
      t2 += ch; in >> ch;
    }
    t2.pop_back();
    temp.key3 = t2;
  }
  else
  {
    in.setstate(std::ios::failbit); std::string line; getline(in, line);
    return in;
  }
  in >> ch;
  if (ch != 'k')
  {
    in.setstate(std::ios::failbit); std::string line; getline(in, line);
    return in;
  }
  in >> ch;
  if (ch != 'e')
  {
    in.setstate(std::ios::failbit); std::string line; getline(in, line);
    return in;
  }
  in >> ch;
  if (ch != 'y')
  {
    in.setstate(std::ios::failbit); std::string line; getline(in, line);
    return in;
  }
  in >> ch;
  if (ch == '1')
  {
    in >> ch;
    if (ch != ' ')
    {
      in.setstate(std::ios::failbit); std::string line; getline(in, line);
      return in;
    }
    in >> ch;
    if (ch != '0')
    {
      in.setstate(std::ios::failbit); std::string line; getline(in, line);
      return in;
    }
    in >> ch;
    if (ch != 'b')
    {
      in.setstate(std::ios::failbit); std::string line; getline(in, line);
      return in;
    }
    std::string t2;
    while (ch != ':')
    {
      t2 += ch; in >> ch;
    }
    temp.key1 = std::stoull(t2);
  }
else if (ch == '2')
  {
    double t3, t4;
    in >> ch;
    if (ch != ' ')
    {
      in.setstate(std::ios::failbit); std::string line; getline(in, line);
      return in;
    }
    in >> ch;
    if (ch != '#')
    {
      in.setstate(std::ios::failbit); std::string line; getline(in, line);
      return in;
    }
    in >> ch;
    if (ch != 'c')
    {
      in.setstate(std::ios::failbit); std::string line; getline(in, line);
      return in;
    }
    in >> ch;
    if (ch != '(')
    {
      in.setstate(std::ios::failbit); std::string line; getline(in, line);
      return in;
    }
    in >> t3;
    in >> ch;
    if (ch != ' ')
    {
      in.setstate(std::ios::failbit); std::string line; getline(in, line);
      return in;
    }
    in >> t4;
    in >> ch;
    if (ch != ')')
    {
      in.setstate(std::ios::failbit); std::string line; getline(in, line);
      return in;
    }
    temp.key2 = std::complex<double>(t3, t4);
  }
  else if (ch == '3')
  {
    in >> ch;
    if (ch != ' ')
    {
      in.setstate(std::ios::failbit); std::string line; getline(in, line);
      return in;
    }
    in >> ch;
    if (ch != '"')
    {
      in.setstate(std::ios::failbit); std::string line; getline(in, line);
      return in;
    }
    std::string t2;
    while (ch != ':')
    {
      t2 += ch; in >> ch;
    }
    t2.pop_back();
    temp.key3 = t2;
  }
  else
  {
    in.setstate(std::ios::failbit); std::string line; getline(in, line);
    return in;
  }
  in >> ch;
  if (ch != 'k')
  {
    in.setstate(std::ios::failbit); std::string line; getline(in, line);
    return in;
  }
  in >> ch;
  if (ch != 'e')
  {
    in.setstate(std::ios::failbit); std::string line; getline(in, line);
    return in;
  }
  in >> ch;
  if (ch != 'y')
  {
    in.setstate(std::ios::failbit); std::string line; getline(in, line);
    return in;
  }
  in >> ch;
  if (ch == '1')
  {
    in >> ch;
    if (ch != ' ')
    {
      in.setstate(std::ios::failbit); std::string line; getline(in, line);
      return in;
    }
    in >> ch;
    if (ch != '0')
    {
      in.setstate(std::ios::failbit); std::string line; getline(in, line);
      return in;
    }
    in >> ch;
    if (ch != 'b')
    {
      in.setstate(std::ios::failbit); std::string line; getline(in, line);
      return in;
    }
    std::string t2;
    while (ch != ':')
    {
      t2 += ch; in >> ch;
    }
    temp.key1 = std::stoull(t2);
  }
  else if (ch == '2')
  {
    double t3, t4;
    in >> ch;
    if (ch != ' ')
    {
      in.setstate(std::ios::failbit); std::string line; getline(in, line);
      return in;
    }
    in >> ch;
    if (ch != '#')
    {
      in.setstate(std::ios::failbit); std::string line; getline(in, line);
      return in;
    }
    in >> ch;
    if (ch != 'c')
    {
      in.setstate(std::ios::failbit); std::string line; getline(in, line);
      return in;
    }
    in >> ch;
    if (ch != '(')
    {
      in.setstate(std::ios::failbit); std::string line; getline(in, line);
      return in;
    }
    in >> t3;
    in >> ch;
    if (ch != ' ')
    {
      in.setstate(std::ios::failbit); std::string line; getline(in, line);
      return in;
    }
    in >> t4;
    in >> ch;
    if (ch != ')')
    {
      in.setstate(std::ios::failbit); std::string line; getline(in, line);
      return in;
    }
    temp.key2 = std::complex<double>(t3, t4);
  }
  else if (ch == '3')
  {
    in >> ch;
    if (ch != ' ')
    {
      in.setstate(std::ios::failbit); std::string line; getline(in, line);
      return in;
    }
    in >> ch;
    if (ch != '"')
    {
      in.setstate(std::ios::failbit); std::string line; getline(in, line);
      return in;
    }
    std::string t2;
    while (ch != ':')
    {
      t2 += ch; in >> ch;
    }
    t2.pop_back();
    temp.key3 = t2;
  }
  else
  {
    in.setstate(std::ios::failbit); std::string line; getline(in, line);
    return in;
  }
  in >> ch;
  if (ch != ')')
  {
    in.setstate(std::ios::failbit); std::string line; getline(in, line);
    return in;
  }
  data = temp;
  std::string line;
  getline(in, line);
  return in;
}
