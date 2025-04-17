#include "data.h"
#include <sstream>
#include <cmath>
#include <iomanip>
#include <algorithm>
#include "iofmtguard.h"

std::istream& ohantsev::operator>>(std::istream& in, DelimiterIO&& dest)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  char c;
  in >> c;
  if (in && (c != dest.exp))
  {
    in.setstate(std::ios::failbit);
  }
  return in;
}

std::istream& ohantsev::operator>>(std::istream& in, MultDelimiterIO&& dest)
{
  for (auto chr: dest.exp)
  {
    in >> DelimiterIO{ chr };
  }
  return in;
}

std::istream& ohantsev::operator>>(std::istream& in, TypenameIO&& dest)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  std::string name;
  in >> name;
  if (in && std::find(dest.exp.cbegin(), dest.exp.cend(), name) == dest.exp.cend())
  {
    in.setstate(std::ios::failbit);
  }
  return in;
}

std::istream& ohantsev::operator>>(std::istream& in, DoubleI&& dest)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  std::string numberStr;
  std::getline(in, numberStr, ':');
  bool hasExponent = false;
  for (auto chr: numberStr)
  {
    if (chr == 'e' || chr == 'E')
    {
      hasExponent = true;
      break;
    }
  }
  if (!hasExponent)
  {
    in.setstate(std::ios::failbit);
    return in;
  }
  try
  {
    dest.ref = std::stod(numberStr);
  }
  catch (...)
  {
    in.setstate(std::ios::failbit);
  }
  return in;
}

std::istream& ohantsev::operator>>(std::istream& in, UllI&& dest)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  std::string ull = "";
  std::getline(in, ull, ':');
  if (ull != "" && ull[0] != '-')
  {
    std::istringstream ullSource(ull);
    ullSource >> dest.ref >> TypenameIO{ { "ull", "ULL" } };
    if ((!ullSource.fail()) && (ullSource.eof()))
    {
      return in;
    }
  }
  in.setstate(std::ios::failbit);
  return in;
}

std::istream& ohantsev::operator>>(std::istream& in, StringI&& dest)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  std::getline(in >> DelimiterIO{ '"' }, dest.ref, '"');
  return in >> DelimiterIO{ ':' };
}

std::istream& ohantsev::operator>>(std::istream& in, LabelIO&& dest)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  in >> MultDelimiterIO{ "key" };
  int ID;
  in >> ID;
  if (in) {
    auto iter = std::find(dest.notFilled.cbegin(), dest.notFilled.cend(), static_cast<KeyID>(ID));
    if (iter == dest.notFilled.cend())
    {
      in.setstate(std::ios::failbit);
    }
    else
    {
      dest.ID = *iter;
      dest.notFilled[iter - dest.notFilled.cbegin()] = KeyID::EMPTY;
    }
  }
  return in;
}

std::istream& ohantsev::operator>>(std::istream& in, KeyIO&& dest)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  KeyID ID;
  in >> LabelIO{ ID, dest.notFilled };
  if (!in.fail())
  {
    switch (ID)
    {
    case KeyID::DBL:
      in >> DoubleI{ dest.data.key1 };
      break;
    case KeyID::ULL:
      in >> UllI{ dest.data.key2 };
      break;
    case KeyID::STR:
      in >> StringI{ dest.data.key3 };
      break;
    case KeyID::EMPTY:
      in.setstate(std::ios::failbit);
      break;
    }
  }
  return in;
}

std::istream& ohantsev::operator>>(std::istream& in, Data& dest)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  iofmtguard guard(in);
  std::array<KeyID, Data::FIELDS_COUNT> labels{ KeyID::DBL, KeyID::ULL, KeyID::STR };
  in >> MultDelimiterIO{ "(:" };
  in >> KeyIO{ dest, labels };
  in >> KeyIO{ dest, labels };
  in >> KeyIO{ dest, labels };
  in >> MultDelimiterIO{ ")" };
  return in;
}

std::ostream& ohantsev::operator<<(std::ostream& out, DoubleO&& dest)
{
  double mant = dest.ref;
  int order = 0;
  if (mant != 0)
  {
    while (std::abs(mant) < 1)
    {
      mant *= 10;
      order--;
    }
    while (std::abs(mant) >= 9.95)
    {
      mant /= 10;
      order++;
    }
    mant = std::round(mant * 10) / 10.0;
  }
  out << std::fixed << std::setprecision(1) << mant;
  out << 'e' << ((order < 0) ? '-': '+') << std::abs(order);
  return out;
}

std::ostream& ohantsev::operator<<(std::ostream& out, UllO&& dest)
{
  return (out << dest.ref << "ull");
}

std::ostream& ohantsev::operator<<(std::ostream& out, StringO&& dest)
{
  return (out << '"' << dest.ref << '"');
}

std::ostream& ohantsev::operator<<(std::ostream& out, const Data& dest)
{
  std::ostream::sentry sentry(out);
  if (!sentry)
  {
    return out;
  }
  iofmtguard fmtguard(out);
  out << "(:key1 " << DoubleO{ dest.key1 } << ':';
  out << "key2 " << UllO{ dest.key2 } << ':';
  out << "key3 " << StringO{ dest.key3 } << ":)";
  return out;
}

bool ohantsev::Data::operator<(const ohantsev::Data& rhs)
{
  if (key1 != rhs.key1)
  {
    return key1 < rhs.key1;
  }
  if (key2 != rhs.key2)
  {
    return key2 < rhs.key2;
  }
  return key3.size() < rhs.key3.size();
}
