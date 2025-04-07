#include "data-struct.hpp"
#include <cmath>

std::istream & savintsev::operator>>(std::istream & in, DelimiterIO && dest)
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

std::istream & savintsev::operator>>(std::istream & in, DoubleIO && dest)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }

  double value = 0.0;
  in >> value;

  if (!in)
  {
    return in;
  }

  dest.ref = value;
  return in;
}

std::istream & savintsev::operator>>(std::istream & in, StringIO && dest)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  return std::getline(in >> DelimiterIO{'"'}, dest.ref, '"');
}

std::istream & savintsev::operator>>(std::istream & in, UllIO && dest)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }

  std::ios::fmtflags f = in.flags();
  in >> std::oct >> dest.ref;
  in.flags(f);

  return in;
}

std::istream & savintsev::operator>>(std::istream & in, LabelIO && dest)
{
  return in >> dest.ref;
}

std::istream & savintsev::operator>>(std::istream & in, DataStruct & dest)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }

  savintsev::DataStruct temp{};
  bool has_key1 = false, has_key2 = false, has_key3 = false;

  try
  {
    in >> savintsev::DelimiterIO{'('};
    in >> savintsev::DelimiterIO{':'};

    for (int i = 0; i < 3; ++i)
    {
      std::string label;
      in >> savintsev::LabelIO{label};

      if (label == "key1")
      {
        in >> savintsev::DoubleIO{temp.key1};
        has_key1 = true;
      }
      else if (label == "key2")
      {
        in >> savintsev::UllIO{temp.key2};
        has_key2 = true;
      }
      else if (label == "key3")
      {
        in >> savintsev::StringIO{temp.key3};
        has_key3 = true;
      }
      else
      {
        throw std::runtime_error("Unknown key: " + label);
      }

      in >> savintsev::DelimiterIO{':'};
    }

    in >> savintsev::DelimiterIO{')'};

    if (!(has_key1 && has_key2 && has_key3))
    {
      throw std::runtime_error("Missing keys");
    }

    dest = temp;
  }
  catch (...)
  {
    in.setstate(std::ios::failbit);
  }

  return in;
}
