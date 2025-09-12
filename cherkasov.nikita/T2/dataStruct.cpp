#include "dataStruct.hpp"
#include <sstream>
#include <iomanip>
#include <complex>

namespace cherkasov
{
  std::istream& operator>>(std::istream& in, ExpectChar&& exp)
  {
    char c{};
    if (!(in >> c) || c != exp.exp)
    {
      in.setstate(std::ios::failbit);
    }
    return in;
  }

  std::istream& operator>>(std::istream& in, Label&& exp)
  {
    std::string token;
    if (!(in >> token) || token != exp.exp)
    {
      in.setstate(std::ios::failbit);
    }
    return in;
  }

  std::istream& operator>>(std::istream& in, Complex&& c)
  {
    in >> Label{"#c"} >> ExpectChar{'('};
    double re{}, im{};
    if (!(in >> re >> im))
    {
      in.setstate(std::ios::failbit);
      return in;
    }
    in >> ExpectChar{')'};
    c.c = {re, im};
    return in;
  }

  std::istream& operator>>(std::istream& in, Rational&& rat)
  {
    in >> ExpectChar{'('} >> Label{"N"};
    long long num{};
    if (!(in >> num))
    {
      in.setstate(std::ios::failbit);
      return in;
    }
    in >> ExpectChar{':'} >> Label{"D"};
    unsigned long long den{};
    if (!(in >> den))
    {
      in.setstate(std::ios::failbit);
      return in;
    }
    in >> ExpectChar{':'} >> ExpectChar{')'};
    rat.rat = {num, den};
    return in;
  }

  std::istream& operator>>(std::istream& in, Strings&& s)
  {
    in >> ExpectChar{'"'};
    std::ostringstream oss;
    char ch{};
    while (in.get(ch))
    {
      if (ch == '"')
      {
        break;
      }
      if (ch == '\n')
      {
        in.setstate(std::ios::failbit);
        return in;
      }
      oss << ch;
    }
    s.s = oss.str();
    return in;
  }

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

  std::istream& operator>>(std::istream& in, DataStruct& value)
  {
    std::istream::sentry s(in);
    if (!s)
    {
      return in;
    }
    DataStruct tmp{};
    bool has1 = false, has2 = false, has3 = false;
    in >> ExpectChar{'('};
    while (in)
    {
      in >> ExpectChar{':'};
      std::string key{};
      if (!(in >> key))
      {
        in.setstate(std::ios::failbit);
        return in;
      }

      if (key == "key1")
      {
        in >> Complex{tmp.key1};
        has1 = true;
      }
      else if (key == "key2")
      {
        in >> Rational{tmp.key2};
        has2 = true;
      }
      else if (key == "key3")
      {
        in >> Strings{tmp.key3};
        has3 = true;
      }
      else
      {
        in.setstate(std::ios::failbit);
        return in;
      }

      if (in.peek() == ':')
      {
        in.get();
        if (in.peek() == ')')
        {
          in.get();
          break;
        }
      }
    }

    if (has1 && has2 && has3)
    {
      value = tmp;
    }
    else
    {
      in.setstate(std::ios::failbit);
    }
    return in;
  }

  std::ostream& operator<<(std::ostream& out, const DataStruct& value)
  {
    out << "(:key1 #c(" << std::fixed << std::setprecision(1)
        << value.key1.real() << " " << value.key1.imag() << "):";
    out << "key2 (:N " << value.key2.first << ":D " << value.key2.second << ":):";
    out << "key3 \"" << value.key3 << "\":)";
    return out;
  }
}
