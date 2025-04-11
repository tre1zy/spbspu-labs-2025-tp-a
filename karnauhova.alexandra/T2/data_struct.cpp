#include "data_struct.hpp"
#include <sstream>
#include <string>
#include <exception>

std::istream& karnauhova::operator>>(std::istream& in, DelimiterIO&& dest)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  char c = '0';
  in >> c;
  if (in && (std::tolower(c) != dest.exp))
  {
    in.setstate(std::ios::failbit);
  }
  return in;
}

std::istream& karnauhova::operator>>(std::istream& in, DoubleIO&& dest)
{
  std::istream::sentry sentry(in);
  if (!sentry) {
    return in;
  }
  in >> dest.ref >> DelimiterIO{'d'};
  return in;
}

std::istream& karnauhova::operator>>(std::istream& in, UllIO&& dest)
{
  std::istream::sentry sentry(in);
  if (!sentry) {
    return in;
  }
  in >> dest.ref >> DelimiterIO{'u'} >> DelimiterIO{'l'} >> DelimiterIO{'l'};
  return in;
}

std::istream& karnauhova::operator>>(std::istream& in, StringIO&& dest)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  std::getline(in >> DelimiterIO{ '"' }, dest.ref, '"');
  return in >> DelimiterIO{ ':' };
}

std::istream& karnauhova::operator>>(std::istream& in, KeyIO& dest)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  std::string input = "";
  in >> input;
  if (input.substr(0, 3) != "key")
  {
    in.setstate(std::ios::failbit);
  }
  dest.num = input.back();
  return in;
}

std::istream& karnauhova::operator>>(std::istream& in, DataStruct& dest)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  DataStruct input;
  {
    using key = KeyIO;
    using symb = DelimiterIO;
    using dbl = DoubleIO;
    using ull = UllIO;
    using str = StringIO;
    in >> symb{'('} >> symb{':'};
    for (size_t i = 0; i < 3; i++)
    {
      key key_num{ 0 };
      in >> key_num;
      if (key_num.num == '1')
      {
        in >> dbl{input.key1} >> symb{':'};
      }
      else if (key_num.num == '2')
      {
        in >> ull{input.key2};
      }
      else if (key_num.num == '3')
      {
        in >> str{input.key3} >> symb{':'};
      }
    }
    in >> symb{')'};
  }
  if (in)
  {
    dest = input;
  }
  return in;
}