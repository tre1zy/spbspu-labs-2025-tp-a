#include "output_save.hpp"
#include <fstream>
#include <algorithm>
#include <functional>
#include <iterator>
#include <vector>

namespace
{
  bool compare_characters(const std::pair<size_t, karnauhova::Character>& pair, const karnauhova::Character& target)
  {
    return pair.second == target;
  }
  struct ExtractCharacter
  {
    karnauhova::Character operator()(const std::pair<size_t, karnauhova::Character>& pair) const
    {
      return pair.second;
    }
};
}
void karnauhova::save_data(std::map< std::string, Character >& players, std::map< size_t, Character >& characters)
{
  std::ofstream file;
  file.open("karnauhova.alexandra/F0/game_data_save.txt");
  auto it = players.begin();
  file << it->first << " " << (++it)->first << "\n";
  auto character = std::find_if(characters.begin(), characters.end(), std::bind(compare_characters, std::placeholders::_1, std::cref((--it)->second)));
  file << character->first << " " << it->second.volume_hp() << " " << it->second.position << "\n";
  character = std::find_if(characters.begin(), characters.end(), std::bind(compare_characters, std::placeholders::_1, std::cref((++it)->second)));
  file << character->first << " " << it->second.volume_hp() << " " << it->second.position << "\n";

  using ostr_iter = std::ostream_iterator< Character >;
  std::vector< Character > data;
  data.reserve(characters.size());
  std::transform(characters.begin(), characters.end(), std::back_inserter(data), ExtractCharacter());
  std::copy(std::begin(data), std::end(data), ostr_iter(file, "\n"));
}
