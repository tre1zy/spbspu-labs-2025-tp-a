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

  std::istream& operator>>(std::istream& in, DataStruct& value)
  {
    std::istream::sentry sentry(in);
    if (!sentry)
    {
      return in;
    }

    char ch;
    if (!(in >> ch) || ch != '(')
    {
      in.setstate(std::ios::failbit);
      return in;
    }

    bool hasKey1 = false;
    bool hasKey2 = false;
    bool hasKey3 = false;
    std::complex<double> tmp1{};
    std::pair<long long, unsigned long long> tmp2{};
    std::string tmp3{};

    while (in)
    {
      if (!(in >> ch) || ch != ':')
      {
        in.setstate(std::ios::failbit);
        return in;
      }

      std::string key;
      if (!(in >> key))
      {
        in.setstate(std::ios::failbit);
        return in;
      }

      if (key == "key1")
      {
        std::string mark;
        if (!(in >> mark) || mark != "#c")
        {
          in.setstate(std::ios::failbit);
          return in;
        }
        if (!(in >> ch) || ch != '(')
        {
          in.setstate(std::ios::failbit);
          return in;
        }
        double re{}, im{};
        if (!(in >> re >> im))
        {
          in.setstate(std::ios::failbit);
          return in;
        }
        if (!(in >> ch) || ch != ')')
        {
          in.setstate(std::ios::failbit);
          return in;
        }
        tmp1 = {re, im};
        hasKey1 = true;
      }
      else if (key == "key2")
      {
        if (!(in >> ch) || ch != '(')
        {
          in.setstate(std::ios::failbit);
          return in;
        }
        std::string Nmark;
        if (!(in >> Nmark) || Nmark != "N")
        {
          in.setstate(std::ios::failbit);
          return in;
        }
        long long num{};
        if (!(in >> num))
        {
          in.setstate(std::ios::failbit);
          return in;
        }
        if (!(in >> ch) || ch != ':')
        {
          in.setstate(std::ios::failbit);
          return in;
        }
        std::string Dmark;
        if (!(in >> Dmark) || Dmark != "D")
        {
          in.setstate(std::ios::failbit);
          return in;
        }
        unsigned long long den{};
        if (!(in >> den))
        {
          in.setstate(std::ios::failbit);
          return in;
        }
        if (!(in >> ch) || ch != ':')
        {
          in.setstate(std::ios::failbit);
          return in;
        }
        if (!(in >> ch) || ch != ')')
        {
          in.setstate(std::ios::failbit);
          return in;
        }
        tmp2 = {num, den};
        hasKey2 = true;
      }
      else if (key == "key3")
      {
        if (!(in >> ch) || ch != '"')
        {
          in.setstate(std::ios::failbit);
          return in;
        }
        std::ostringstream oss;
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
        tmp3 = oss.str();
        hasKey3 = true;
      }
      else if (key == ")")
      {
        break;
      }
      else
      {
        in.setstate(std::ios::failbit);
        return in;
      }
      std::streampos pos = in.tellg();
      if (in >> ch)
      {
        if (ch == ':' && in.peek() == ')')
        {
          in.get();
          break;
        }
        else
        {
          in.unget();
        }
      }
      else
      {
        in.clear();
        in.seekg(pos);
      }
    }

    if (hasKey1 && hasKey2 && hasKey3)
    {
      value.key1 = tmp1;
      value.key2 = tmp2;
      value.key3 = tmp3;
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
