#include "dataStruct.hpp"
#include "streamGuard.hpp"
#include <sstream>
#include <cmath>
#include <iomanip>

namespace cherkasov
{
  bool DataStruct::operator<(const DataStruct& other) const
  {
    if (std::abs(key1) != std::abs(other.key1))
      return std::abs(key1) < std::abs(other.key1);
    if (key2 != other.key2)
      return key2 < other.key2;
    return key3.size() < other.key3.size();
  }

  std::istream& operator>>(std::istream& in, ExpectChar&& d)
  {
    char c;
    if (!(in >> c) || c != d.exp)
      in.setstate(std::ios::failbit);
    return in;
  }

  std::istream& operator>>(std::istream& in, Label&& l)
  {
    std::string tmp;
    if (!(in >> tmp) || tmp != l.exp)
      in.setstate(std::ios::failbit);
    return in;
  }

  std::istream& operator>>(std::istream& in, Complex&& io)
  {
    StreamGuard g(in);
    in >> ExpectChar{'#'} >> ExpectChar{'c'} >> ExpectChar{'('};
    double re = 0.0, im = 0.0;
    in >> re;
    if (in.peek() == '+' || in.peek() == '-')
      in >> im;
    in >> ExpectChar{')'};
    if (in) io.c = {re, im};
    return in;
  }

  std::istream& operator>>(std::istream& in, Rational&& io)
  {
    in >> ExpectChar{'('} >> ExpectChar{':'} >> Label{"N"} >> io.rat.first
       >> ExpectChar{':'} >> Label{"D"} >> io.rat.second
       >> ExpectChar{':'} >> ExpectChar{')'};
    return in;
  }

  std::istream& operator>>(std::istream& in, Strings&& io)
  {
    StreamGuard guard(in);
    in >> std::noskipws >> ExpectChar{'"'};
    char ch;
    while (in >> ch && ch != '"')
    {
      if (ch == '\n')
      {
        in.setstate(std::ios::failbit);
        break;
      }
      io.s += ch;
    }
    return in;
  }

  std::istream& operator>>(std::istream& in, DataStruct& obj)
  {
    std::istream::sentry s(in);
    if (!s) return in;
    DataStruct temp;
    bool k1 = false, k2 = false, k3 = false;
    in >> ExpectChar{'('};
    while (in)
    {
      std::string label;
      in >> ExpectChar{':'} >> label;
      if (label == "key1" && !k1)
      {
        in >> Complex{temp.key1};
        k1 = true;
      }
      else if (label == "key2" && !k2)
      {
        in >> Rational{temp.key2};
        k2 = true;
      }
      else if (label == "key3" && !k3)
      {
        in >> Strings{temp.key3};
        k3 = true;
      }
      else
      {
        in.setstate(std::ios::failbit);
        break;
      }
      if (k1 && k2 && k3) break;
    }
    in >> ExpectChar{':'} >> ExpectChar{')'};
    if (in && k1 && k2 && k3) obj = temp;
    return in;
  }

  std::ostream& operator<<(std::ostream& out, const DataStruct& obj)
  {
    StreamGuard guard(out);
    out << "(:key1 #c(" << std::fixed << std::setprecision(1) << obj.key1.real();
    if (obj.key1.imag() >= 0) out << "+";
    out << obj.key1.imag() << ")";
    out << ":key2 (:N" << obj.key2.first << ":D" << obj.key2.second << ":)";
    out << ":key3 \"" << obj.key3 << "\":)";
    return out;
  }
}
