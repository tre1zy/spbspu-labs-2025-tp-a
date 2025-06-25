#include "input_file.hpp"
#include <iostream>
#include <fstream>
#include <limits>
#include <iterator>
#include <vector>
#include <algorithm>
#include <filesystem>
#include "terminal_text.hpp"

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
  setlocale(LC_ALL, "ru");
  std::map< std::string, karnauhova::Character > players;
  clear_screen();
  std::cout << "\033[1;32m" << std::string(100, '=') << "\033[0m" << "\n";
  std::cout << "Привет!\n" << "Хотите продолжить прошлую или начать новую?\n" << "> NEW_GAME\n" << "> CONTINUE\n";
  std::cout << "Пожалуйста выберите ответ(Если у вас нет прошлого сохранения выберите NEW_GAME)\n";
  std::string answer;
  bool end_input = false;
  while(!end_input)
  {
    std::cin >> answer;
    try
    {
      if (answer == "NEW_GAME")
      {
        std::ifstream file;
        file.open("karnauhova.alexandra/F0/game_data.txt");
        input_new_data(file, characters);
        end_input = true;
      }
      else if (answer == "CONTINUE")
      {
        std::ifstream file;
        file.open("karnauhova.alexandra/F0/game_data_save.txt");
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
      std::cin.clear();
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
  in.seekg(0, std::ios::end);
  if (in.tellg() == std::streampos(0))
  { 
    throw std::logic_error("File is empty");
  }
  in.seekg(0);
  std::string name1, name2;
  in >> name1 >> name2;
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
