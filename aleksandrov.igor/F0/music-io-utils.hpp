#ifndef MUSIC_IO_UTILS_HPP
#define MUSIC_IO_UTILS_HPP

#include <ios>
#include "music.hpp"

namespace aleksandrov
{
  std::istream& operator>>(std::istream&, Note&);
  std::istream& operator>>(std::istream&, Interval&);
  std::istream& operator>>(std::istream&, MusicalElement&);

  std::ostream& operator<<(std::ostream&, const Note&);
  std::ostream& operator<<(std::ostream&, const Interval&);
  std::ostream& operator<<(std::ostream&, const MusicalElement&);

  void readSequence(std::istream& in, Sequence& dest);
}

#endif

