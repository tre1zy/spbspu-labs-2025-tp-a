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

aleksandrov::MusicalElement::MusicalElement(const MusicalElement& other):
  type_(other.type_)
{
  switch (type_)
  {
  case MusicalElementType::Note:
    new (std::addressof(note_)) Note(other.note_);
    break;
  case MusicalElementType::Interval:
    new (std::addressof(interval_)) Interval(other.interval_);
    break;
  case MusicalElementType::Chord:
    new (std::addressof(chord_)) Chord(other.chord_);
    break;
  default:
    break;
  }
}

aleksandrov::MusicalElement::MusicalElement(MusicalElement&& other) noexcept:
  type_(std::move(other.type_))
{
  switch (type_)
  {
  case MusicalElementType::Note:
    new (std::addressof(note_)) Note(std::move(other.note_));
    break;
  case MusicalElementType::Interval:
    new (std::addressof(interval_)) Interval(std::move(other.interval_));
    break;
  case MusicalElementType::Chord:
    new (std::addressof(chord_)) Chord(std::move(other.chord_));
    break;
  default:
    break;
  }
  other.type_ = MusicalElementType::None;
}

aleksandrov::MusicalElement& aleksandrov::MusicalElement::operator=(const MusicalElement& rhs)
{
  MusicalElement copy(rhs);
  swap(copy);
  return *this;
}

aleksandrov::MusicalElement& aleksandrov::MusicalElement::operator=(MusicalElement&& rhs) noexcept
{
  MusicalElement copy(std::move(rhs));
  swap(copy);
  return *this;
}

aleksandrov::MusicalElement::MusicalElement(const Note& note):
  type_(MusicalElementType::Note),
  note_(note)
{}

aleksandrov::MusicalElement::MusicalElement(const Interval& interval):
  type_(MusicalElementType::Interval),
  interval_(interval)
{}

aleksandrov::MusicalElement::MusicalElement(const Chord& chord):
  type_(MusicalElementType::Chord),
  chord_(chord)
{}

aleksandrov::MusicalElement::~MusicalElement() noexcept
{
  switch (type_)
  {
  case MusicalElementType::Note:
    note_.~Note();
    break;
  case MusicalElementType::Interval:
    interval_.~Interval();
    break;
  case MusicalElementType::Chord:
    chord_.~Chord();
    break;
  default:
    break;
  }
}

void aleksandrov::MusicalElement::swap(MusicalElement& other) noexcept
{
  std::swap(type_, other.type_);
  switch (type_)
  {
  case MusicalElementType::Note:
    std::swap(note_, other.note_);
    break;
  case MusicalElementType::Interval:
    std::swap(interval_, other.interval_);
    break;
  case MusicalElementType::Chord:
    std::swap(chord_, other.chord_);
    break;
  default:
    break;
  }
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

