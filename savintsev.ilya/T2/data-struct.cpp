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
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }

  std::string data = "";
  if ((in >> StringIO{data}) && (data != dest.exp))
  {
    in.setstate(std::ios::failbit);
  }
  return in;
}

std::istream & savintsev::operator>>(std::istream & in, DataStruct & dest)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }

  DataStruct input;
  bool has_key1 = false, has_key2 = false, has_key3 = false;

  try
  {
    in >> DelimiterIO{'{'};
    in >> DelimiterIO{':'};

    while (in.peek() != '}')
    {

      std::string label;
      if (!(in >> LabelIO{label}))
      {
        throw std::runtime_error("Expected key");
      }

      if (label == "key1")
      {
        if (!(in >> DoubleIO{input.key1}))
        {
          throw std::runtime_error("Failed to read key1");
        }
        has_key1 = true;
      }
      else if (label == "key2")
      {
        if (!(in >> UllIO{input.key2}))
        {
          throw std::runtime_error("Failed to read key2");
        }
        has_key2 = true;
      }
      else if (label == "key3")
      {
        if (!(in >> StringIO{input.key3}))
        {
          throw std::runtime_error("Failed to read key3");
        }
        has_key3 = true;
      }
      else
      {
        throw std::runtime_error("Unknown key: " + label);
      }
    }

    in >> DelimiterIO{'}'};

    if (!(has_key1 && has_key2 && has_key3))
    {
      throw std::runtime_error("Missing required key(s)");
    }

    dest = input;
  }
  catch (...)
  {
    in.setstate(std::ios::failbit);
  }

  return in;
}
