#include <iostream>
#include <map>
#include <string>
#include "characters.hpp"
#include "input_file.hpp"
#include "terminal_text.hpp"

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
    catch(const std::exception& e)
    {
      return 0;
    }
  }
  auto it = players.begin();
  size_t m = 0;
  std::cin >> m;
  std::cout << it->second.get_name();
  //считать данные
  //std::cout << imp.attack();
  //while (!std::cin.eof())
  //{
  //}
  //сохранить данные
}
