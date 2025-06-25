#ifndef TERMINAL_TEXT_HPP
#define TERMINAL_TEXT_HPP

#include <map>
#include "characters.hpp"

namespace karnauhova
{
  void clear_screen();
  void hello_world(std::map< std::string, Character >& players);

}
#endif
