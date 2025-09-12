#include "dataStruct.hpp"
#include "streamGuard.hpp"
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

  std::istream& operator>>(std::istream& in, DelimiterIO&& obj)
  {
    std::istream::sentry sentry(in);
    if (!sentry)
    {
      return in;
    }
    char c = 0;
    in >> c;
    if (in && std::tolower(c) != obj.exp)
    {
      in.setstate(std::ios::failbit);
    }
    return in;
  }

  std::istream& operator>>(std::istream& in, ComplexIO&& obj)
  {
    std::istream::sentry sentry(in);
    if (!sentry)
    {
      return in;
    }
    in >> DelimiterIO{'#'} >> DelimiterIO{'c'} >> DelimiterIO{'('};
    double real = 0.0, imag = 0.0;
    in >> real >> imag;
    in >> DelimiterIO{')'};
    if (in)
    {
      obj.ref = std::complex<double>(real, imag);
    }
    return in;
  }

  std::istream& operator>>(std::istream& in, RationalIO&& obj)
  {
    std::istream::sentry sentry(in);
    if (!sentry)
    {
      return in;
    }
    in >> DelimiterIO{'('} >> DelimiterIO{':'} >> DelimiterIO{'N'};
    long long numerator = 0;
    in >> numerator;
    in >> DelimiterIO{':'} >> DelimiterIO{'D'};
    unsigned long long denominator = 0;
    in >> denominator;
    in >> DelimiterIO{':'} >> DelimiterIO{')'};
    if (in)
    {
      obj.ref = std::make_pair(numerator, denominator);
    }
    return in;
  }

  std::istream& operator>>(std::istream& in, StringIO&& obj)
  {
    std::istream::sentry sentry(in);
    if (!sentry)
    {
      return in;
    }
    in >> DelimiterIO{'"'};
    std::getline(in, obj.ref, '"');
    return in;
  }

  std::istream& operator>>(std::istream& in, DataStruct& obj)
  {
    std::istream::sentry sentry(in);
    if (!sentry)
    {
      return in;
    }
    DataStruct tmp;
    bool hasKey1 = false, hasKey2 = false, hasKey3 = false;
    try
    {
      in >> DelimiterIO{'('};
      for (int i = 0; i < 3; ++i)
      {
        std::string label;
        in >> DelimiterIO{':'} >> label;
        if (label == "key1" && !hasKey1)
        {
          in >> ComplexIO{tmp.key1};
          hasKey1 = true;
        }
        else if (label == "key2" && !hasKey2)
        {
          in >> RationalIO{tmp.key2};
          hasKey2 = true;
        }
        else if (label == "key3" && !hasKey3)
        {
          in >> StringIO{tmp.key3};
          hasKey3 = true;
        }
        else
        {
          throw std::runtime_error("Invalid or duplicate key");
        }
      }
      in >> DelimiterIO{':'} >> DelimiterIO{')'};
      if (!hasKey1 || !hasKey2 || !hasKey3)
      {
        throw std::runtime_error("Missing required keys");
      }
    }
    catch (...)
    {
      in.setstate(std::ios::failbit);
    }
    if (in)
    {
      obj = tmp;
    }
    return in;
  }

  std::ostream& operator<<(std::ostream& out, const DataStruct& obj)
  {
    std::ostream::sentry sentry(out);
    if (!sentry)
    {
      return out;
    }
    StreamGuard guard(out);
    out << "(:";
    out << "key1 #c(" << std::fixed << std::setprecision(1) 
        << obj.key1.real() << " " << obj.key1.imag() << ")";
    out << ":";
    out << "key2 (:N " << obj.key2.first << ":D " << obj.key2.second << ":)";
    out << ":";
    out << "key3 \"" << obj.key3 << "\"";
    out << ":)";
    return out;
  }
}