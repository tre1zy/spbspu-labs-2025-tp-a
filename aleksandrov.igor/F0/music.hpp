#ifndef MUSIC_HPP
#define MUSIC_HPP

#include <deque>
#include <string>

namespace aleksandrov
{
  enum class NoteType;
  enum class MusicalElementType;

  struct Note;
  struct Interval;
  class MusicalElement;

  using Sequence = std::deque< MusicalElement >;
}

enum class aleksandrov::MusicalElementType
{
  Note,
  Interval,
  None
};

struct aleksandrov::Note
{
  char letter;
  char accidental;
  char pitch;
};

struct aleksandrov::Interval
{
  Note first;
  Note second;
};

class aleksandrov::MusicalElement
{
public:
  MusicalElement();
  explicit MusicalElement(const Note&);
  explicit MusicalElement(const Interval&);

  MusicalElementType getType() const noexcept;

  Note& note();
  const Note& note() const;
  Interval& interval();
  const Interval& interval() const;

  bool isNote() const noexcept;
  bool isInterval() const noexcept;
  bool isNone() const noexcept;

private:
  MusicalElementType type_;
  union
  {
    Note note_;
    Interval interval_;
  };
};

#endif

