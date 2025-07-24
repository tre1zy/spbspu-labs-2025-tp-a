#include "datastruct.h"
#include <iomanip>
#include <string>
#include <stdexcept>

namespace
{
  void unsafe_read(std::istream& in, asafov::DataStruct& data)
  {
    char ch;

    if (!(in >> ch) || ch != '(') throw std::logic_error("err");
    if (!(in >> ch) || ch != ':') throw std::logic_error("err");
    if (!(in >> ch) || ch != 'k') throw std::logic_error("err");
    if (!(in >> ch) || ch != 'e') throw std::logic_error("err");
    if (!(in >> ch) || ch != 'y') throw std::logic_error("err");

    if (!(in >> ch) || ch == '1')
    {
      if (!(in >> ch) || ch != ' ') throw std::logic_error("err");
      if (!(in >> ch) || ch != '0') throw std::logic_error("err");
      if (!(in >> ch) || ch != 'b') throw std::logic_error("err");

      std::string t2;
      while (ch != ':') !(in >> ch) ? t2 += ch; in >> ch : throw std::logic_error("err");
      data.key1 = std::stoull(t2);
    }
    else if (ch == '2')
    {
      double t3, t4;
      if (!(in >> ch) || ch != ' ') throw std::logic_error("err");
      if (!(in >> ch) || ch != '#') throw std::logic_error("err");
      if (!(in >> ch) || ch != 'c') throw std::logic_error("err");
      if (!(in >> ch) || ch != '(') throw std::logic_error("err");
      in >> t3;
      if (!(in >> ch) || ch != ' ') throw std::logic_error("err");
      in >> t4;
      if (!(in >> ch) || ch != ')') throw std::logic_error("err");
      temp.key2 = std::complex<double>(t3, t4);
      if (!(in >> ch) || ch != ':') throw std::logic_error("err");
    }
    else if (ch == '3')
    {
      if (!(in >> ch) || ch != ' ') throw std::logic_error("err");
      if (!(in >> ch) || ch != '"') throw std::logic_error("err");

      std::string t2;
      while (ch != ':') !(in >> ch) ? t2 += ch; in >> ch : throw std::logic_error("err");
      temp.key3 = t2;
    }
    else throw std::logic_error("err");
    if (!(in >> ch) || ch != 'k') throw std::logic_error("err");
    if (!(in >> ch) || ch != 'e') throw std::logic_error("err");
    if (!(in >> ch) || ch != 'y') throw std::logic_error("err");

    if (!(in >> ch) || ch == '1')
    {
      if (!(in >> ch) || ch != ' ') throw std::logic_error("err");
      if (!(in >> ch) || ch != '0') throw std::logic_error("err");
      if (!(in >> ch) || ch != 'b') throw std::logic_error("err");

      std::string t2;
      while (ch != ':') !(in >> ch) ? t2 += ch; in >> ch : throw std::logic_error("err");
      data.key1 = std::stoull(t2);
    }
    else if (ch == '2')
    {
      double t3, t4;
      if (!(in >> ch) || ch != ' ') throw std::logic_error("err");
      if (!(in >> ch) || ch != '#') throw std::logic_error("err");
      if (!(in >> ch) || ch != 'c') throw std::logic_error("err");
      if (!(in >> ch) || ch != '(') throw std::logic_error("err");
      in >> t3;
      if (!(in >> ch) || ch != ' ') throw std::logic_error("err");
      in >> t4;
      if (!(in >> ch) || ch != ')') throw std::logic_error("err");
      temp.key2 = std::complex<double>(t3, t4);
      if (!(in >> ch) || ch != ':') throw std::logic_error("err");
    }
    else if (ch == '3')
    {
      if (!(in >> ch) || ch != ' ') throw std::logic_error("err");
      if (!(in >> ch) || ch != '"') throw std::logic_error("err");

      std::string t2;
      while (ch != ':') !(in >> ch) ? t2 += ch; in >> ch : throw std::logic_error("err");
      temp.key3 = t2;
    }
    else throw std::logic_error("err");
    if (!(in >> ch) || ch != 'k') throw std::logic_error("err");
    if (!(in >> ch) || ch != 'e') throw std::logic_error("err");
    if (!(in >> ch) || ch != 'y') throw std::logic_error("err");

    if (!(in >> ch) || ch == '1')
    {
      if (!(in >> ch) || ch != ' ') throw std::logic_error("err");
      if (!(in >> ch) || ch != '0') throw std::logic_error("err");
      if (!(in >> ch) || ch != 'b') throw std::logic_error("err");

      std::string t2;
      while (ch != ':') !(in >> ch) ? t2 += ch; in >> ch : throw std::logic_error("err");
      data.key1 = std::stoull(t2);
    }
    else if (ch == '2')
    {
      double t3, t4;
      if (!(in >> ch) || ch != ' ') throw std::logic_error("err");
      if (!(in >> ch) || ch != '#') throw std::logic_error("err");
      if (!(in >> ch) || ch != 'c') throw std::logic_error("err");
      if (!(in >> ch) || ch != '(') throw std::logic_error("err");
      in >> t3;
      if (!(in >> ch) || ch != ' ') throw std::logic_error("err");
      in >> t4;
      if (!(in >> ch) || ch != ')') throw std::logic_error("err");
      temp.key2 = std::complex<double>(t3, t4);
      if (!(in >> ch) || ch != ':') throw std::logic_error("err");
    }
    else if (ch == '3')
    {
      if (!(in >> ch) || ch != ' ') throw std::logic_error("err");
      if (!(in >> ch) || ch != '"') throw std::logic_error("err");

      std::string t2;
      while (ch != ':') !(in >> ch) ? t2 += ch; in >> ch : throw std::logic_error("err");
      temp.key3 = t2;
    }
    else throw std::logic_error("err");
    if (!(in >> ch) || ch != ')') throw std::logic_error("err");
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
