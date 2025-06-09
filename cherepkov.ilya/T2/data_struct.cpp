#include "data_struct.hpp"
#include "stream_guard.hpp"
#include "input_operators.hpp"

std::istream& cherepkov::operator>>(std::istream& in, DataStruct& dest)
{
    std::istream::sentry sentry(in);
    if (!sentry)
    {
        return in;
    }

    DataStruct temp;
    bool has_key1 = false, has_key2 = false, has_key3 = false;

    in >> DelimiterIO{ '(' } >> DelimiterIO{ ':' };

    for (size_t i = 0; i < 3; ++i)
    {
        std::string key;
        in >> key;

        if (key == "key1" && !has_key1)
        {
            in >> UllLitValue{ temp.key1 };
            has_key1 = true;
        }
        else if (key == "key2" && !has_key2)
        {
            in >> UllBinValue{ temp.key2 };
            has_key2 = true;
        }
        else if (key == "key3" && !has_key3)
        {
            in >> StringValue{ temp.key3 };
            has_key3 = true;
        }
        else
        {
            in.setstate(std::ios::failbit);
            return in;
        }

        in >> DelimiterIO{ ':' };
    }

    in >> DelimiterIO{ ')' };

    if (in && has_key1 && has_key2 && has_key3)
    {
        dest = std::move(temp);
    }
    else
    {
        in.setstate(std::ios::failbit);
    }

    return in;
}

std::string convertToBinary(unsigned long long val)
{
  if (val == 0)
  {
    return "0";
  }
  std::string bin;
  while (val > 0)
  {
    bin.insert(0, std::to_string(val % 2));
    val /= 2;
  }
  return bin;
}

std::ostream& cherepkov::operator<<(std::ostream& out, const DataStruct& src)
{
  std::ostream::sentry sentry(out);
  if (!sentry)
  {
    return out;
  }
  cherepkov::StreamGuard guard(out);

  out << "(:";
  out << "key1 " << src.key1 << "ull" << ":";
  out << "key2 0b" << (src.key2 == 0 ? "" : "0") << convertToBinary(src.key2);
  out << ":key3 \"" << src.key3;
  out << "\":)";

  return out;
}

bool cherepkov::operator<(const DataStruct& lhs, const DataStruct& rhs)
{
  if (lhs.key1 == rhs.key1)
  {
    if (lhs.key2 == rhs.key2)
    {
      return lhs.key3.length() < rhs.key3.length();
    }
    return lhs.key2 < rhs.key2;
  }
  return lhs.key1 < rhs.key1;
}
