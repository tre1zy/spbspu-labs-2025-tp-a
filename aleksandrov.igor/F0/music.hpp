#ifndef MUSIC_HPP
#define MUSIC_HPP

#include <map>
#include <deque>
#include <string>

namespace aleksandrov
{
  enum class NoteType;
  enum class MusicalElementType;

  struct Note;
  struct Interval;
  struct Chord;
  class MusicalElement;

  using Sequence = std::deque< MusicalElement >;
}

enum class aleksandrov::MusicalElementType
{
  Note,
  Interval,
  Chord,
  None
};

struct aleksandrov::Note
{
  char letter;
  char accidental;
  char pitch;

  std::string toString() const;
};

struct aleksandrov::Interval
{
  Note first;
  Note second;
};

struct aleksandrov::Chord
{
  std::map< std::string, Note > notes;
};

class aleksandrov::MusicalElement
{
public:
  MusicalElement();
  MusicalElement(const MusicalElement&);
  MusicalElement(MusicalElement&&) noexcept;
  explicit MusicalElement(const Note&);
  explicit MusicalElement(const Interval&);
  explicit MusicalElement(const Chord&);
  ~MusicalElement() noexcept;

  MusicalElement& operator=(const MusicalElement&);
  MusicalElement& operator=(MusicalElement&&) noexcept;

  void swap(MusicalElement&) noexcept;

  MusicalElementType getType() const noexcept;

  Note& note();
  const Note& note() const;
  Interval& interval();
  const Interval& interval() const;
  Chord& chord();
  const Chord& chord() const;

  bool isNote() const noexcept;
  bool isInterval() const noexcept;
  bool isChord() const noexcept;
  bool isNone() const noexcept;

private:
  MusicalElementType type_;
  union
  {
    Note note_;
    Interval interval_;
    Chord chord_;
  };
};

#endif

