#include "music.hpp"
#include <cassert>

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

bool aleksandrov::MusicalElement::isNote() const noexcept
{
  return type_ == MusicalElementType::Note;
}

bool aleksandrov::MusicalElement::isInterval() const noexcept
{
  return type_ == MusicalElementType::Interval;
}

bool aleksandrov::MusicalElement::isNone() const noexcept
{
  return type_ == MusicalElementType::None;
}

