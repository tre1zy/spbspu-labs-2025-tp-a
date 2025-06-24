#ifndef INPUT_FILE_HPP
#define INPUT_FILE_HPP

#include <map>
#include "characters.hpp"

namespace karnauhova
{
  std::map< size_t, Character > input_data(std::map< size_t, Character >& characters);
  void input_new_data(std::istream& in, std::map< size_t, Character >& characters);
  std::map< size_t, Character > input_save_data(std::istream& in, std::map< size_t, Character >& characters);
}
#endif
