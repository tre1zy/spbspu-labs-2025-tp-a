#include "dataStruct.hpp"
#include "streamGuard.hpp"
#include <sstream>
#include <iomanip>
#include <cctype>

namespace cherkasov
{
  bool DataStruct::operator<(const DataStruct& other) const 
  {
    if (std::abs(key1) != std::abs(other.key1))
    {
      return std::abs(key1) < std::abs(other.key1);
    }
    if (key2 != other.key2)
    {
      return key2 < other.key2;
    }
    return key3.size() < other.key3.size();
  }
  std::istream& operator>>(std::istream& in, ExpectChar&& d)
  {
    char c;
    if (!(in >> c) || c != d.exp)
    {
      in.setstate(std::ios::failbit);
    }
    return in;
  }

  std::istream& operator>>(std::istream& in, Complex&& io)
  {
    StreamGuard g(in);
    std::istream::sentry s(in);
    if (!s)
    {
      return in;
    }
    in >> ExpectChar{'#'} >> ExpectChar{'c'} >> ExpectChar{'('};
    if (!in)
    {
      return in;
    }
    double re = 0.0;
    in >> re;
    if (!in)
    {
      return in;
    }
    while (std::isspace(in.peek()))
    {
      in.get();
    }
    double im = 0.0;
    in >> im;
    if (!in)
    {
      return in;
    }
    in >> ExpectChar{')'};
    if (in)
    {
      io.c = std::complex<double>(re, im);
    }
    return in;
  }

  std::istream& operator>>(std::istream& in, Rational&& io)
  {
    StreamGuard guard(in);
    std::istream::sentry s(in);
    if (!s)
    {
      return in;
    }
    in >> ExpectChar{'('} >> ExpectChar{':'} >> ExpectChar{'N'};
    if (!in)
    {
      return in;
    }
    while (std::isspace(in.peek()))
    {
      in.get();
    }
    in >> io.rat.first;
    if (!in)
    {
      return in;
    }
    in >> ExpectChar{':'} >> ExpectChar{'D'};
    if (!in)
    {
      return in;
    }
    while (std::isspace(in.peek()))
    {
      in.get();
    }
    in >> io.rat.second;
    if (io.rat.second == 0)
    {
      in.setstate(std::ios::failbit);
      return in;
    }
    in >> ExpectChar{':'} >> ExpectChar{')'};
    return in;
  }

  std::istream& operator>>(std::istream& in, Strings&& io)
  {
    StreamGuard guard(in);
    std::istream::sentry s(in);
    if (!s)
    {
      return in;
    }
    in >> std::noskipws >> ExpectChar{'"'};
    if (!in)
    {
      return in;
    }
    char ch = 0;
    io.s.clear();
    bool escape = false;
    while (in >> ch)
    {
      if (escape)
      {
        io.s += ch;
        escape = false;
      }
      else if (ch == '\\')
      {
        escape = true;
      }
      else if (ch == '"')
      {
        break;
      }
      else
      {
        io.s += ch;
      }
    }
    if (ch != '"')
    {
      in.setstate(std::ios::failbit);
    }
    in >> std::skipws;
    return in;
  }

  std::istream& operator>>(std::istream& in, DataStruct& obj)
  {
    std::istream::sentry s(in);
    if (!s)
    {
      return in;
    }
    DataStruct temp;
    bool hasKey1 = false;
    bool hasKey2 = false;
    bool hasKey3 = false;
    in >> ExpectChar{'('} >> ExpectChar{':'};
    while (in && in.peek() != ')')
    {
      std::string label;
      char c;
      while (in.get(c) && c != ':' && !std::isspace(c))
      {
        label += c;
      }
      if (c != ':')
      {
        while (in.get(c) && c != ':') {}
      }
      if (label == "key1")
      {
        in >> Complex{temp.key1};
        hasKey1 = bool(in);
      }
      else if (label == "key2")
      {
        in >> Rational{temp.key2};
        hasKey2 = bool(in);
      }
      else if (label == "key3")
      {
        in >> Strings{temp.key3};
        hasKey3 = bool(in);
      }
      else
      {
        std::string dummy;
        std::getline(in, dummy, ':');
      }
      while (std::isspace(in.peek()))
      {
        in.get();
      }
    }
    in >> ExpectChar{')'};
    if (in && hasKey1 && hasKey2 && hasKey3)
    {
      obj = temp;
    }
    else
    {
      in.setstate(std::ios::failbit);
    }
    return in;
  }

  std::ostream& operator<<(std::ostream& out, const DataStruct& obj)
  {
    StreamGuard guard(out);
    out << "(:key1 #c(" << std::fixed << std::setprecision(1);
    out << obj.key1.real();
    if (obj.key1.imag() >= 0)
    {
      out << "+";
    }
    out << obj.key1.imag() << ")";
    out << ":key2 (:N " << obj.key2.first << ":D " << obj.key2.second << ":)";
    out << ":key3 \"" << obj.key3 << "\":)";
    return out;
  }
}
