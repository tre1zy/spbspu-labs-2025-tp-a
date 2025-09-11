#include "music-io-utils.hpp"
#include <cassert>
#include <iostream>
#include <stream-guard.hpp>

namespace
{
  bool isCorrectLetter(char l) noexcept
  {
    return 'A' <= l && l <= 'H';
  }

  bool isCorrectAccidental(char acc) noexcept
  {
    return acc == '#' || acc == 'b';
  }

  bool isCorrectPitch(char l, char p, char acc = '\0') noexcept
  {
    switch (p)
    {
    case '0':
      return l == 'H' || (l == 'A' && acc != 'b');
    case '8':
      return l == 'C' && !isCorrectAccidental(acc);
    default:
      return '1' <= p && p <= '7';
    }
  }
}

std::istream& aleksandrov::operator>>(std::istream& in, Note& note)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  Note temp;

  char l = '\0';
  in >> l;
  if (in && isCorrectLetter(l))
  {
    temp.letter = l;
  }
  else
  {
    in.setstate(std::ios::failbit);
    return in;
  }

  char c = '\0';
  in >> std::noskipws >> c;
  if (in && isCorrectAccidental(c))
  {
    char p = '\0';
    in >> p;
    if (in && isCorrectPitch(l, p, c))
    {
      temp.accidental = c;
      temp.pitch = p;
    }
    else
    {
      in.setstate(std::ios::failbit);
    }
  }
  else if (in && isCorrectPitch(l, c))
  {
    temp.pitch = c;
  }
  else
  {
    in.setstate(std::ios::failbit);
  }

  if (in)
  {
    note = std::move(temp);
  }
  return in >> std::skipws;
}

std::istream& aleksandrov::operator>>(std::istream& in, Interval& interval)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  Note first;
  if (in >> first)
  {
    char c = '\0';
    in.get(c);
    if (in && c == '-')
    {
      Note second;
      if (in >> second)
      {
        interval.first = first;
        interval.second = second;
      }
    }
    else
    {
      in.setstate(std::ios::failbit);
    }
  }
  return in;
}

std::istream& aleksandrov::operator>>(std::istream& in, MusicalElement& element)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  Note temp;
  in >> temp;
  if (in && in.peek() == '-')
  {
    in.get();
    Note yaTemp;
    if (in >> yaTemp)
    {
      Interval interval{ temp, yaTemp };
      MusicalElement el(interval);
      element = std::move(el);
    }
  }
  else
  {
    MusicalElement el(temp);
    element = std::move(el);
  }
  return in;
}

std::ostream& aleksandrov::operator<<(std::ostream& out, const Note& note)
{
  StreamGuard guard(out);
  return out << note.letter << note.accidental << note.pitch;
}

std::ostream& aleksandrov::operator<<(std::ostream& out, const Interval& interval)
{
  StreamGuard guard(out);
  return out << interval.first << '-' << interval.second;
}

std::ostream& aleksandrov::operator<<(std::ostream& out, const MusicalElement& element)
{
  StreamGuard guard(out);
  switch (element.getType())
  {
  case MusicalElementType::Note:
    return out << element.note();
  default:
    return out << element.interval();
  }
  assert(false && "Unsupported musical element!");
  return out;
}

