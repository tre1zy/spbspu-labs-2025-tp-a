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
    if (name == "EXITE")
    {
      return;
    }
    try
    {
      Character check = players.at(name);
      std::cout << "Доступные персонажи:\n";
      std::for_each(characters.begin(), characters.end(), std::bind(print_info, std::placeholders::_1));
      std::cout << "Напишите индекс персонажа:\n";
      size_t index = 0;
      std::cin >> index;

    }
    catch (const std::exception& e)
    {
      std::cout << "Сложно ввести существующий ник?:(\n";
      delay(1200);
    }
    
  }
}
