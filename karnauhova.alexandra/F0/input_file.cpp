#include "input_file.hpp"
#include <iostream>
#include <fstream>
#include <limits>
#include <iterator>
#include <vector>
#include <algorithm>

namespace
{
  struct TransformMap
  {
    TransformMap(size_t& index):
      index_(index)
    {}
    std::pair< size_t, karnauhova::Character > operator()(const karnauhova::Character& ch)
    {
      return std::make_pair(index_++, ch);
    }
    size_t& index_;
  };
}

std::map< std::string, karnauhova::Character > karnauhova::input_data(std::map< size_t, Character >& characters)
{
  setlocale(LC_ALL, "");
  std::map< std::string, karnauhova::Character > players;
  std::cout << "HELLO!\n" << "Do you wanna continue last game?\n" << "> NEW_GAME\n" << "> CONTINUE\n";
  std::cout << "Please choose answer(If you don't have last version peek continue)\n";
  std::string answer;
  bool end_input = false;
  while(!end_input)
  {
    std::cin >> answer;
    try
    {
      if (answer == "NEW_GAME")
      {
        std::ifstream file("game_data.txt");
        input_new_data(file, characters);
        end_input = true;
      }
      else if (answer == "CONTINUE")
      {
        std::ifstream file("game_data_save.txt");
        players = input_save_data(file, characters);
        end_input = true;
      }
      else if (std::cin.eof())
      {
        throw std::out_of_range("ERROR: EOF");
      }
    }
    catch(const std::out_of_range& o)
    {
      throw;
    }
    catch(const std::logic_error& l)
    {
      std::cout << "У вас нет доступного сохранения:(\n";
    }
  }
  return players;
}

void karnauhova::input_new_data(std::istream& in, std::map< size_t, Character >& characters)
{
  std::vector<Character> temp;
  while (!in.eof())
  {
    using istr_iterator = std::istream_iterator< Character >;
    std::copy(istr_iterator(in), istr_iterator(), std::back_inserter(temp));
    if (!in)
    {
      in.clear();
      in.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    }
  }
  size_t index = 1;
  TransformMap trmp(index);
  std::transform(temp.begin(), temp.end(), std::inserter(characters, characters.end()), trmp);
}

std::map< std::string, karnauhova::Character > karnauhova::input_save_data(std::istream& in, std::map< size_t, Character >& characters)
{
  if (in.peek() == std::ifstream::traits_type::eof())
  {
    throw std::logic_error("File empty");
  }
  int stage = 0;
  in >> stage;
  std::string name1, name2;
  in >> name1 >> name2;
  in >> std::ws;
  size_t index_character1 = 0, index_character2 = 0;
  int hp1 = 0, hp2 = 0, position1 = 0, position2 = 0;
  in >> index_character1 >> hp1 >> position1;
  in >> index_character2 >> hp2 >> position2;
  input_new_data(in, characters);
  std::map< std::string, karnauhova::Character > players;
  players[name1] = characters.at(index_character1);
  players.at(name1).position = position1;
  players.at(name1).damage(players.at(name1).volume_hp() - hp1);

  players[name2] = characters.at(index_character2);
  players.at(name2).position = position2;
  players.at(name2).damage(players.at(name2).volume_hp() - hp2);
  return players;
}
