#ifndef MUSIC_HPP
#define MUSIC_HPP

#include <unordered_map>
#include <deque>
#include <string>

namespace aleksandrov
{
  struct Note
  {
    char letter;
    char accidental;
    char pitch;
  };

  struct MusicalElement
  {
    Note note_;
  };

  using Sequence = std::deque< MusicalElement >;
}

#endif

