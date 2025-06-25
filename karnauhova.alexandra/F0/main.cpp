#include <iostream>
#include <map>
#include <string>
#include <functional>
#include <limits>
#include "characters.hpp"
#include "input_file.hpp"
#include "terminal_text.hpp"
#include "interaction_game.hpp"

int main()
{
  setlocale(LC_ALL, "ru");
  using namespace karnauhova;
  std::map< size_t, Character > characters;
  std::map< std::string, Character > players;
  players = input_data(characters);
  if (players.size() != 0)
  {
    auto it = players.begin();
    if (it->second.position != 0 || (++it)->second.position != 0)
    {
      clear_screen();
      std::cout << "\033[1;31m" << std::string(100, '=') << "\033[0m" << "\n";
      std::cout << "Возвращение в прошлый бой:\n";
      //boy fight
    }
    clear_screen();
    std::cout << "\033[1;32m" << std::string(100, '=') << "\033[0m" << "\n";
    std::cout << "С возвращением " << it->first << ", " << (++it)->first << "!\n";
  }
  else
  {
    try
    {
      hello_world(players);
    }
    catch (const std::exception& e)
    {
      return 0;
    }
  }
  delay(1200);
  std::map< std::string, std::function< void() > > cmds;
  cmds["CHARACTERS"] = std::bind(choice_characters, std::ref(players), std::cref(characters));
  //cmds["FIGHT"] = std::bind(fight, std::ref(players), std::cref(characters));
  while (!std::cin.eof())
  {
    clear_screen();
    std::cout << "\033[1;32m" << std::string(100, '=') << "\033[0m" << "\n";
    std::cout << "-Лобби-\n\n" << ">Чтобы взять нового персонажа или поменять старого введите CHARACTERS\n";
    std::cout << ">Чтобы начать бой введите FIGHT\n";
    std::string answer;
    std::cin >> answer;
    std::cout << answer;
    try
    {
      std::cout << "meow\n";
      cmds.at(answer)();
    }
    catch (...)
    {
      if (std::cin.eof())
      {
        std::cin.clear();
        break;
      }
    }
    //catch (...)
    //{
     // return 1;
    //}
    
  }
//сохраить данные
}
