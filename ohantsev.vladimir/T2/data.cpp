#include "data.h"
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
  char c = '0';
  in >> c;
  if (in && (c != dest.exp))
  {
    in.setstate(std::ios::failbit);
  }
  return in;
}

std::istream& ohantsev::operator>>(std::istream& in, MultDelimiterIO&& dest)
{
  for (auto chr : dest.exp)
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
  std::string s = "";
  std::getline(in, s, ':');
  if (in && std::find(dest.exp.begin(), dest.exp.end(), s) == dest.exp.end())
  {
    in.setstate(std::ios::failbit);
  }
  return in;
}

std::istream& ohantsev::operator>>(std::istream& in, DoubleIO&& dest)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  return in >> dest.ref >> DelimiterIO{ ':' };
}

std::istream& ohantsev::operator>>(std::istream& in, UllIO&& dest)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  in >> dest.ref >> TypenameIO{ { "ull", "ULL" } };
  return in;
}

std::istream& ohantsev::operator>>(std::istream& in, StringIO&& dest)
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
  int LabelID = 0;
  in >> LabelID;
  if (in) {
    auto iter = std::find(dest.notFilled.begin(), dest.notFilled.end(), LabelID);
    if (iter == dest.notFilled.end())
    {
      in.setstate(std::ios::failbit);
    }
    else
    {
      dest.labelID = *iter;
      dest.notFilled.erase(iter);
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
  int ID;
  in >> LabelIO{ ID, dest.notFilled };
  switch (ID)
  {
  case 1:
    in >> DoubleIO{ dest.data.key1 };
    break;
  case 2:
    in >> UllIO{ dest.data.key2 };
    break;
  case 3:
    in >> StringIO{ dest.data.key3 };
    break;
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
  std::vector<int> labels{ 1,2,3 };
  in >> MultDelimiterIO{ "(:" };
  in >> KeyIO{ dest, labels };
  in >> KeyIO{ dest, labels };
  in >> KeyIO{ dest, labels };
  in >> MultDelimiterIO{ ")" };
  return in;
}
std::ostream& ohantsev::operator<<(std::ostream& out, const Data& dest)
{
  std::ostream::sentry sentry(out);
  if (!sentry)
  {
    return out;
  }
  iofmtguard fmtguard(out);
  out << std::fixed << std::setprecision(1) << std::scientific;
  out << "(:key1 " << dest.key1 << ':';
  out << "key2 " << dest.key2 << "ull:";
  out << "key3 " << '"' << dest.key3 << '"' << ":)";
  return out;
}
