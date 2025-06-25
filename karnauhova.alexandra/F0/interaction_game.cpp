#include "interaction_game.hpp"
#include <iostream>
#include <algorithm>
#include <functional>
#include "terminal_text.hpp"

namespace
{
  void print_info(const std::pair<size_t, karnauhova::Character>& pair)
  {
    std::cout << pair.first << " ";
    pair.second.about_character();
    std::cout << "\n";
  }
}

void karnauhova::choice_characters(std::map< std::string, Character >& players, const std::map< size_t, Character >& characters)
{
  while (!std::cin.eof())
  {
    clear_screen();
    std::cout << "\033[1;33m" << std::string(100, '=') << "\033[0m" << "\n";
    std::cout << "Чтобы вернуться напишите EXITE\n\n";
    std::cout << "Напишите ник игрока, который будет выбирать персонажа:\n";
    std::string name;
    std::cin >> name;
    if (name == "EXIT")
    {
      return;
    }
    try
    {
      Character check = players.at(name);
      bool input_end = false;
      while (!input_end)
      {
        clear_screen();
        std::cout << "\033[1;33m" << std::string(100, '=') << "\033[0m" << "\n";
        std::cout << "Доступные персонажи:\n";
        std::for_each(characters.begin(), characters.end(), std::bind(print_info, std::placeholders::_1));
        std::cout << "\n" << "> GET_STORY <index> - более пожробная информация про персонажа\n" <<"> CHOOSE <index> - выбрать персонажа\n";
        std::string answer;
        if (!(std::cin >> answer))
        {
        if (std::cin.eof())
        {
            std::cin.clear();
            return;
        }
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        continue;
        }
        if (answer == "EXIT")
        {
          return;
        }
        else if (answer == "GET_STORY")
        {
          size_t index = 0;
          std::cin >> index;
          if (!std::cin || index == 0 || index > characters.size())
          {
            std::cin.clear();
            continue;
          }
          std::cout << characters.at(index).get_story() << "\n";
          delay(15000);
        }
        else if (answer == "CHOOSE")
        {
          size_t index = 0;
          std::cin >> index;
          if (!std::cin || index == 0 || index > characters.size())
          {
            std::cin.clear();
            continue;
          }
          players[name] = characters.at(index);
          std::cout << "\n" << characters.at(index).get_philosophy() << "\n";
          std::cout << "\n" << name << " выбрал " << players.at(name).get_name() << "\n";
          delay(2400);
          input_end = true;
        }
      
      }
    }
    catch (const std::exception& e)
    {
      std::cout << "Сложно ввести существующий ник?:(\n";
      delay(1200);
    }
    
  }
}
