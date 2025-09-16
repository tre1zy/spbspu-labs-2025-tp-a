#include "dataStruct.hpp"
#include <iomanip>
#include <string>
#include <limits>
#include "streamGuard.hpp"

bool shramko::dataStruct::operator<(const dataStruct& other)
{
  if (key1 != other.key1)
  {
    return key1 < other.key1;
  }

  if (key2 != other.key2)
  {
    return key2 < other.key2;
  }

  return key3.size() < other.key3.size();
}

std::ostream& shramko::operator<<(std::ostream& out, const DoubleScienceT& x)
{
  char keyStr[32];
  std::snprintf(keyStr, sizeof(keyStr), "%.1e", x.key);
  std::string key(keyStr);
  size_t exp_pos = key.find('e');
  if (exp_pos != std::string::npos && (key[exp_pos + 2] == '0'))
  {
    key.erase(exp_pos + 2, 1);
  }

  out << key;
  return out;
}

std::ostream& shramko::operator<<(std::ostream& out, const UllBinT& x)
{
  out << "0b";
  if (x.key == 0)
  {
    out << "0";
  }
  else
  {
    out << std::string(x.prefix_zeroes, '0');
    bool started = false;
    for (int i = 63; i >= 0; --i)
    {
      bool bit = (x.key & (1ULL << i)) != 0;
      if (bit || started)
      {
        out << (bit ? '1' : '0');
        started = true;
      }
    }
  }
  return out;
}

std::ostream& shramko::operator<<(std::ostream& out, const StringT& x)
{
  out << std::quoted(x.key);
  return out;
}

std::ostream& shramko::operator<<(std::ostream& out, const dataStruct& ds)
{
  DoubleScienceT x { ds.key1 };
  UllBinT y { ds.key2, ds.key2_zeroes };
  StringT z { ds.key3 };

  StreamGuard guard(out);
  out << "(";
  out << ":key1 " << x;
  out << ":key2 " << y;
  out << ":key3 " << z;
  out << ":)";
  return out;
}

std::istream& shramko::operator>>(std::istream& in, ExpectCharT&& x)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }

  char ch;
  if (!(in >> ch) || ch != x.ch)
  {
    in.setstate(std::ios::failbit);
  }
  return in;
}

std::istream& shramko::operator>>(std::istream& in, DoubleScienceT& x)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }

  in >> x.key;
  return in;
}

std::istream& shramko::operator>>(std::istream& in, UllBinT& x)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }

  StreamGuard guard(in);
  in >> ExpectCharT{'0'} >> ExpectCharT{'b'};

  x.key = 0;
  x.prefix_zeroes = 0;
  bool has_bits = false;
  char ch;
  while (in.get(ch))
  {
    if (ch != '0' && ch != '1')
    {
      in.unget();
      break;
    }

    if (ch == '0' && !has_bits)
    {
      ++x.prefix_zeroes;
      continue;
    }

    has_bits = true;
    if (x.key > (std::numeric_limits<unsigned long long>::max() >> 1))
    {
      in.setstate(std::ios::failbit);
      return in;
    }

    x.key <<= 1;
    if (ch == '1')
    {
      x.key |= 1;
    }
  }

  if (!has_bits && x.prefix_zeroes == 0)
  {
    in.setstate(std::ios::failbit);
  }

  return in;
}

std::istream& shramko::operator>>(std::istream& in, StringT& x)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }

  StreamGuard guard(in);
  in >> std::noskipws;
  in >> ExpectCharT{'"'};

  char next;
  while (in >> next && next != '"')
  {
    if (next == '\n')
    {
      in.setstate(std::ios_base::failbit);
      break;
    }
    x.key.push_back(next);
  }
  return in;
}

std::istream& shramko::operator>>(std::istream& in, dataStruct& ds)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }

  in >> ExpectCharT{'('};
  dataStruct result;

  bool is_key1 = false, is_key2 = false, is_key3 = false;
  while ((!is_key1 || !is_key2 || !is_key3) && in)
  {
    in >> ExpectCharT{':'};
    std::string keyName;
    in >> keyName;
    if (keyName == "key1" && !is_key1)
    {
      DoubleScienceT x;
      is_key1 = true;
      in >> x;
      result.key1 = x.key;
    }
    else if (keyName == "key2" && !is_key2)
    {
      UllBinT x;
      is_key2 = true;
      in >> x;
      result.key2 = x.key;
      result.key2_zeroes = x.prefix_zeroes;
    }
    else if (keyName == "key3" && !is_key3)
    {
      StringT x;
      is_key3 = true;
      in >> x;
      result.key3 = x.key;
    }
    else
    {
      in.setstate(std::ios::failbit);
    }
  }

  in >> ExpectCharT{':'} >> ExpectCharT{')'};
  if (in)
  {
    ds = result;
  }

  return in;
}
