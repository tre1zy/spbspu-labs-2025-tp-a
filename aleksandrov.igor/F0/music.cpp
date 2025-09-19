#include "music.hpp"
#include <cassert>

std::string aleksandrov::Note::toString() const
{
  std::string str;
  str += letter;
  if (accidental != '\0')
  {
    str += accidental;
  }
  str += pitch;
  return str;
}

aleksandrov::MusicalElement::MusicalElement():
  type_(MusicalElementType::None)
{}

aleksandrov::MusicalElement::MusicalElement(const Note& note):
  type_(MusicalElementType::Note),
  note_(note)
{}

aleksandrov::MusicalElement::MusicalElement(const Interval& interval):
  type_(MusicalElementType::Interval),
  interval_(interval)
{}

aleksandrov::MusicalElement::MusicalElement(const Chord& chord):
  type_(MusicalElementType::Chord)
{
  new (std::addressof(chord_)) Chord(chord);
}

aleksandrov::MusicalElement::~MusicalElement() noexcept
{
  if (isChord())
  {
    chord_.~Chord();
  }
}

aleksandrov::MusicalElement::MusicalElement(const MusicalElement& other):
  type_(other.type_)
{
  switch (type_)
  {
  case MusicalElementType::Note:
    note_ = other.note_;
    break;
  case MusicalElementType::Interval:
    interval_ = other.interval_;
    break;
  case MusicalElementType::Chord:
    new (std::addressof(chord_)) Chord(other.chord_);
    break;
  default:
    break;
  }
}

aleksandrov::MusicalElement& aleksandrov::MusicalElement::operator=(const MusicalElement& rhs)
{
  if (isChord())
  {
    chord_.~Chord();
  }
  switch (rhs.type_)
  {
  case MusicalElementType::Note:
    note_ = rhs.note_;
    break;
  case MusicalElementType::Interval:
    interval_ = rhs.interval_;
    break;
  case MusicalElementType::Chord:
    new (std::addressof(chord_)) Chord(rhs.chord_);
    break;
  default:
    break;
  }
  type_ = rhs.type_;
  return *this;
}

aleksandrov::MusicalElementType aleksandrov::MusicalElement::getType() const noexcept
{
  return type_;
}

aleksandrov::Note& aleksandrov::MusicalElement::note()
{
  assert(isNote());
  return const_cast< Note& >(static_cast< const MusicalElement& >(*this).note());
}

const aleksandrov::Note& aleksandrov::MusicalElement::note() const
{
  assert(isNote());
  return note_;
}

aleksandrov::Interval& aleksandrov::MusicalElement::interval()
{
  assert(isInterval());
  return const_cast< Interval& >(static_cast< const MusicalElement& >(*this).interval());
}

const aleksandrov::Interval& aleksandrov::MusicalElement::interval() const
{
  assert(isInterval());
  return interval_;
}

aleksandrov::Chord& aleksandrov::MusicalElement::chord()
{
  assert(isChord());
  return const_cast< Chord& >(static_cast< const MusicalElement& >(*this).chord());
}

const aleksandrov::Chord& aleksandrov::MusicalElement::chord() const
{
  assert(isChord());
  return chord_;
}

bool aleksandrov::MusicalElement::isNote() const noexcept
{
  return type_ == MusicalElementType::Note;
}

bool aleksandrov::MusicalElement::isInterval() const noexcept
{
  return type_ == MusicalElementType::Interval;
}

bool aleksandrov::MusicalElement::isChord() const noexcept
{
  return type_ == MusicalElementType::Chord;
}

bool aleksandrov::MusicalElement::isNone() const noexcept
{
  return type_ == MusicalElementType::None;
}

