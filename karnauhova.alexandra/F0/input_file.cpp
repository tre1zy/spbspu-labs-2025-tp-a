#include "input_file.hpp"
#include <iostream>
#include <fstream>

std::map< size_t, karnauhova::Character > karnauhova::input_data(std::map< size_t, Character >& characters)
{
  setlocale(LC_ALL, "");
  std::map< size_t, karnauhova::Character > players;
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
  size_t index = 0;
  while (!in.eof())
  {
    if (in >> index)
    {
        
    }

  }

}

//std::map< size_t, karnauhova::Character > karnauhova::input_save_data(std::istream& in, std::map< size_t, Character >& characters)
//{

//}
