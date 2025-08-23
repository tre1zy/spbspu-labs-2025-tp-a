#include "music-io-utils.hpp"
#include <iostream>

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

  char c = '\0';
  in >> std::noskipws >> c;
  if (in && isCorrectAccidental(c))
  {
    char p = '\0';
    in >> p;
    if (in && isCorrectPitch(l, p, c))
    {
      temp.accidental = c;
      if (std::isspace(in.peek()) || in.peek() == EOF)
      {
        temp.pitch = p;
      }
      else
      {
        in.setstate(std::ios::failbit);
      }
    }
    else if (in && !std::isspace(p))
    {
      in.setstate(std::ios::failbit);
    }
  }
  else if (in && isCorrectPitch(l, c))
  {
    if (std::isspace(in.peek()) || in.peek() == EOF)
    {
      temp.pitch = c;
    }
    else
    {
      in.setstate(std::ios::failbit);
    }
  }

  if (in)
  {
    note = temp;
  }
  return in >> std::skipws;
}

std::istream& aleksandrov::operator>>(std::istream& in, MusicalElement& element)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  Note note;
  if (in >> note)
  {
    element.note_ = note;
  }
  return in;
}

std::ostream& aleksandrov::operator<<(std::ostream& out, const Note& note)
{
  return out << note.letter << note.accidental << note.pitch;
}

std::ostream& aleksandrov::operator<<(std::ostream& out, const MusicalElement& element)
{
  return out << element.note_;
}

void aleksandrov::readSequence(std::istream& in, Sequence& sequence)
{
  MusicalElement el;
  while (in >> el)
  {
    std::string name;
    name = el.note_.letter + el.note_.accidental + el.note_.pitch;
    sequence.push_back(el);
  }
}

